#! /usr/bin/perl
#
# configure.pl (bootstrap cgixx)
# Use on *nix platforms.
#
# cgixx CGI C++ Class Library
# Copyright (C) 2002-2004 Isaac W. Foraker (isaac at noscience dot net)
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

# Possible names for the compiler
my @cxx_guess = qw(g++ c++ CC cl bcc32);

my $mkmf	= "${cwd}/tools/mkmf";
my $cxxflags	= "${cwd}/tools/cxxflags";
my $mkmf_flags  = "--cxxflags '$cxxflags' --quiet ";

my $libname	= "cgixx";
my $install_spec= "doc/install.spec";

my $includes	= "--include '${cwd}/inc' ";
my $libraries	= "--slinkwith '${cwd}/src,$libname' ";

my @compile_dirs = (
	"${cwd}/src",
	"${cwd}/test"
);

#####
# Code Start
$|++;

GetOptions(
	\%clo,
	'help',
	'bundle',
	'developer',
	'prefix=s',
	'bindir=s',
	'incdir=s',
	'libdir=s',
	'cxx=s'
) or usage();
$clo{'help'} && usage();

sub usage {
	print "Usage: $0 [options]\n", <<EOT;
--developer        Turn on developer mode

--prefix path      Set the install prefix to path  [/usr/local]
--bindir path      Set the install bin dir to path [PREFIX/bin]
--incdir path      Set the install inc dir to path [PREFIX/inc]
--libdir path      Set the install lib dir to path [PREFIX/lib]
EOT
	exit;
}

$clo{'prefix'}	||= "/usr/local";
$clo{'bindir'}	||= "$clo{'prefix'}/bin";
$clo{'incdir'}	||= "$clo{'prefix'}/include";
$clo{'libdir'}	||= "$clo{'prefix'}/lib";

print "Configuring cgixx...\n";

if ($clo{'developer'}) {
	print "Developer extensions... enabled\n";
	$mkmf_flags .= "--developer ";
}

#####
# Determine C++ compiler settings
$clo{'cxx'} ||= $ENV{'CXX'};
if (not $clo{'cxx'}) {
	print "Checking C++ compiler... ";
	my $path;
	# search for a compiler
	foreach (@cxx_guess) {
		if ($path = search_path($_)) {
			$clo{'cxx'} = "$path/$_";
			last;
		}
	}
	if ($clo{'cxx'}) {
		print "$clo{'cxx'}\n";
	} else {
		print <<EOT;
Not found.

You must specify your C++ compiler with the --cxx parameter or by setting the
CXX environment variable.
EOT
		exit;
	}
} else {
	print "Using C++ compiler... $clo{'cxx'}\n";
}

print "Generating cgixx Makefiles ";
generate_toplevel_makefile();
generate_library_makefile();
generate_tests_makefile();
print "\n";
if (!$clo{'bundle'}) {
	print "\n";
	print "Install Prefix:          $clo{'prefix'}\n";
	print "Binary Install Path:     $clo{'bindir'}\n";
	print "Includes Install Path:   $clo{'incdir'}\n";
	print "Libraries Install Path:  $clo{'libdir'}\n";
	print "\n";

	print <<EOT;
===============================================================================

Configuration complete.  To build, type:

make

To install, type:

make install

===============================================================================
EOT
}


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

	system("$^X $mkmf $mkmf_flags --install $install_spec --wrapper @compile_dirs");
	print ".";
}


sub generate_library_makefile {
	if (not chdir("$cwd/src")) {
		print STDERR "\n$0: can't chdir to src: $!\n";
		exit 1;
	}

	system("$^X $mkmf $mkmf_flags $includes --static-lib $libname *.cxx");
	print ".";
	chdir $cwd;
}

sub generate_tests_makefile {
	unless (chdir("$cwd/test")) {
		print STDERR "\n$0: can't chdir $cwd/test: $!\n";
		exit 1;
	}
	system("$^X $mkmf $mkmf_flags $includes $libraries --quiet --many-exec *.cxx");
	print ".";
	chdir $cwd;
}

# Search the path for the specified program.
sub search_path {
	my $prog = shift;
	# Determine search paths
	my $path = $ENV{'PATH'} || $ENV{'Path'} || $ENV{'path'};
	my @paths = split /[;| |:]/, $path;

	my $ext = $^O =~ /win/i ? '.exe' : '';

	foreach (@paths) {
		if (-e "$_/$prog$ext") {
			return $_;
		}
	}
	return undef;
}

