/* mpdscribble (MPD Client)
 * Copyright (C) 2008-2019 The Music Player Daemon Project
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

#ifndef CONFIG_HXX
#define CONFIG_HXX

#include "ScrobblerConfig.hxx"

#include <forward_list>
#include <string>

enum file_location { file_etc, file_home, file_unknown, };

static inline const char *
NullableString(const std::string &s) noexcept
{
	return s.empty() ? nullptr : s.c_str();
}

struct Config {
	/** don't daemonize the mpdscribble process */
	bool no_daemon = false;

	std::string pidfile;

	std::string daemon_user;

	std::string log;
	std::string conf;
	std::string host;
	std::string proxy;

	unsigned port = 0;

	/**
	 * The interval in seconds after which the journal is saved to
	 * the file system.
	 */
	unsigned journal_interval = 600;

	int verbose = -1;
	enum file_location loc = file_unknown;

	std::forward_list<ScrobblerConfig> scrobblers;
};

#endif
