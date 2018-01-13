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
#include <search.h>

/* headers from tmail */
#include <smtp.h>
#include <config.h>
#include <hashmap.h>

/* bison API */
extern int yylex(void);
int yylex_destroy(void);
void yyerror(char const *s);

/* parser internal API */
void fill_smtp_conf(char *name, char *val);
void set_val(char *name, char **key, char *val, int state);

/* internal parser data structures */
smtp_ctx_t *smtp_conf;
int current_type;
%}

%code requires {
	struct var {
		char *variable_name;
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

/**
 * trim - removes leading and trailing space characters
 * from the given string.
 *
 * Result should be released by the callee.
 */
char *trim(const char *str)
{
	char *out = NULL;
	const char *end;
	size_t len = strlen(str);
	size_t out_size = 0;

	if(len == 0)
		return NULL;

	out = malloc(len + 1);
	if (!out)
		return NULL;
	memset(out, 0, len + 1);

	/* remove leading spaces */
	while(isspace((unsigned char)*str))
		str++;

	/* if all spaces */
	if(*str == 0)
	{
		mfree(out);
		return NULL;
	}

	/* Trim trailing space */
	end = str + strlen(str) - 1;
	while (end > str &&
	       (isspace((unsigned char)*end) || (unsigned char)*end == '\\'))
		end--;
	end++;

	/*
	 * calculate and copy copy trimmed string
	 * and add null terminator
	 */
	memcpy(out, str, end - str);
	return out;
}

void fill_smtp_conf(char *name, char *val)
{
	if (strcmp(name, "smtp.realname") == 0)
		set_val(name, &smtp_conf->realname, val, state);
	else if (strcmp(name, "smtp.server") == 0)
		set_val(name, &smtp_conf->smtp_server, val, state);
	else if (strcmp(name, "smtp.port") == 0)
	{
		set_val(name, &smtp_conf->smtp_port, val, state);
		if (strcmp(val, "578") == 0)
			smtp_conf->tls = true;
	}
	else if (strcmp(name, "smtp.password") == 0)
		set_val(name, &smtp_conf->password, val, state);
	else if (strcmp(name, "smtp.from") == 0)
		set_val(name, &smtp_conf->from, val, state);
	else if (strcmp(name, "smtp.signature") == 0)
	{
		char *signature_path = NULL;
		fd_t signature_fd = 0;

		signature_path = trim(val);
		if (!signature_path)
		{
			fprintf(stderr, "Error: Can't allocate memory for trimming of %s\n", val);
			exit(EXIT_FAILURE);
		}

		signature_fd = open(signature_path, O_RDONLY);
		if (signature_fd == -1)
		{
			free(signature_path);
			fprintf(stderr, "Error: Can't open signature file %s\n", val);
			exit(EXIT_FAILURE);
		}
		smtp_conf->signature_fd = signature_fd;
		free(signature_path);
		free(name);
	}

	free(val);
}

void set_val(char *name, char **key, char *val, int state)
{
	char *value = strdup(val);

	if (!value)
	{
		fprintf(stderr, "Error: can't allocate memory for trim\n");
		exit(EXIT_FAILURE);
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

		if (!key)
		{
			fprintf(stderr, "Error: trim failed for string - %s\n", value);
			exit(EXIT_FAILURE);
		}
	}

	mfree(value);
}

void yyerror(char const *s)
{
	fprintf(stderr, "Error: (yyerror) %s\n", s);
}

int parse_tmail_configuration(char *filename,
			      char *configuration,
			      int type)
{
	int ret = 0;
	ENTRY ep;

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
			exit(EXIT_FAILURE);
		}
		memset(smtp_conf, 0, sizeof(smtp_ctx_t));

		/* start to parse */
		ret = yyparse();

		/* store result */
		ep.key = strdup(basename(filename));
		ep.data = (void *)smtp_conf;
		hsearch(ep, ENTER);

		if (ret == 1)
		{
			yylex_destroy();
			fprintf(stderr, "Error: wrong configuration in %s configuration file\n",
				filename);
			return 0;
		}

		if (ret == 2)
		{
			yylex_destroy();
			fprintf(stderr, "Error: parsing of configuration file is failed"
				"due to memory exhaustion\n");
			return 0;
		}

		/* last name will not be free in lexer, so we should do it here */
		free(yylval.var.variable_name);

		break;
	}

	yylex_destroy();

	return 1;
}
