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

// Forward declaration
struct cgi_impl;
class cookie;

class cgi {
public:
	cgi();
	~cgi();

	// Get count of a variable
	unsigned count(const std::string& id);

	// Get next available value of a variable
	bool get(const std::string& id, std::string& value);

	// Get next available value of a cookie
	bool getcookie(const std::string& id, cookie& value);

	// Get server
	const std::string& server() const;

	// Get path
	const std::string& path() const;

	static std::string text2cgi(const std::string& text);
	static std::string cgi2text(const std::string& message);

private:
	operator=(const cgi&);

	cgi_impl* imp;
};

//extern char **environ;

} // end namespace cgixx

#endif // __cgixx_cgi_h
