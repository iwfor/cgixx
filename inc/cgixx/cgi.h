/*
 * cgi.h
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

#ifndef __cgixx_cgi_h
#define __cgixx_cgi_h

#include <string>

namespace cgixx {

enum methods {
	method_get = 0,
	method_post,
	method_head,
	method_put
};

// Forward declaration
struct cgi_impl;
class cookie;

/**
 * The cgi class is used to process CGI requests.  cgi will process all
 * server set environment variables and the standard input when
 * applicable to extract CGI variables and cookies.
 *
 * @author	Isaac W. Foraker
 *
 */
class cgi {
public:
	cgi();
	~cgi();

	/// Get the cgixx library version string.
	const std::string& libver();

	/// Get CGI method requested.
	methods method() const;

	/// Get count of a variable.
	unsigned count(const std::string& id) const;

	/// Get next available value of a variable.
	bool get(const std::string& id, std::string& value);

	/// Get count of a cookie.
	unsigned countcookie(const std::string& id) const;

	/// Get next available value of a cookie.
	bool getcookie(const std::string& id, std::string& value);

	/// Get web server name.
	const std::string& server() const;

	/// Get web server port.
	unsigned serverport() const;

	/// Get web server protocol.
	const std::string& serverprotocol() const;

	/// Get the CGI version supported by the web server.
	const std::string& gatewayinterface() const;

	/// Get the name of the server software.
	const std::string& serversoftware() const;

	/// Get extra path information.
	const std::string& pathinfo() const;

	/// Get CGI script real path.
	const std::string& realpath() const;

	/// Get virtual path to this script.
	const std::string& script() const;

	/// Get the remote host.
	const std::string& remotehost() const;

	/// Get the remote address.
	const std::string& remoteaddr() const;

	/// Get the authorization type.
	const std::string& authtype() const;

	/// Get the username.
	const std::string& remoteusername() const;

	/// Get the remote identity.
	const std::string& remoteidentity() const;

	/// Get the content type.
	const std::string& contenttype() const;

	/// Get the content length.
	unsigned long contentlength() const;

	/// Get the mime types accepted by the client.
	const std::string& httpaccept() const;

	/// Get user agent (i.e. web browser software)
	const std::string& agent() const;


private:
	// There is no copy constructor.
	cgi(const cgi&);
	// There is not copy operator.
	cgi& operator=(const cgi&);

	cgi_impl* imp;
};

//extern char **environ;

} // end namespace cgixx

#endif // __cgixx_cgi_h
