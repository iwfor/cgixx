/*
 * header.cxx
 *
 * $Id$
 *
 */

/*
 * Copyright (C) 2002 Isaac W. Foraker (isaac@tazthecat.net)
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

#include "timedefs.inl"
#include <cgixx/header.h>
#include <cgixx/cookie.h>
#include <vector>
#include <cstdio>
#include <cctype>
#include <ctime>

namespace cgixx {

struct header_impl
{
	std::string httpver;
	std::string status;
	unsigned content_length;
	std::string content_type;
	std::string expire;
	std::string location;

	std::vector< std::string > extra_headers;

	header_impl() : httpver("HTTP/1.1"), content_length(0),
		content_type("text/html") {}
};

/**
 * Construct a header object.
 *
 */
header::header()
{
	imp = new header_impl;
}

/**
 * Destroy *this header object.
 *
 */
header::~header()
{
	delete imp;
}


/**
 * Get the formatted CGI HTTP header to send to the client.  The
 * default header will be
 *
 * 400 OK
 * Content-type: text/html
 *
 * unless modified by other header methods.  The web server may modify
 * or expand on these headers unless a specific status code has been
 * specified (e.g. through redirect method).
 *
 * @param	none
 * @return	The header string.
 *
 */
std::string header::get() const
{
	std::string hdr;

	if (!imp->status.empty())
	{
		// Add status (e.g. "HTTP/1.1 302 Redirect")
		hdr = imp->httpver;
		hdr+= ' ';
		hdr+= imp->status;
		hdr+= "\r\n";
	}

	if (!imp->location.empty())
	{
//		hdr+= "URI: ";
//		hdr+= imp->location;
//		hdr+= "; vary=*\r\n";

		hdr+= "Location: ";
		hdr+= imp->location;
		hdr+= "\r\n";
	}

	if (!imp->content_type.empty())
	{
		// Build content type (e.g. "Content-type: text/html")
		hdr+= "Content-type: ";
		hdr+= imp->content_type;
		hdr+= "\r\n";
	}

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
		hdr+= "Expires: ";
		hdr+= imp->expire;
		hdr+= "\r\n";
	}

	std::vector< std::string >::const_iterator it(imp->extra_headers.begin()),
		end(imp->extra_headers.end());
	for (; it != end; ++it)
	{
		hdr+= *it;
		hdr+= "\r\n";
	}

	hdr+= "\r\n";
	return hdr;
}


/**
 * Set the status code to be returned to the client.  Manually setting
 * the status code will override the web server's ability to alter
 * HTTP headers.
 *
 * @param	status			Desired Status code.
 * @param	description		Optional description for status.  If no
 *							description is specified, a default is
 *							supplied based on the status code.
 * @return	nothing.
 *
 */
void header::setstatus(unsigned status, const std::string& description)
{
	// Bounds check status
	status = status < 200 || status > 599 ? 500 : status;

	// Convert status to a string
	char buf[32];
	std::sprintf(buf, "%u", status);

	imp->status = buf;
	imp->status+= ' ';
	if (!description.empty())
		imp->status+= description;
	else
	{
		switch (status) {
		case 200:
			imp->status+= "OK";
			break;
		case 201:
			imp->status+= "Created";
			break;
		case 202:
			imp->status+= "Accepted";
			break;
		case 203:
			imp->status+= "Partial Information";
			break;
		case 204:
			imp->status+= "No Response";
			break;
		case 301:
			imp->status+= "Moved";
			break;
		case 302:
			imp->status+= "Found";
			break;
		case 303:
			imp->status+= "Method";
			break;
		case 304:
			imp->status+= "Not Modified";
			break;
		case 400:
			imp->status+= "Bad Request";
			break;
		case 401:
			imp->status+= "Unauthorized";
			break;
		case 402:
			imp->status+= "Payment Required";
			break;
		case 403:
			imp->status+= "Forbidden";
			break;
		case 404:
			imp->status+= "Not Found";
			break;
		case 500:
			imp->status+= "Internal Error";
			break;
		case 501:
			imp->status+= "Not Implemented";
			break;
		case 502:
			imp->status+= "Service temporarily overloaded";
			break;
		case 503:
			imp->status+= "Gateway Timeout";
			break;
		default:
			imp->status+= "Unknown";
			break;
		}
	}
}


/**
 * Set the content length to be sent to the client.
 *
 * @param	length		The content length.
 * @return	nothing
 *
 */
void header::setlength(unsigned length)
{
	imp->content_length = length;
}


/**
 * Set the mime type of the content to be sent to the client.  This is
 * only required if content other than HTML is being returned.
 * Format: type/sub-type
 *
 * @param	contenttype		The content mime type (e.g. image/jpg).
 * @return	nothing
 *
 */
void header::settype(const std::string& contenttype)
{
	imp->content_type = contenttype;
}


/**
 * Set the HTTP version string, overriding the web servers processing
 * of the HTTP headers returned by this program.
 *
 * @param	httpversion		The HTTP version string (e.g. HTTP/1.1).
 * @return	nothing
 *
 */
void header::override(const std::string& httpversion)
{
	imp->httpver = httpversion;
}


/**
 * Set a user-defined HTTP header.
 *
 * @param	id		Identifier for header.
 * @param	value	Value of header.
 * @return	nothing
 *
 */
void header::setheader(const std::string& id, const std::string& value)
{
	std::string newhead(id);
	newhead+= ": ";
	newhead+= value;
	imp->extra_headers.push_back(newhead);
}


/**
 * Create a redirect header.
 *
 * @param	location	Path or URL to redirect to.
 * @return	nothing
 *
 */
void header::redirect(const std::string& location)
{
	setstatus(302);
	imp->location = location;
	imp->content_type.clear();
}


/**
 * Add a cookie to the header.
 *
 * @param	value	Reference to completed cookie.
 * @return	nothing
 *
 */
void header::addcookie(cookie& value)
{
	imp->extra_headers.push_back(value.get());
}


/**
 * Set the expiration date for the document.
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
bool header::setexpire(const std::string& expire)
{
	// First, attempt to parse expire.
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

} // end namespace cgixx
