/*
 * header.h
 *
 * $Id$
 *
 */

#ifndef __cgixx_header_h
#define __cgixx_header_h

#include <string>

namespace cgixx {

// Forward declaration
struct header_impl;
class cookie;

/**
 * The header class is used to generate valid HTTP headers to be
 * sent to the web client.  If no modifing methos are accessed, a
 * default header of:
 *
 * 400 OK
 * Content-type: text/html
 *
 * will be generated.  The get method returns the header string.
 *
 * @author	Isaac W. Foraker
 *
 */
class header {
public:
	header();
	~header();

	/// Set Content-length header.
	void setlength(unsigned length);
	
	/// Set Content-type header.
	void settype(const std::string& contenttype);

	/// Set Expire header.
	bool setexpire(const std::string& expire);

	/// Set Status Code.
	void setstatus(unsigned status, const std::string& description = "");

	/// Override the web server's parsing of headers
	void override(const std::string& httpversion = "HTTP/1.0");

	/// Set a generic header.
	void setheader(const std::string& id, const std::string& value);

	/// Add a cookie object to the header.
	void addcookie(cookie& value);

	/// Get the formatted header string.
	std::string get() const;

private:
	header_impl* imp;
};

} // end namespace cgixx

#endif // __cgixx_header_h
