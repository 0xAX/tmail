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

static int build_ehlo_msg(char *buffer)
{
	char *host = hostname();

	if (!host)
	{
		fprintf(stderr, "Error: can't get local hostname\n");
		return 0;
	}

	memset(buffer, 0, 1024);

	strcat(buffer, "EHLO ");
	strcat(buffer, host);
	strcat(buffer, "\r\n");

	free(host);
	return 1;
}

static int read_smtp_greetings(int socket, char *buffer)
{
	int n = 0;

	if ((n = recv(socket, buffer, 1024, 0)) == -1)
	{
		fprintf(stderr, "Error: something going wrong. SMTP server "
				"didn't return response\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '2' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP server greetings error\n");
		return 0;
	}

	/* clear response buffer */
	memset(buffer, 0, 1024);

	return 1;
}

static int send_ehlo_message(int socket, char *request, char *buffer)
{
	int n = 0;

	send(socket, request, strlen(request), 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr, "Error: Can\'t read SMTP EHLO response\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP EHLO wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}

static int send_mail_from_message(int socket, char *buffer)
{
	int n = 0;
	char *mail_from_msg = "MAIL FROM:kuleshovmail@gmail.com\r\n";

	send(socket, mail_from_msg, strlen(mail_from_msg), 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for MAIL FROM command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP MAIL FROM wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}

static int send_rcpt_to_message(int socket, char *buffer)
{
	int n = 0;
	char *rcpt_to_msg = "RCPT TO:kuleshovmail@gmail.com\r\n";

	send(socket, rcpt_to_msg, strlen(rcpt_to_msg), 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can't get response for RCPT TO command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: SMTP RCPT TO wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}

static int send_data_message(int socket, char *buffer)
{
	int n = 0;

	send(socket, "DATA\r\n", 6, 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response for DATA command\n");
		return 0;
	}

	if (!(buffer[0] == '3' && buffer[1] == '5' && buffer[2] == '4'))
	{
		fprintf(stderr, "Error: SMTP DATA wrong response: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}

static int send_message_body(int socket, char *buffer)
{
	int n = 0;

	send(socket, "test message 1\r\n.\r\n", 19, 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr,
			"Error: Can\'t get response from message BODY\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '5' && buffer[2] == '0'))
	{
		fprintf(stderr, "Error: wrong response for message body: %s\n",
			buffer);
		return 0;
	}

	memset(buffer, 0, 1024);

	return 1;
}

static int send_quit_message(int socket, char *buffer)
{
	int n = 0;

	send(socket, "QUIT\r\n", 6, 0);

	if ((n = recv(socket, buffer, 1024, 0) == -1))
	{
		fprintf(stderr, "Can't get response for SMTP QUIT command\n");
		return 0;
	}

	if (!(buffer[0] == '2' && buffer[1] == '2' && buffer[2] == '1'))
	{
		fprintf(stderr, "Error: Your message should be sent, but an "
				"error is gotten from SMTP server on QUIT: "
				"%s\n",
			buffer);
		return 0;
	}

	return 1;
}

void *send_email(int socket, message_t *message, bitmap_t opts)
{
	char request[1024];
	char response[1024];

	assert(socket != -1);

	UNUSED(message);

	memset(response, 0, 1024);

	if (!build_ehlo_msg(request))
		return NULL;
	if (!read_smtp_greetings(socket, response))
		return NULL;
	if (!send_ehlo_message(socket, request, response))
		return NULL;

	if (opts & STOP_AFTER_CAPS)
		return strdup(response);

	/* everything is ok, let's parse SMTP server capabilities */
	if (parse_smtp_caps(response))
	{
		;
	}
	memset(response, 0, 1024);

	if (!send_mail_from_message(socket, response))
		return NULL;
	if (!send_rcpt_to_message(socket, response))
		return NULL;
	if (!send_data_message(socket, response))
		return NULL;
	if (!send_message_body(socket, response))
		return NULL;
	if (!send_quit_message(socket, response))
		return NULL;

	return (void *)1;
}
