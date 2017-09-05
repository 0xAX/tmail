/*
 * mime.h - MIME types utils.
 *
 * Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
 *
 * This file is released under the BSD license, see the COPYING file
 */

#include "mime.h"

char *mime_file_content = NULL;

/*
 * load_mime_file - loads `mime.types` file
 * at startup.
 *
 * NOTE: it should be used at the start of an
 * application, because it will exit if something
 * going wrong.
 */
void load_mime_file(const char *filepath)
{
	int n = 0;
	struct stat st;
	char buffer[4096];
	fd_t mime_file = open(filepath, O_RDONLY);

	if (mime_file == -1)
	{
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	if (stat(filepath, &st) != 0)
	{
		close(mime_file);
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}

	mime_file_content = malloc(st.st_size + 1);
	if (!mime_file_content)
	{
		close(mime_file);
		fprintf(stderr, "%s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	memset(mime_file_content, 0, st.st_size);

	while ((n = read(mime_file, buffer, 4096)) > 0)
	{
		strncat(mime_file_content, buffer, n);
		memset(buffer, 0, 4096);
	}

	close(mime_file);
}

/*
 * get_mime_type - tries to determine a MIME type by
 * the given @filename.
 *
 * Returns allocated `char*`. In failure case
 * returns NULL.
 */
char *get_mime_type(char *filename)
{
	char *extension = strrchr(filename, '.');

	if (!mime_file_content)
	{
		fprintf(stderr,
			"Error: something going wrong. No mime.types file\n");
		return NULL;
	}

	/* if a file does not have extenssion, return plain/text */
	if (!extension || extension + 1 == 0)
		return strdup(DEFAULT_MIME_TYPE);

	/* skip '.' */
	extension = strdup(extension);
	if (!extension)
	{
		fprintf(
		    stderr,
		    "Error: Can't allocate memory for mime file extension\n");
		free(extension);
		return NULL;
	}
	/* all extension have a space before in mime.types */
	strncpy(extension, " ", 1);

	while (true)
	{
		int i = 0;
		char buffer[50];
		char *ptr = strstr(mime_file_content, extension);

		if (!ptr)
		{
			free(extension);
			return strdup(DEFAULT_MIME_TYPE);
		}

		memset(buffer, 0, 50);

		/*
		 * ok we've found line with our mime type,
		 * go back to the start of line.
		 */
		while (*ptr != '\n')
		{
			ptr--;
		}
		ptr++;

		while (!isspace(*ptr))
		{
			buffer[i] = *ptr;
			ptr++;
			i++;
		}

		free(extension);
		return strdup(buffer);
	}
}

/*
 * mime_free - releases memory under global mime_file_content.
 * The preferred way to call it in `at_exit()` callback.
 */
void mime_free(void)
{
	if (mime_file_content)
		mfree(mime_file_content);
}
