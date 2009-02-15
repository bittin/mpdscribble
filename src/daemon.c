/* mpdscribble (MPD Client)
 * Copyright (C) 2008-2009 The Music Player Daemon Project
 * Copyright (C) 2005-2008 Kuno Woudt <kuno@frob.nl>
 * Project homepage: http://musicpd.org
 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

#include "daemon.h"

#include <glib.h>

#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>

/** the absolute path of the pidfile */
static char *pidfile;

void
daemonize_close_stdin(void)
{
	int fd = open("/dev/null", O_RDONLY);

	if (fd < 0)
		close(STDIN_FILENO);
	else if (fd != STDIN_FILENO) {
		dup2(fd, STDIN_FILENO);
		close(fd);
	}
}

void
daemonize_close_stdout_stderr(void)
{
	int fd = open("/dev/null", O_WRONLY);

	if (fd >= 0) {
		if (fd != STDOUT_FILENO)
			dup2(fd, STDOUT_FILENO);
		if (fd != STDERR_FILENO)
			dup2(fd, STDERR_FILENO);
		if (fd != STDOUT_FILENO && fd != STDERR_FILENO)
			close(fd);
	} else {
		close(STDOUT_FILENO);
		close(STDERR_FILENO);
	}
}

void
daemonize_detach(void)
{
	int ret;

	/* detach from parent process */

	ret = fork();
	if (ret < 0)
		g_error("fork() failed: %s", g_strerror(errno));

	if (ret > 0)
		/* exit the parent process */
		_exit(EXIT_SUCCESS);

	/* release the current working directory */

	ret = chdir("/");
	if (ret < 0)
		g_error("chdir() failed: %s\n", g_strerror(errno));

	/* detach from the current session */

	setsid();
}

void
daemonize_write_pidfile(void)
{
	FILE *file;

	if (pidfile == NULL)
		return;

	unlink(pidfile);

	file = fopen(pidfile, "w");
	if (file == NULL)
		g_error("Failed to create pidfile %s: %s",
			pidfile, g_strerror(errno));

	fprintf(file, "%d\n", getpid());
	fclose(file);
}

void
daemonize_init(const char *_pidfile)
{
	pidfile = g_strdup(_pidfile);
}

void
daemonize_finish(void)
{
	if (pidfile != NULL)
		unlink(pidfile);

	g_free(pidfile);
}
