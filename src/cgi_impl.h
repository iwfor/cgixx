/*
 * cgi_impl.h
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

#ifndef __cgixx_cgi_impl_h
#define __cgixx_cgi_impl_h

#include "compat.h"

#include <cgixx/cgi.h>
#include <string>
#include <map>
#include <queue>

namespace cgixx {

typedef std::queue< std::string > strqueue;
typedef std::map< std::string, strqueue > ParameterList;

struct cgi_impl {
	cgi_impl();
	void parseparams(const std::string& paramlist);

	// Store an environment variable in the specified string.
	void getenvvar(std::string& dest, const char* name, const char* defval=0);

	void parsecookies(const std::string&);

	// Map of parameters.
	ParameterList vars;
	ParameterList cookies;

	// The method with which the request was made.
	methods method;
};

std::string cgi2text(const std::string& cgistr);
std::string text2cgi(const std::string& textstr);

unsigned char hex2dec(char c);
unsigned char dec2hex(char c);

} // end namespace cgixx

#endif // __cgixx_cgi_impl_h
