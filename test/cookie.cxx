/*
 * cookie.cxx
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

#include <cgixx/cgi.h>
#include <cgixx/header.h>
#include <cgixx/cookie.h>
#include <iostream>
#include <stdexcept>

void test();

int main(int argc, char* argv[])
{
	try {
		test();
	} catch(const std::exception& e) {
		std::cerr << "EXCEPTION: " << e.what() << std::endl;
	} catch(...) {
		std::cerr << "UNKNOWN EXCEPTION" << std::endl;
	}

	return 0;
}


void test()
{
	cgixx::cgi cgi;
	cgixx::header header;
	cgixx::cookie a(cgi, "a", "1"), b(cgi, "b", "2");
	header.setexpire("-1D");
	header.addcookie(a);
	header.addcookie(b);
	std::cout << header.get();
	std::string val;

	std::cout << "<html>\n<head><title>test</title></head>\n<body>\n";
	std::cout << "<h1>cookie test</h1>\n";

	cgixx::cgi::identifierlist idlist;
	cgi.getcookielist(idlist);
	cgixx::cgi::identifierlist::iterator it(idlist.begin()),
		end(idlist.end());
	for (; it != end; ++it)
	{
		while (!cgi.getcookie(*it, val))
			std::cout << *it << ": " << val << "<br>\n";
	}

	cgi.getvariablelist(idlist);
	it = idlist.begin();
	end = idlist.end();
	for (; it != end; ++it)
	{
		while (!cgi.get(*it, val))
			std::cout << *it << ": " << val << "<br>\n";
	}

	std::cout << "</body>\n</html>\n";
	std::cout.flush();
}

