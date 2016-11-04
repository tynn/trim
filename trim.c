/*
 *	This file is part of trim.
 *
 *	Copyright (c) 2014 Christian Schmitz <tynn.dev@gmail.com>
 *
 *	trim is free software: you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation, either version 3 of the License, or
 *	(at your option) any later version.
 *
 *	trim is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with trim. If not, see <http://www.gnu.org/licenses/>.
 */

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static int help (const char *name) {
	printf("Usage: %s [--help] [FILE]...\n", name);
	return 0;
}

static void trimlines (char **lineptr, size_t *n, const char *file) {
	ssize_t i, len;
	FILE *stream;
	int errno2 = errno;

	errno = 0;

	if (file[0] == '-' && file[1] == '\0')
		stream = stdin;
	else
		stream = fopen(file, "r");

	if (stream != NULL) {
		while ((len = getline (lineptr, n, stream)) >= 0) {
			for (;len > 0 && isspace((*lineptr)[len-1]);len--);
			(*lineptr)[len] = '\0';
			for (i = 0; i < len && isspace((*lineptr)[i]); i++);
			puts((*lineptr) + i);
		}

		if (stream == stdin)
			file = "stdin";
		else
			fclose(stream);
	}

	if (errno > 0)
		perror(file);
	else
		errno = errno2;
}

int main (int argc, char **argv) {
	int i;
	char *line = NULL;
	size_t n = 0;

	if (argc == 1) {
		trimlines(&line, &n, "-");
	} else {
		for (i = 1; i < argc; i++)
			if (strcmp("--help", argv[i]) == 0)
				return help(argv[0]);

		for (i = 1; i < argc; i++)
			trimlines(&line, &n, argv[i]);
	}

	if (line != NULL)
		free(line);

	return errno > 0;
}

