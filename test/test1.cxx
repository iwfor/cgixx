/*
 * test1.cxx
 *
 * $Id$
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
	cgixx::cookie cookie(cgi, "My-Cookie", "'Tis the value!");
	cgixx::header header;

	cookie.setsecure(true);
	cookie.setexpire("2m");

	header.setexpire("-1D");
	header.addcookie(cookie);

	std::cout << header.get();
}