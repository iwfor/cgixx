/*
 * cgi.cxx
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
#include "cgi_impl.h"

namespace cgixx {

static std::string cgixx_version("0.60");

/**
 * Construct an instance of cgi.
 *
 */
cgi::cgi() : imp(new cgi_impl)
{
}


/**
 * Destruct *this instance of cgi.
 *
 */
cgi::~cgi()
{
	delete imp;
}


/**
 * Get the cgixx library version string.
 *
 * @param	none;
 * @return	The cgixx version string.
 *
 */
const std::string& cgi::libver()
{
	return cgixx_version;
}


/**
 * Get the CGI method requested by the web client.
 *
 * @param	none
 * @return	Member of the methods enumeration.
 *
 */
methods cgi::method() const
{
	return imp->method;
}


/**
 * Get the server's hostname, DNS alias, or IP address as it would
 * appear in self-referencing URLs.
 *
 * @param	none
 * @return	Name of web server.
 *
 */
const std::string& cgi::server() const
{
	return imp->server_name;
}


/**
 * Get the port number to which the request was sent.
 *
 * @param	none
 * @return	The web server port number (typically 80).
 *
 */
unsigned cgi::serverport() const
{
	return imp->server_port;
}


/**
 * Get the name and revision of the information protcol this request
 * came in with. Format: protocol/revision
 *
 * @param	none
 * @return	Server protocol.
 *
 */
const std::string& cgi::serverprotocol() const
{
	return imp->server_protocol;
}


/**
 * Get the revision of the CGI specification to which this server
 * complies. Format: CGI/revision
 *
 * @param	none
 * @return	Web server CGI version.
 *
 */
const std::string& cgi::gatewayinterface() const
{
	return imp->gateway_interface;
}


/**
 * Get the name and version of the web server software.  Format:
 * name/version
 *
 * @param	none
 * @return	Name of the web server software.
 *
 */
const std::string& cgi::serversoftware() const
{
	return imp->server_software;
}


/**
 * Get the extra path information, as given by the client. In other
 * words, scripts can be accessed by their virtual pathname, followed by
 * extra information at the end of this path.
 *
 * @param	none
 * @return	Extra URL path information.
 *
 */
const std::string& cgi::pathinfo() const
{
	return imp->path_info;
}


/**
 * Get the server translated version of pathinfo(), which takes the path
 * and does any virtual-to-physical mapping to it.
 *
 * @param	none
 * @return	Physical path to CGI script.
 *
 */
const std::string& cgi::realpath() const
{
	return imp->path_translated;
}


/**
 * Get the virtual path to the script being executed, used for
 * self-referencing URLs.
 *
 * @param	none
 * @return	Virtual path to this CGI script.
 *
 */
const std::string& cgi::script() const
{
	return imp->script_name;
}


/**
 * Get the hostname making the request. If the server does not have this
 * information, it should set remoteaddr() and leave this unset.
 *
 * @param	none
 * @return	The remote host name, if available.
 *
 */
const std::string& cgi::remotehost() const
{
	return imp->remote_host;
}


/**
 * Get the IP address of the remote host making the request.
 *
 * @param	none
 * @return	The IP address of the web client.
 *
 */
const std::string& cgi::remoteaddr() const
{
	return imp->remote_addr;
}


/**
 * If the server supports user authentication, and the script is
 * protected, this is the protocol-specific authentication method used
 * to validate the user.
 *
 * @param	none
 * @return	The authorization method, if available.
 *
 */
const std::string& cgi::authtype() const
{
	return imp->auth_type;
}


/**
 * If the server supports user authentication, and the script is
 * protected, this is the username they have authenticated as.
 *
 * @param	none
 * @return	Authenticated username, if available.
 *
 */
const std::string& cgi::remoteusername() const
{
	return imp->remote_user;
}


/**
 * If the HTTP server supports RFC 931 identification, then this
 * variable will be set to the remote user name retrieved from the
 * server. Usage of this variable should be limited to logging only.
 *
 * @param	none
 * @return	The remote identity, if available.
 *
 */
const std::string& cgi::remoteidentity() const
{
	return imp->remote_ident;
}


/**
 * For queries which have attached information, such as HTTP POST and
 * PUT, get the content type of the data.
 *
 * @param	none
 * @return	The content type, if available.
 *
 */
const std::string& cgi::contenttype() const
{
	return imp->content_type;
}


/**
 * The length of the content as given by the client in a POST or PUT.
 *
 * @param	none
 * @return	The content length, if available.
 *
 */
unsigned long cgi::contentlength() const
{
	return imp->content_length;
}


/**
 * Get the MIME types which the client will accept, as given by HTTP
 * headers.  Each item in this list should be separated by commas as per
 * the HTTP spec.  Format: type/subtype, type/subtype
 *
 * @param	none
 * @return	The mime types accepted by the client.
 *
 */
const std::string& cgi::httpaccept() const
{
	return imp->http_accept;
}


/**
 * The browser the client is using to send the request. General format:
 * software/version library/version.
 *
 * @param	none
 * @return The name of the user agent.
 *
 */
const std::string& cgi::agent() const
{
	return imp->http_user_agent;
}

/**
 * Get the count of values for the CGI variable with the specified id.
 * This count is decremented with each call to get.
 *
 * @param	id		Identifier of cookie.
 * @return	Count of values for specified cookie.
 *
 */
unsigned cgi::count(const std::string& id) const
{
	ParameterList::const_iterator it(imp->vars.find(id));
	if (it == imp->vars.end())
		return 0;
	return it->second.size();
}


/**
 * Get the next value of the CGI variable with the specified id.  Once a
 * value has been retrieved, that value is removed, and the next value,
 * if any, is will be available on the next call to get.
 *
 * @param	id		Identifier of CGI variable.
 * @param	value	Reference to string to receive value of variable.
 * @return	false on success; true when no more values are available.
 *
 */
bool cgi::get(const std::string& id, std::string& value)
{
	ParameterList::iterator it(imp->vars.find(id));
	if (it == imp->vars.end())
		return true;
	strqueue& sq = it->second;
	value = sq.front();
	sq.pop();
	if (it->second.empty())
		imp->vars.erase(it);
	return false;
}


/**
 * Get the count of values for the cookie with the specified id.  This
 * count is decremented with each call to getcookie.
 *
 * @param	id		Identifier of cookie.
 * @return	Count of values for specified cookie.
 *
 */
unsigned cgi::countcookie(const std::string& id) const
{
	ParameterList::const_iterator it(imp->cookies.find(id));
	if (it == imp->cookies.end())
		return 0;
	return it->second.size();
}


/**
 * Get the next value of the cookie with the specified id.  Once a
 * cookie has been retrieved, that value is removed, and the next value,
 * if any, is made available.
 *
 * @param	id		Identifier of cookie.
 * @param	value	Reference to string to receive value of cookie.
 * @return	false on success; true when no more values are available.
 *
 */
bool cgi::getcookie(const std::string& id, std::string& value)
{
	ParameterList::iterator it(imp->cookies.find(id));
	if (it == imp->cookies.end())
		return true;
	strqueue& sq = it->second;
	value = sq.front();
	sq.pop();
	if (it->second.empty())
		imp->cookies.erase(it);
	return false;
}


} // end namespace cgixx
