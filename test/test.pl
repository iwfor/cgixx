#!/usr/bin/perl

$ENV{SERVER_NAME} = "www.thisdomain.net";
$ENV{PATH_INFO} = "/cgi-bin/script.pl";
$ENV{REQUEST_METHOD} = "GET";
$ENV{QUERY_STRING} = "var1=val1&var2=val2";

system("./test1");
