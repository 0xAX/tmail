/*
 * config_parser.l - routines to parse and manage tmail configuration.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

%{
#define YY_DECL int yylex()

#include <config.h>
#include <config_parser.tab.h>

#undef YY_INPUT

#define NEWLINE '\n'
#define COMMENT '#'

/*
 * By default flex uses stdin as the primary source of data.
 * We should overwrite it with `configuration_data` from
 * the config_parser.y file.
 */
#define YY_INPUT(buf,result,max_size)				\
{								\
	int c = *configuration_data;				\
								\
	/* skip comments in the middle of a line */		\
	if ((char)c == COMMENT)					\
	{							\
		while ((char)*configuration_data != NEWLINE)	\
			configuration_data++;			\
		c = *configuration_data;			\
	}							\
	configuration_data++;					\
	result = (c == 0) ? YY_NULL : (buf[0] = c, 1);		\
}
%}

/* This tells flex to read only one input file */
%option noyywrap
/* do not pay attention fo case sensitiviness */
%option case-sensitive
/* force 'flex' to calculate line number */
%option yylineno

/* assignment used in `set var = val`*/
assign "="
/* comment lines should be ignored */
comment #.*
/* all spaces should be ignored */
spaces	[ \t\r\n]*
/* set var = variable_val*/
variable_val [ 0-9_\"\'A-Z@a-z\/\.\\\\#]+$
%%

		/* variable asignment */
set		{
			state = SET;
			return SET;
		}

		/* possible SMTP variable names in tmail */
"smtp.realname"		{
				yylval.var.variable_name = SMTP_REALNAME;
				return VARIABLE_NAME;
			}
"smtp.server"		{
				yylval.var.variable_name = SMTP_SERVER;
				return VARIABLE_NAME;
			}
"smtp.from"		{
				yylval.var.variable_name = SMTP_FROM;
				return VARIABLE_NAME;
			}
"smtp.port"		{
				yylval.var.variable_name = SMTP_PORT;
				return VARIABLE_NAME;
			}
"smtp.password"		{
				yylval.var.variable_name = SMTP_PASSWORD;
				return VARIABLE_NAME;
			}
"smtp.signature"	{
				yylval.var.variable_name = SMTP_SIGNATURE;
				return VARIABLE_NAME;
			}
		/* variables values */
{variable_val}		{
				int i, len = strlen(yytext);

				i = len - 1;

				/* skip all spaces from right side */
				while (i != 0)
				{
					if (!isspace(yytext[i]))
						break;
					i--;
				}

				if (yytext[i] == '\\')
				{
					yylval.var.variable_val = strndup(yytext, i);
					state = VARIABLE_VAL_PART;
					return VARIABLE_VAL_PART;
				}

				if (state == VARIABLE_VAL_PART)
				{
					yylval.var.variable_val = strndup(yytext, i + 1);
					state = VARIABLE_END;
					return VARIABLE_END;
				}

				yylval.var.variable_val = strndup(yytext, i + 1);
				state = VARIABLE_VAL;
				return VARIABLE_VAL;

			}
 
		/* operators */
{assign}	{
			state = ASSIGN;
			return ASSIGN;
		}

		/* comments */
{comment}	;

		/* everything else */
{spaces}	{ /* ignore spaces*/ }

.		{ printf("bad input character '%s' at line %d\n", yytext, yylineno); }

%%
