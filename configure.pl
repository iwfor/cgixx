#! /usr/bin/perl
#
# configure.pl (bootstrap cgixx)
# Use on *nix platforms.
#
# cgixx CGI C++ Class Library
# Copyright (C) 2002 Isaac W. Foraker (isaac@tazthecat.net)
# All Rights Reserved
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
# 
# 3. Neither the name of the Author nor the names of its contributors
#    may be used to endorse or promote products derived from this
#    software without specific prior written permission.
# 
# THIS SOFTWARE AND DOCUMENTATION IS PROVIDED BY THE AUTHOR AND
# CONTRIBUTORS ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING,
# BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
# FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE
# AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
# TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
# LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
# NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# Most of this code is plagerized from configure.pl, written by Peter J Jones
# for use in clo++, and uses Peter's cxxtools.

#####
# Includes
use strict;
use Getopt::Long;
use Cwd qw(cwd chdir);

#####
# Constants
use constant DATE		=> 'Thu May  10 4:44:54 2002';
use constant ID			=> '$Id$';

#####
# Global Variables
my $cwd = cwd();
my %clo;
my $dirsep = "/";

my $mkmf	= "${cwd}${dirsep}tools${dirsep}mkmf";
my $cxxflags	= "${cwd}${dirsep}tools${dirsep}cxxflags";
my $mkmf_flags  = "--cxxflags '$cxxflags' --quiet ";

my $libname	= "cgixx";
my $install_spec= "doc${dirsep}install.spec";

my $includes	= "--include '${cwd}${dirsep}inc' ";
my $libraries	= "--slinkwith '${cwd}${dirsep}src,$libname' ";

my @extra_compile = (
	"${cwd}${dirsep}src"
);

#####
# Code Start
$|++;

if (not defined $ENV{'CXX'}) {
	print STDERR "*** your CXX environment variable is not set. cgixx needs this ***\n";
	print STDERR "*** variable to find your C++ compiler. Please set it to the   ***\n";
	print STDERR "*** path to your compiler and re-run configure.pl. Thanks.     ***\n";
	exit 1;
}

GetOptions(
	\%clo,
	'help',
	'developer',
	'prefix=s',
	'bindir=s',
	'incdir=s',
	'libdir=s',
) or usage();
$clo{'help'} && usage();

sub usage {
	print "Usage: $0 [options]\n", <<EOT;
  --developer        Turn on developer mode

  --prefix path      Set the install prefix to path  [/usr/local]
  --bindir path      Set the install bin dir to path [PREFIX/bin]
  --incdir path      Set the install inc dir to path [PREFIX/inc]
  --libdir path      Set the install lib dir to path [PREFIX/lib]

  --xml2-config path  Use the xml2-config script given by path.
EOT
	exit;
}

$clo{'prefix'}	||= "/usr/local";
$clo{'bindir'}	||= "$clo{'prefix'}/bin";
$clo{'incdir'}	||= "$clo{'prefix'}/include";
$clo{'libdir'}	||= "$clo{'prefix'}/lib";
	
if ($clo{'developer'}) {
	$mkmf_flags .= "--developer ";
}

print "Generating cgixx Makefiles ";
generate_toplevel_makefile();
generate_library_makefile();
print "\n";
print "+-------------------------------------------------------------+\n";
print "| Okay, looks like you are ready to go.  To build, type:      |\n";
print "|                                                             |\n";
print "|       make                                                  |\n";
print "|                                                             |\n";
print "| To install, type:                                           |\n";
print "|                                                             |\n";
print "|       make install                                          |\n";
print "|                                                             |\n";
print "| While you wait, why not drop a note to isaac\@tazthecat.net? |\n";
print "+-------------------------------------------------------------+\n";


sub generate_toplevel_makefile {
	unless (open(SPEC, ">$install_spec")) {
		print STDERR "\n$0: can't open $install_spec: $!\n";
		exit 1;
	}

	print SPEC "libdir=$clo{'libdir'}\n";
	print SPEC "static-lib src cgixx\n";
	print SPEC "includedir=$clo{'incdir'}\n";
	print SPEC "include-dir inc/cgixx cgixx\n";
	close SPEC;

	system("$^X $mkmf $mkmf_flags --install $install_spec --wrapper @extra_compile");
	print ".";
}


sub generate_library_makefile {
	if (not chdir("$cwd${dirsep}src")) {
		print STDERR "\n$0: can't chdir to src: $!\n";
		exit 1;
	}

	system("$^X $mkmf $mkmf_flags $includes --static-lib $libname *.cxx");
	print ".";
	chdir $cwd;
}
