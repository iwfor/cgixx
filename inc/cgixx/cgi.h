/*
 * cgi.h
 *
 * $Id$
 *
 */

/*
 * Copyright (C) 2002-2004 Isaac W. Foraker (isaac@tazthecat.net)
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

#ifndef __cgixx_cgi_h
#define __cgixx_cgi_h

#include <string>
#include <vector>
#include <stdexcept>

namespace cgixx {

/**
 * CGI Exception
 */
class cgiexception : public std::runtime_error {
	public:
        /// Wrap the standard exception with a string explanation
		cgiexception(const std::string& what_arg) :
			std::runtime_error(what_arg) {}
        /// Wrap the standard exception
		cgiexception() :
			std::runtime_error(std::string()) {}
};

/**
 * The methods enumeration lists all CGI methods supported by cgixx.
 */
enum methods {
	method_get = 0,
	method_post,
	method_head,
	method_put
};

/**
 * The headers enumeration defines headers that can be returned with
 * a call to getheader().
 */
enum headers {
	header_request_method,
	header_query_string,
	header_server_software,
	header_server_name,
	header_gateway_interface,
	header_server_protocol,
	header_server_port,
	header_path_info,
	header_path_translated,
	header_script_name,
	header_remote_addr,
	header_remote_host,
	header_auth_type,
	header_remote_user,
	header_remote_ident,
	header_content_type,
	header_content_length,
	header_http_accept,
	header_http_user_agent,
	header_http_cookie
};

/// Forward declaration, for intenal use
struct cgi_impl;


/**
 * The cgi class is used to process CGI requests.  cgi will process all
 * server set environment variables and the standard input when
 * applicable to extract CGI variables and cookies, and convert values
 * to text.
 *
 * @author	Isaac W. Foraker
 *
 */
class cgi {
public:
	cgi();
	~cgi();

	typedef std::vector< std::string > identifierlist;

	/// Get the cgixx library version string.
	const std::string& libver();

	/// Get count of a variable.
	unsigned count(const std::string& id) const;

	/// Check if a variable exists.
	bool exists(const std::string& id);

	/// Get next available value of a variable.
	bool get(const std::string& id, std::string& value);

	/// Get list of variable identifiers.
	void getvariablelist(identifierlist& idlist) const;

	/// Get count of a cookie.
	unsigned countcookie(const std::string& id) const;

	/// Check if a cookie exists.
	bool cookieexists(const std::string& id);

	/// Get next available value of a cookie.
	bool getcookie(const std::string& id, std::string& value);

	/// Get list of cookie identifiers.
	void getcookielist(identifierlist& idlist) const;

	/// Get the specified header.
	bool getheader(headers hid, std::string& copy) const;

	/// Get the request method.
	methods getmethod() const;

private:
	// There is no copy constructor.
	cgi(const cgi&);
	// There is not copy operator.
	cgi& operator=(const cgi&);

	cgi_impl* imp;
};

// Other useful functions
std::string& makesafestring(const std::string& instr, std::string& outstr);

} // end namespace cgixx

#endif // __cgixx_cgi_h
