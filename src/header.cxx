/*
 * header.cxx
 *
 * $Id$
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

	header_impl() : httpver("HTTP/1.0"), content_length(0),
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
		// Add status (e.g. "HTTP/1.0 302 Redirect")
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
		hdr+= "Expire: ";
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

void header::setlength(unsigned length)
{
	imp->content_length = length;
}

// Content-type
void header::settype(const std::string& contenttype)
{
	imp->content_type = contenttype;
}

void header::override(const std::string& httpversion)
{
	imp->httpver = httpversion;
}

void header::setheader(const std::string& id, const std::string& value)
{
	std::string newhead(id);
	newhead+= ": ";
	newhead+= value;
	imp->extra_headers.push_back(newhead);
}

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
