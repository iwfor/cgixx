/*
 * cgi_impl.h
 *
 * $Id$
 *
 */

/*
 * Copyright (C) 2002-2003 Isaac W. Foraker (isaac@tazthecat.net)
 * All Rights Reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name of the Author nor the names of its contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifdef _MSC_VER
#	pragma warning(disable:4503)
#endif

#include "cgi_impl.h"
#include <iostream>
#include <memory>
#include <cstdlib>
#include <cstring>
#include <cctype>

namespace cgixx {

cgi_impl::cgi_impl()
{
	std::string temp;
	getenvvar(temp, "REQUEST_METHOD", "GET");
	if (temp== "GET")
		method = method_get;
	else if (temp == "POST")
		method = method_post;
	else if (temp == "HEAD")
		method = method_head;
	else if (temp == "PUT")
		method = method_put;
	else
		method = method_get;

	getenvvar(temp, "CONTENT_LENGTH");
	unsigned long clength = std::atoi(temp.c_str());

	if (method == method_post)
	{
		// Parse STDIN
		if (clength)
		{
			char c;
			temp.erase();
			while (clength > 0)
			{
				std::cin >> c;
				temp+= c;
				--clength;
			}
			parseparams(temp);
		}
		// else no parameters
	}
	else	// GET, HEAD, PUT
	{
		// Parse QUERY_STRING
		getenvvar(temp, "QUERY_STRING");
		parseparams(temp);
	}

	getenvvar(temp, "HTTP_COOKIE");
	parsecookies(temp);
}

/*
 * Helper function that gets an environment variable into a string, or
 * sets the default value if available.
 *
 */
void cgi_impl::getenvvar(std::string& dest, const char* name, const char* defval)
{
	const char* t;
	if ( (t = std::getenv(name)) )
		dest = t;
	else if (defval)
		dest = defval;
	else
		dest.erase();
}


/*
 * Parse cookies from HTTP_COOKIE environment variable.
 * Format: id=val; id=val; id=val
 *
 */
void cgi_impl::parsecookies(const std::string& cookielist)
{
	std::string id, val;
	std::size_t pos = 0, newpos, len = cookielist.length();
	while ((pos < len) &&
		((newpos = cookielist.find('=', pos)) != std::string::npos))
	{
		id = cgi2text(cookielist.substr(pos, newpos-pos));
		pos = newpos + 1;	// skip '='
		newpos = cookielist.find(';', pos);
		if (newpos == std::string::npos)
		{
			val = cgi2text(cookielist.substr(pos));
		}
		else
		{
			val = cgi2text(cookielist.substr(pos, newpos-pos));
			// Skip whitespace
			++newpos;
			while (std::isspace(cookielist[newpos]))
				++newpos;
			pos = newpos;	// skip ':'
		}
		cookies[id].push(val);
	}
}

void cgi_impl::parseparams(const std::string& paramlist)
{
	if (paramlist.empty())
		return;
	if (paramlist.find('=') == std::string::npos)
	{
		// ISINDEX
		vars["query_string"].push(cgi2text(paramlist));
	}
	else
	{
		// identifiers are delimited by = and values are delimited by
		// & or \0.
		std::string id, val;
		std::size_t pos = 0, newpos, len = paramlist.length();
		while ((pos < len) &&
			((newpos = paramlist.find('=', pos)) != std::string::npos))
		{
			id = cgi2text(paramlist.substr(pos, newpos-pos));
			pos = newpos + 1;	// skip '='
			newpos = paramlist.find('&', pos);
			if (newpos == std::string::npos)
			{
				val = cgi2text(paramlist.substr(pos));
			}
			else
			{
				val = cgi2text(paramlist.substr(pos, newpos-pos));
				pos = newpos + 1;	// skip '&'
			}
			vars[id].push(val);
		}
	}
}

std::string cgi2text(const std::string& cgistr)
{
	std::string textstr;
	std::string::const_iterator it(cgistr.begin()), end(cgistr.end());

	for (; it != end; ++it)
	{
		if (*it == '%')
		{
			++it;
			if (it == end)
				break;
			unsigned char temp = hex2dec(*it) * 16;
			++it;
			if (it == end)
				break;
			temp+= hex2dec(*it);
			textstr+= temp;
		}
		else if (*it == '+')
			textstr+= ' ';
		else
			textstr+= *it;
	}

	return textstr;
}

std::string text2cgi(const std::string& textstr)
{
	std::string cgistr;
	std::string::const_iterator it(textstr.begin()), end(textstr.end());

	for (; it != end; ++it)
	{
		if (std::isalnum(*it))
			cgistr+= *it;
		else
		{
			cgistr+= '%';
			cgistr+= dec2hex(*it / 16);
			cgistr+= dec2hex(*it % 16);
		}
	}
	return cgistr;
}

unsigned char dec2hex(char c)
{
	if (c < 0)
		return 0;
	else if (c < 10)
		return c + '0';
	else if (c < 16)
		return c - 10 + 'A';
	else
		return '0';
}

unsigned char hex2dec(char c)
{
	switch (c) {
	case 'A':
	case 'B':
	case 'C':
	case 'D':
	case 'E':
	case 'F':
		return c - 'A' + 10;
	case 'a':
	case 'b':
	case 'c':
	case 'd':
	case 'e':
	case 'f':
		return c - 'a' + 10;
	default:
		return c - '0';
	}
}

} // end namespace cgixx
