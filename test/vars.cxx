/*
 * vars.cxx
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

struct vars_s {
	const char* desc;
	cgixx::headers hid;
} vars[] = {
	{"Method", cgixx::header_request_method},
	{"Server", cgixx::header_server_name},
	{"Server Port", cgixx::header_server_port},
	{"Server Software", cgixx::header_server_software},
	{"Gateway Interface", cgixx::header_gateway_interface},
	{"Server Protocol", cgixx::header_server_protocol},
	{"Path Info", cgixx::header_path_info},
	{"Path Translated", cgixx::header_path_translated},
	{"Script Name", cgixx::header_script_name},
	{"Remote Address", cgixx::header_remote_addr},
	{"Remote Host", cgixx::header_remote_host},
	{"Authorization Type", cgixx::header_auth_type},
	{"Remote User", cgixx::header_remote_user},
	{"Remote Ident", cgixx::header_remote_ident},
	{"Content Type", cgixx::header_content_type},
	{"Content Length", cgixx::header_content_length},
	{"Accepted Mime Types", cgixx::header_http_accept},
	{"User Agent", cgixx::header_http_user_agent}
};

const int varcount = sizeof(vars)/sizeof(vars_s);

void test()
{
	cgixx::cgi cgi;
	cgixx::header header;
	header.setexpire("-1D");
	std::cout << header.get();
	std::string val;

	std::cout << "<html>\n<head><title>test</title></head>\n<body>\n";
	std::cout << "<h1>test</h1>\n";
	for (int i=0; i<varcount; ++i)
	{
		if (cgi.getheader(vars[i].hid, val))
			val = "Invalid header";
		std::cout << vars[i].desc << ": " << val << "<br>\n";
	}
	
	std::cout << "</body>\n</html>\n";
	std::cout.flush();
}

