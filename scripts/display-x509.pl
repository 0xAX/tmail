#!/usr/bin/env perl
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

my $argc = @ARGV;

if ($argc < 1) {
    print PUSHCOLOR RED "display-x509.pl: path to x509 certificate should be passed.\n";
    print POPCOLOR;
    exit 1;
}

my ($cert) = @ARGV;

# check is openssl is installed
my $openssl = `whereis openssl`;
$openssl =~ s/openssl\:\s//;

if ($openssl eq "") {
    print PUSHCOLOR RED "display-x509.pl: execpted that openssl is installed\n";
    print POPCOLOR;
    exit 1;
}

system("openssl", "x509", "-inform", "der", "-in", $cert, "-noout",  "-text");

exit 0;
