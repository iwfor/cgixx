/*
 * header.h
 *
 * $Id$
 *
 */

/*
 * Copyright (C) 2002-2004 Isaac W. Foraker (isaac at noscience dot net)
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

	/// Create a redirect header
	void redirect(const std::string& location);

	/// Add a cookie object to the header.
	void addcookie(cookie& value);

	/// Get the formatted header string.
	std::string get() const;

private:
	header_impl* imp;
};

} // end namespace cgixx

#endif // __cgixx_header_h
