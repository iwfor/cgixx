/*
 * cookie.cxx
 *
 * $Id$
 *
 */

#include <cgixx/cookie.h>
#include <cgixx/cgi.h>

namespace cgixx {

//Set-Cookie: NAME=VALUE; expires=DATE; path=PATH; domain=DOMAIN_NAME; secure

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


} // end namespace cgixx
