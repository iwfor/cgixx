/*
 * cookie.h
 *
 * $Id$
 *
 */

#ifndef __cgixx_cookie_h
#define __cgixx_cookie_h

#include <string>
#include <ctime>

namespace cgixx {

// Forward declaration
struct cookie_impl;
class cgi;

class cookie {
public:
	cookie(const cgi& initcgi, const std::string& name,
		const std::string& value="");
	cookie(const std::string& name, const std::string& value="");
	~cookie();

	void setvalue(const std::string& value);
	void setdomain(const std::string& domain);
	void setpath(const std::string& path);
	void setexpire(const std::string& expire);
	void setexpire(const std::tm& expire);
	void setsecure(bool requiressl);

	cookie& operator=(const cookie&);

	// Get cookie string
	std::string get();

private:
	cookie();
	cookie_impl* imp;
};

} // end namespace cgixx

#endif // __cgixx_cookie_h
