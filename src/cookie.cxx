/*
 * cookie.cxx
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

#include "cgi_impl.h"
#include "timedefs.inl"
#include <cgixx/cookie.h>
#include <cgixx/cgi.h>
#include <cstdio>
#include <cctype>
#include <ctime>

namespace cgixx {

// expire=Wdy, DD-Mon-YYYY HH:MM:SS GMT

struct cookie_impl {
	std::string name;
	std::string value;
	std::string expire;
	std::string path;
	std::string domain;
	bool secure;
};


/**
 * Construct a cookie on a cgi session.  The cgi session will be used
 * to prepopulate domain and path.
 *
 * @param	initcgi		Reference to cgi instance.
 * @param	name		Name of this cookie.
 * @param	value		Value of this cookie.
 *
 */
cookie::cookie(const cgi& initcgi, const std::string& name,
			   const std::string& value)
	: imp(new cookie_impl)
{
	imp->name = name;
	imp->value = value;
	initcgi.getheader(header_server_name, imp->domain);
	initcgi.getheader(header_script_name, imp->path);
	imp->secure = false;
}


/**
 * Construct a basic cookie.
 *
 * @param	name		Name of this cookie.
 * @param	value		Value of this cookie.
 *
 */
cookie::cookie(const std::string& name, const std::string& value)
	: imp(new cookie_impl)
{
	imp->name = name;
	imp->value = value;
	imp->secure = false;
}


/**
 * Construct a copy of another cookie.
 *
 * @param	copy	Reference to cookie to copy.
 *
 */
cookie::cookie(const cookie& copy)
	: imp(new cookie_impl)
{
	imp->name = copy.imp->name;
	imp->value = copy.imp->value;
	imp->expire = copy.imp->expire;
	imp->path = copy.imp->path;
	imp->domain = copy.imp->domain;
	imp->secure = copy.imp->secure;
}


/**
 * Destroy *this instance of cookie.
 *
 */
cookie::~cookie()
{
	delete imp;
}


/**
 * Set the value of this cookie.
 *
 * @param	value		Value for this cookie.
 * @return	nothing
 *
 */
void cookie::setvalue(const std::string& value)
{
	imp->value = value;
}


/**
 * The the domain of this cookie.
 *
 * @param	domain		Domain for this cookie.
 * @return	nothing
 *
 */
void cookie::setdomain(const std::string& domain)
{
	imp->domain = domain;
}


/**
 * Set the path of this cookie.
 *
 * @param	path		Path for this cookie.
 * @return	nothing
 *
 */
void cookie::setpath(const std::string& path)
{
	imp->path = path;
}


/**
 * Specify whether this cookie requires an ssl connection in order for
 * the client to transmit this cookie.
 *
 * @param	requiressl	When true, require ssl.
 * @return	nothing
 *
 */
void cookie::setsecure(bool requiressl)
{
	imp->secure = requiressl;
}


/**
 * Set the expiration date and time for this cookie.
 *
 * @param	expire		The expire parameter my be either the RFC 850
 *						encoded date of expiration, or an offset string
 *						containing "(-)123(M|H|S|D|W|m|Y)".  I.e. a
 *						number (negative numbers generally cause the
 *						document to expire immediately) followed by a
 *						time modifier.  M=minute, H=hour, S=second,
 *						D=day, W=week, m=month, Y=year.
 * @return	false if the offset was successfully parsed into a date;
 * @return	true if the the expire string was accepted as is.
 *
 */
bool cookie::setexpire(const std::string& expire)
{
	unsigned pos = 0;

	if (expire[pos] == '-')
		++pos;
	if (std::isdigit(expire[pos]))
	{
		while (std::isdigit(expire[++pos]))
			;
		unsigned duration = std::atoi(expire.substr(0, pos).c_str());
		switch (expire[pos])
		{
		case 'M':	// minute
			duration*= 60;
			break;
		case 'H':
			duration*= 3600;
			break;
		case 'D':
		case 'd':
			duration*= 84600;
			break;
		case 'W':
		case 'w':
			duration*= 84600*7;
			break;
		case 'm':
			duration*= 84600*30;
			break;
		case 'Y':
		case 'y':
			duration*= 84600*365;
			break;
		case 'S':
		case 's':
			break;
		default:
			imp->expire = expire;
			return true;
		}
		std::time_t timer = std::time(NULL) + duration;
		struct std::tm* ts = std::gmtime(&timer);
		char buf[72];
		std::sprintf(buf, "%s, %02u-%s-%04u %02u:%02u:%02u GMT",
			weekday[ts->tm_wday], ts->tm_mday, month[ts->tm_mon],
			ts->tm_year + 1900, ts->tm_hour, ts->tm_min, ts->tm_sec);
		imp->expire = buf;
		return false;
	}

	imp->expire = expire;
	return true;
}


/**
 * Get the name of this cookie.
 *
 * @param	none
 * @return	The name of this cookie.
 *
 */
const std::string& cookie::getname() const
{
	return imp->name;
}


/**
 * Get the value of this cookie.
 *
 * @param	none
 * @return	The value of this cookie.
 *
 */
const std::string& cookie::getvalue() const
{
	return imp->value;
}

/**
 * Get the formatted cookie header of the form:
 *
 * Set-Cookie: NAME=VALUE; expire=DATE; path=PATH; domain=DOMAIN_NAME; secure
 *
 * @param	none
 * @return	The formatted cookie header.
 *
 */
std::string cookie::get()
{
	std::string setmsg("Set-Cookie: ");
	setmsg+= text2cgi(imp->name);
	setmsg+= '=';
	setmsg+= text2cgi(imp->value);
	if (!imp->expire.empty())
	{
		setmsg+= "; expires=";
		setmsg+= imp->expire;
	}
	if (!imp->path.empty())
	{
		setmsg+= "; path=";
		setmsg+= imp->path;
	}
	if (!imp->domain.empty())
	{
		setmsg+= "; domain=";
		setmsg+= imp->domain;
	}
	if (imp->secure)
	{
		setmsg+= "; secure";
	}
	return setmsg;
}


} // end namespace cgixx
