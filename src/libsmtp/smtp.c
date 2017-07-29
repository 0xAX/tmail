/*
 * smtp.c - implementation of an SMTP client.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "smtp.h"

static __attribute__((pure)) void skip_cl_rl(char *str)
{
	while (*str != '\r')
		str++;
	str += 2;
}

__attribute__((pure, unused)) char *smtp_cap_to_str(unsigned long cap)
{
	if (cap & SIZE)
		return "SIZE";

	if (cap & EIGHT_BITMIME)
		return "8BITMIME";

	if (cap & HELP)
		return "HELP";

	if (cap & PIPELINING)
		return "PIPELINING";

	if (cap & PRDR)
		return "PRDR";

	if (cap & CHUNKING)
		return "CHUNKING";

	if (cap & SMTPTLS)
		return "STARTTLS";

	if (cap & ENHANCEDSTATUSCODES)
		return "ENHANCEDSTATUSCODES";

	if (cap & SMTPUTF8)
		return "SMTPUTF8";

	return NULL;
}

/*
 * Parse SMTP capabilities that we receive from EHLO response.
 *
 * RFC 5321 denotes it as:
 *
 * ehlo-ok-rsp = ( "250" SP Domain [ SP ehlo-greet ] CRLF )
 *               / ( "250-" Domain [ SP ehlo-greet ] CRLF
 *                *( "250-" ehlo-line CRLF )
 *                   "250" SP ehlo-line CRLF )
 */
__attribute__((pure)) unsigned long parse_smtp_caps(char *r)
{
	/* bitmap of a SMTP server capabilities */
	bitmap_t smtp_caps = 0;

	/* skip greetings and parse SMTP capabilites */
	skip_cl_rl(r);

	while (r[0] != 0)
	{
		if (r[0] == ' ' || r[0] == '\r' || r[0] == '\n')
		{
			r++;
			continue;
		}
		/*
		 * skip first 4 bytes for code as described in rfc5321
		 * (4.1.1.1.)
		 */
		r += 4;

		/* get capability name and set it in the bitmap */
		if (strncmp(r, "SIZE", 4) == 0)
		{
			/* skip SIZE */
			r += 4;

			/* go to the next line */
			skip_cl_rl(r);
			smtp_caps |= SIZE;
			continue;
		}

		ADD_SIMPLE_SMTP_CAPABILITY("HELP", 4, r, HELP);
		ADD_SIMPLE_SMTP_CAPABILITY("8BITMIME", 8, r, EIGHT_BITMIME);
		ADD_SIMPLE_SMTP_CAPABILITY("PIPELINING", 10, r, PIPELINING);
		ADD_SIMPLE_SMTP_CAPABILITY("PRDR", 4, r, PRDR);
		ADD_SIMPLE_SMTP_CAPABILITY("CHUNKING", 8, r, CHUNKING);
		ADD_SIMPLE_SMTP_CAPABILITY("ENHANCEDSTATUSCODES", 19, r,
					   ENHANCEDSTATUSCODES);
		ADD_SIMPLE_SMTP_CAPABILITY("STARTTLS", 8, r, SMTPTLS);
		ADD_SIMPLE_SMTP_CAPABILITY("SMTPUTF8", 8, r, SMTPUTF8);

		/* Go to the next line */
		skip_cl_rl(r);
	}

	return smtp_caps;
}

void *send_email(int socket, message_t *message, bitmap_t opts)
{
	int n;
	char request[1024];
	char response[1024];
	char *mail_from_msg = "MAIL FROM:kuleshovmail@gmail.com\r\n";
	char *rcpt_to_msg = "RCPT TO:kuleshovmail@gmail.com\r\n";
	char *host = hostname();

	assert(socket != -1);

	if (!host)
	{
		fprintf(stderr, "Error: can't get local hostname\n");
		return NULL;
	}

	/* clear buffers */
	memset(request, 0, 1024);
	memset(response, 0, 1024);

	/* build ehlo message */
	strcat(request, "EHLO ");
	strcat(request, host);
	strcat(request, "\r\n");

	UNUSED(message);

	/* reading greetings from the server */
	if ((n = recv(socket, response, sizeof(response), 0)) == -1)
	{
		fprintf(stderr, "Error: something going wrong. SMTP server "
				"didn't return response\n");
		return NULL;
	}

	/* exit early if something going wrong */
	if (!(response[0] == '2' && response[1] == '2' && response[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP server greetings error\n");
		return NULL;
	}

	/* clear response buffer */
	memset(response, 0, 1024);

	/* send EHLO message */
	send(socket, request, strlen(request), 0);

	/* read SMTP capabilities */
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr, "Error: Can\'t read SMTP EHLO response\n");
		return NULL;
	}

	/* check SMTP code */
	if (!(response[0] == '2' && response[1] == '5' && response[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP EHLO wrong response: %s\n",
			response);
		return NULL;
	}

	if (opts & STOP_AFTER_CAPS)
	{
		free(host);
		return strdup(response);
	}

	/* everything is ok, let's parse SMTP server capabilities */
	if (parse_smtp_caps(response))
	{
		;
	}
	memset(response, 0, 1024);

	/* Send MAIL FROM:.. */
	send(socket, mail_from_msg, strlen(mail_from_msg), 0);

	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for MAIL FROM command\n");
		return NULL;
	}

	if (!(response[0] == '2' && response[1] == '5' && response[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP MAIL FROM wrong response: %s\n",
			response);
		return NULL;
	}
	memset(response, 0, 1024);

	/* Send RCPT TO:.. */
	send(socket, rcpt_to_msg, strlen(rcpt_to_msg), 0);
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for RCPT TO command\n");
		return NULL;
	}
	if (!(response[0] == '2' && response[1] == '5' && response[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP RCPT TO wrong response: %s\n",
			response);
		return NULL;
	}
	memset(response, 0, 1024);

	/* send data */
	send(socket, "DATA\r\n", 6, 0);
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response for DATA command\n");
		return NULL;
	}
	if (!(response[0] == '3' && response[1] == '5' && response[2] == '4'))
	{
		fprintf(stderr, "Error: SMTP DATA wrong response: %s\n",
			response);
		return NULL;
	}
	memset(response, 0, 1024);

	/* send message body */
	send(socket, "test message 2\r\n.\r\n", 19, 0);
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response from message BODY\n");
		return NULL;
	}
	if (!(response[0] == '2' && response[1] == '5' && response[2] == '0'))
	{
		fprintf(stderr, "Error: wrong response for message body: %s\n",
			response);
		return NULL;
	}
	memset(response, 0, 1024);

	/* send quit */
	send(socket, "QUIT\r\n", 6, 0);
	if ((n = recv(socket, response, sizeof(response), 0) == -1))
	{
		fprintf(stderr, "Can't get response for SMTP QUIT command\n");
		return NULL;
	}
	if (!(response[0] == '2' && response[1] == '2' && response[2] == '1'))
	{
		fprintf(stderr, "Error: Your message should be sent, but an "
				"error is gotten from SMTP server on QUIT: "
				"%s\n",
			response);
		return NULL;
	}
	memset(response, 0, 1024);

	free(host);

	return NULL;
}
