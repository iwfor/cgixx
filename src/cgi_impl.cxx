/*
 * cgi_impl.h
 *
 * $Id$
 *
 */

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
	if (temp == "GET")
		method = method_get;
	else if (temp == "POST")
		method = method_post;
	else if (temp == "HEAD")
		method = method_head;
	else if (temp == "PUT")
		method = method_put;
	else
		method = method_get;

	getenvvar(query_string, "QUERY_STRING");
	getenvvar(server_software, "SERVER_SOFTWARE");
	getenvvar(server_name, "SERVER_NAME");
	getenvvar(gateway_interface, "GATEWAY_INTERFACE");
	getenvvar(server_protocol, "SERVER_PROTOCOL");
	getenvvar(server_port, "SERVER_PORT");
	getenvvar(path_info, "PATH_INFO");
	getenvvar(path_translated, "PATH_TRANSLATED");
	getenvvar(script_name, "SCRIPT_NAME");
	getenvvar(remote_addr, "REMOTE_ADDR");
	getenvvar(remote_host, "REMOTE_HOST", remote_addr.c_str());
	getenvvar(auth_type, "AUTH_TYPE");
	getenvvar(remote_user, "REMOTE_USER");
	getenvvar(remote_ident, "REMOTE_IDENT");
	getenvvar(content_type, "CONTENT_TYPE");
	getenvvar(temp, "CONTENT_LENGTH");
	content_length = std::atoi(temp.c_str());
	getenvvar(http_accept, "HTTP_ACCEPT");
	getenvvar(http_user_agent, "HTTP_USER_AGENT");

	getenvvar(temp, "HTTP_COOKIE");
	parsecookies(temp);

	if (method == method_post)
	{
		// Parse STDIN
		if (content_length)
		{
			// TODO: Is this legal in the standard?
			temp.resize(content_length);
			std::cin.readsome(&temp[0], content_length);
			parseparams(temp);
		}
	}
	else	// GET, HEAD, PUT
	{
		// Parse query_string
		parseparams(query_string);
	}
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
			pos = newpos;	// skip '&'
		}
		vars[id] = val;
	}
}

void cgi_impl::parseparams(const std::string& paramlist)
{
	if (paramlist.find('=') == std::string::npos)
	{
		// ISINDEX
		vars["query_string"] = cgi2text(query_string);
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
			vars[id] = val;
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
	if (c < 10)
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