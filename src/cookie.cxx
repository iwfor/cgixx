/*
 * cookie.cxx
 *
 * $Id$
 *
 */

#include <cgixx/cookie.h>
#include <cgixx/cgi.h>

namespace cgixx {

// expires=Wdy, DD-Mon-YYYY HH:MM:SS GMT

struct cookie_impl {
	std::string name;
	std::string value;
	std::string expires;
	std::string path;
	std::string domain;
	bool secure;
};

cookie::cookie(const cgi& initcgi, const std::string& name,
			   const std::string& value)
	: imp(new cookie_impl)
{
	imp->name = name;
	imp->value = value;
	imp->domain = initcgi.server();
	imp->path = initcgi.path();
	imp->secure = false;
}

cookie::cookie(const std::string& name, const std::string& value)
	: imp(new cookie_impl)
{
	imp->name = name;
	imp->value = value;
	imp->secure = false;
}

cookie::cookie(const cookie& copy)
	: imp(new cookie_impl)
{
	imp->name = copy.imp->name;
	imp->value = copy.imp->value;
	imp->expires = copy.imp->expires;
	imp->path = copy.imp->path;
	imp->domain = copy.imp->domain;
	imp->secure = copy.imp->secure;
}


cookie::~cookie()
{
	delete imp;
}

void cookie::setvalue(const std::string& value)
{
}

void cookie::setdomain(const std::string& domain)
{
	imp->domain = domain;
}

void cookie::setpath(const std::string& path)
{
	imp->path = path;
}

void cookie::setexpire(const std::string& expires)
{
	imp->expires = expires;
}

//void cookie::setexpire(const std::tm& expires);


void cookie::setsecure(bool requiressl)
{
	imp->secure = requiressl;
}

const std::string& cookie::getname() const
{
	return imp->name;
}

const std::string& cookie::getvalue() const
{
	return imp->value;
}

//Set-Cookie: NAME=VALUE; expires=DATE; path=PATH; domain=DOMAIN_NAME; secure
std::string cookie::get()
{
	std::string setmsg("Set-Cookie: ");
	setmsg+= cgi::text2cgi(imp->name);
	setmsg+= '=';
	setmsg+= cgi::text2cgi(imp->value);
	if (!imp->expires.empty())
	{
		setmsg+= "; expires=";
		setmsg+= imp->expires;
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
