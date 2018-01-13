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
variable_val [ 0-9\"\'A-Z@a-z\/\.\\\\]+$
%%

		/* variable asignment */
set		{
			state = SET;
			return SET;
		}

		/* possible variable names in tmail */
"smtp.realname"		|
"smtp.server"		|
"smtp.from"		|
"smtp.port"		|
"smtp.password"		|
"smtp.signature"	{
				if (yylval.var.variable_name)
					mfree(yylval.var.variable_name);
				yylval.var.variable_name = strdup(yytext);
				state = VARIABLE_NAME;
				return VARIABLE_NAME;
			}
		/* variables values */
{variable_val}		{
				int i, len = strlen(yytext);

				i = len - 1;

				while (i != 0)
				{
					if (!isspace(yytext[i]))
					{
						if (yytext[i] != '\\' && state != VARIABLE_VAL_PART)
						{
							yylval.var.variable_val = strdup(yytext);
							if (state != VARIABLE_END)
								state = VARIABLE_VAL;
							else
								state = VARIABLE_END;
							return state;
						}
						else
						{
							/* match multiline configuration values */
							if (state == VARIABLE_VAL_PART)
							{
								if (yytext[len - 1] == '\\')
								{
									yylval.var.variable_val = strndup(yytext, len - 1);
								        state = VARIABLE_VAL_PART;
								}
								else
								{
									yylval.var.variable_val = strndup(yytext, len);
									state = VARIABLE_END;
								}
								return VARIABLE_END;
							}

							yylval.var.variable_val = strndup(yytext, len - 1);
							state = VARIABLE_VAL_PART;
							return VARIABLE_VAL_PART;
						}
					}
					i--;
				}
			}
 
		/* operators */
{assign}	{
			state = ASSIGN;
			return ASSIGN;
		}

		/* comments */
#.*		;

		/* everything else */
[ \t\r\n]*	{ /* ignore spaces*/ }

.		{ printf("bad input character '%s' at line %d\n", yytext, yylineno); }

%%
