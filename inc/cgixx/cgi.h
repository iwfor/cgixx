/*
 * cgi.h
 *
 * $Id$
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

class cgi {
public:
	cgi();
	~cgi();

	// Get the method used
	methods method() const;

	// Get count of a variable
	unsigned count(const std::string& id) const;

	// Get next available value of a variable
	bool get(const std::string& id, std::string& value) const;

	// Get next available value of a cookie
	bool getcookie(const std::string& id, cookie& value) const;

	// Get server
	const std::string& server() const;

	// Get path
	const std::string& path() const;

private:
	operator=(const cgi&);

	cgi_impl* imp;
};

//extern char **environ;

} // end namespace cgixx

#endif // __cgixx_cgi_h
