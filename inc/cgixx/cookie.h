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
	cookie(const cookie& copy);
	~cookie();

	void setvalue(const std::string& value);
	void setdomain(const std::string& domain);
	void setpath(const std::string& path);
	void setexpire(const std::string& expires);
	void setexpire(const std::tm& expires);
	void setsecure(bool requiressl);

	const std::string& getname() const;
	const std::string& getvalue() const;

	// Get cookie string
	std::string get();

private:
	cookie();
	cookie& operator=(const cookie&);

	cookie_impl* imp;
};

} // end namespace cgixx

#endif // __cgixx_cookie_h
