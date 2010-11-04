/*
 * test1.cxx
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

#include <cgixx/cgi.h>
#include <cgixx/header.h>
#include <cgixx/cookie.h>
#include <iostream>
#include <stdexcept>

void test();

int main()
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
	cgixx::cookie cookie(cgi, "My-Cookie", "'Tis the value!");
	cgixx::header header;

	cookie.setsecure(true);
	cookie.setexpire("2m");

	header.setexpire("-1D");
	header.addcookie(cookie);

	std::cout << header.get();
}
