/*
 * cgi_impl.h
 *
 * $Id$
 *
 */

#ifndef __cgixx_cgi_impl_h
#define __cgixx_cgi_impl_h

#include <cgixx/cgi.h>
#include <string>
#include <map>

namespace cgixx {

typedef std::map< std::string, std::string > ParameterList;

struct cgi_impl {
	cgi_impl();
	void parseparams(const std::string& paramlist);

	// Store an environment variable in the specified string.
	void getenvvar(std::string& dest, const char* name, const char* defval=0);

	std::string cgi2text(const std::string& cgistr);

	// Map of parameters.
	ParameterList vars;

	// The method with which the request was made. For HTTP, this is
	// "GET", "HEAD", "POST", etc.
	methods method;

	// The information which follows the ? in the URL  which referenced
	// this script. This is the query information. It should not be
	// decoded in any fashion. This variable should always be set when
	// there is query information, regardless of command line decoding.
	std::string query_string;

	// The name and version of the information server software answering
	// the request (and running the gateway).  Format: name/version
	std::string server_software;

	// The server's hostname, DNS alias, or IP address as it would
	// appear in self-referencing URLs.
	std::string server_name;

	// The revision of the CGI specification to which this server
	// complies. Format: CGI/revision
	std::string gateway_interface;

	// The name and revision of the information protcol this request
	// came in with. Format: protocol/revision
	std::string server_protocol;

	// The port number to which the request was sent.
	std::string server_port;

	// The extra path information, as given by the client. In other
	// words, scripts can be accessed by their virtual pathname,
	// followed by extra information at the end of this path. The extra
	// information is sent as PATH_INFO. This information should be
	// decoded by the server if it comes from a URL before it is passed
	// to the CGI script.
	std::string path_info;

	// The server provides a translated version of PATH_INFO, which
	// takes the path and does any virtual-to-physical mapping to it.
	std::string path_translated;

	// A virtual path to the script being executed, used for
	// self-referencing URLs.
	std::string script_name;

	// The hostname making the request. If the server does not have this
	// information, it should set REMOTE_ADDR and leave this unset.
	std::string remote_host;

	// The IP address of the remote host making the request.
	std::string remote_addr;

	// If the server supports user authentication, and the script is
	// protects, this is the protocol-specific authentication method
	// used to validate the user.
	std::string auth_type;

	// If the server supports user authentication, and the script is
	// protected, this is the username they have authenticated as.
	std::string remote_user;

	// If the HTTP server supports RFC 931 identification, then this
	// variable will be set to the remote user name retrieved from the
	// server. Usage of this variable should be limited to logging only.
	std::string remote_ident;

	// For queries which have attached information, such as HTTP POST
	// and PUT, this is the content type of the data.
	std::string content_type;

	// The length of the said content as given by the client.
	unsigned long content_length;

	// The MIME types which the client will accept, as given by HTTP
	// headers. Other protocols may need to get this information from
	// elsewhere. Each item in this list should be separated by commas
	// as per the HTTP spec.  Format: type/subtype, type/subtype
	std::string http_accept;

	// The browser the client is using to send the request. General
	// format: software/version library/version.
	std::string http_user_agent;
};

} // end namespace cgixx

#endif // __cgixx_cgi_impl_h
