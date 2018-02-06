/*
 * config_parser.y - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

%{

/* stdlib headers */
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

/* headers from tmail */
#include <tmail/smtp.h>
#include <tmail/config.h>
#include <tmail/hashmap.h>
#include <tmail/string_utils.h>

/* bison API */
extern int yylex(void);
int yylex_destroy(void);
void yyerror(char const *s);

/* parser internal API */
void fill_smtp_conf(int name, char *val);
void set_val(char **key, char *val, int state);

/* internal parser data structures */
smtp_ctx_t *smtp_conf;
int current_type;
%}

%code requires {
	struct var {
		int   variable_name;
		char *variable_val;
	};
}

%union
{
	struct var var;
}

/* set variables related tokens */
%token SET
%token VARIABLE_NAME
%token VARIABLE_VAL
%token VARIABLE_VAL_PART
%token VARIABLE_END
%token ASSIGN

%start configuration

/* grammar description */
%%

configuration:	/* emtpy */
		| configuration conf_statement
		;
conf_statement:	set_stmt
		;
		/* set var = val */
set_stmt:	SET set_expr { }
	;

set_stmt:	VARIABLE_VAL_PART
		{
			if (current_type == SMTP_CONF)
				fill_smtp_conf(yylval.var.variable_name,
					       yylval.var.variable_val);
		}
	;
set_stmt:	VARIABLE_END
		{
			if (current_type == SMTP_CONF)
				fill_smtp_conf(yylval.var.variable_name,
					       yylval.var.variable_val);
		}
	;
set_expr:	VARIABLE_NAME ASSIGN VARIABLE_VAL
		{
			if (current_type == SMTP_CONF)
				fill_smtp_conf(yylval.var.variable_name,
					       yylval.var.variable_val);
		}
	;
set_expr:	VARIABLE_NAME ASSIGN VARIABLE_VAL_PART
		{
			if (current_type == SMTP_CONF)
				fill_smtp_conf(yylval.var.variable_name,
					       yylval.var.variable_val);
		}
	;
%%

static void destroy_smtp_conf()
{
	if (smtp_conf)
	{
		if (yylval.var.variable_val)
			mfree(yylval.var.variable_val);

		if (smtp_conf->realname)
			mfree(smtp_conf->realname);
		if (smtp_conf->smtp_server)
			mfree(smtp_conf->smtp_server);
		if (smtp_conf->smtp_port)
			mfree(smtp_conf->smtp_port);
		if (smtp_conf->password)
			mfree(smtp_conf->password);
		if (smtp_conf->from)
			mfree(smtp_conf->from);
		if (smtp_conf->signature_fd)
			close(smtp_conf->signature_fd);

		mfree(smtp_conf);
	}
}

void fill_smtp_conf(int name, char *val)
{
	if (name == SMTP_REALNAME)
		set_val(&smtp_conf->realname, val, state);
	else if (name == SMTP_SERVER)
		set_val(&smtp_conf->smtp_server, val, state);
	else if (name == SMTP_PORT)
	{
		set_val(&smtp_conf->smtp_port, val, state);
		if (strcmp(val, "578") == 0)
			smtp_conf->tls = true;
	}
	else if (name == SMTP_PASSWORD)
		set_val(&smtp_conf->password, val, state);
	else if (name == SMTP_FROM)
		set_val(&smtp_conf->from, val, state);
	else if (name == SMTP_SIGNATURE)
	{
		char *signature_path = NULL;
		fd_t signature_fd = 0;

		signature_path = trim(val);
		if (!signature_path)
		{
			fprintf(stderr, "Error: Can't allocate memory for trimming of %s\n", val);
			return;
		}

		signature_fd = open(signature_path, O_RDONLY);
		if (signature_fd == -1)
		{
			free(signature_path);
			fprintf(stderr, "Error: Can't open signature "
				"file %s\n", val);
			return;
		}
		smtp_conf->signature_fd = signature_fd;
		free(signature_path);
	}

	free(val);
}

void set_val(char **key, char *val, int state)
{
	char *value = strdup(val);

	if (!value)
	{
		fprintf(stderr, "Error: can't allocate memory"
			"for triming %s configuration option\n", *key);
		return;
	}

	if (*key != NULL)
	{
		char *k_bckp = strdup(*key);
		size_t key_len = strlen(*key);
		size_t val_len = strlen(value);
		size_t total_size = key_len + val_len + 2;

		char *v = trim(value);
		*key = realloc(*key, total_size);
		memset(*key, 0, total_size);
		memcpy(*key, k_bckp, key_len);
		memcpy(*key + key_len, v, val_len);

		free(v);
		free(k_bckp);
	}
	else
	{
		*key = trim(value);

		if (!*key) {
			fprintf(stderr, "Error: trim failed for configuration"
				" value - %s\n", value);
			return;
		}
	}

	mfree(value);
}

void yyerror(char const *s)
{
	fprintf(stderr, "Error: during parsing of configuration - %s\n", s);
}

int parse_tmail_configuration(char *filename,
			      char *configuration,
			      int type)
{
	int ret = 0;
	char *config_key = NULL;

	/*
	 * Here we are adding `\n` as the last symbol of configuration.
	 * For now this is pretty important as parser is a little bit
	 * broken and it may crash in a case if last byte of configuration
	 * is not '\n'.
	 */
	configuration_data = configuration;
	configuration_data[strlen(configuration)] = '\n';

	switch (type)
	{
	case SMTP_CONF:
		current_type = type;
		/*
		 * we support multiple configuration files,
		 * so it might be already created
		 */
		smtp_conf = malloc(sizeof(smtp_ctx_t));
		if (!smtp_conf)
		{
			fprintf(stderr, "Error: Can't allocate space for a smtp_conf\n");
			return 0;
		}
		memset(smtp_conf, 0, sizeof(smtp_ctx_t));

		/* start to parse */
		ret = yyparse();

		/* store result */
		config_key = strdup(basename(filename));
		hashmap_put(config_map, config_key, (void *)smtp_conf);
		/* data will be stored in hashmap, we don't need here anymore*/
		mfree(config_key);

		if (ret == 1)
		{
			destroy_smtp_conf();
			yylex_destroy();
			fprintf(stderr, "Error: wrong configuration in %s configuration file\n",
				filename);
			return 0;
		}

		if (ret == 2)
		{
			destroy_smtp_conf();
			yylex_destroy();
			fprintf(stderr, "Error: parsing of configuration file is failed"
				"due to memory exhaustion\n");
			return 0;
		}

		break;
	}

	yylex_destroy();

	return 1;
}
