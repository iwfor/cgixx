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

class header {
public:
	header();
	~header();

	void setsize(unsigned size);
	
	void addcookie(cookie& value);

	// Content-type
	void settype(const std::string& contenttype);

	// Expire
	void setexpire(const std::string& expire);

	// Set a generic header
	void setheader(const std::string& id, const std::string& value);

	// get the header string
	std::string get();

private:
	header_impl* imp;
};

} // end namespace cgixx

#endif // __cgixx_header_h
