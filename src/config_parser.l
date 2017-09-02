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

/*
 * By default flex uses stdin as the primary source of data.
 * We should overwrite it with `configuration_data` from
 * the config_parser.y file.
 */
#define YY_INPUT(buf,result,max_size)			\
{							\
	int c = *configuration_data;			\
							\
	configuration_data++;				\
	result = (c == 0) ? YY_NULL : (buf[0] = c, 1);	\
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

/* set var = variable_val*/
variable_val [ 0-9\"\'A-Z@a-z\/\.]+$
%%

		/* variable asignment */
set		{ return SET; }

		/* possible variable names in tmail */
"smtp.realname"		|
"smtp.server"		|
"smtp.from"		|
"smtp.port"		|
"smtp.password"		|
"smtp.signature"	{
				yylval.var.variable_name = strdup(yytext);
				return VARIABLE_NAME;
			}
		/* variables values */
{variable_val}		{
				yylval.var.variable_val = strdup(yytext);
				return VARIABLE_VAL;
			}
 
		/* operators */
{assign}	{ return ASSIGN; }

		/* comments */
#.*		;

		/* everything else */
[ \t\r\n]*	{ /* ignore spaces*/ }

.		{ printf("bad input character '%s' at line %d\n", yytext, yylineno); }

%%
