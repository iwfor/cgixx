/*
 * header.cxx
 *
 * $Id$
 *
 */

#include <cgixx/header.h>
#include <cgixx/cookie.h>
#include <vector>
#include <cstdio>

namespace cgixx {

struct header_impl
{
	std::vector< cookie > cookies;
	unsigned content_length;
	std::string content_type;
	std::string expire;

	std::vector< std::string> extra_headers;

	header_impl() : content_length(0), content_type("text/html") {}
};

header::header()
{
	imp = new header_impl;
}

header::~header()
{
	delete imp;
}

std::string header::get()
{
	std::string hdr("Content-type: ");
	hdr+= imp->content_type;
	hdr+= "\r\n";

	if (imp->content_length)
	{
		char buf[32];
		std::sprintf(buf, "%u", imp->content_length);
		hdr+= "Content-length: ";
		hdr+= buf;
		hdr+= "\r\n";
	}

	if (!imp->expire.empty())
	{
		hdr+= "Expire: ";
		hdr+= imp->expire;
		hdr+= "\r\n";
	}

	return hdr;
}

} // end namespace cgixx
