#!/usr/bin/env perl
#
# Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);
use File::Basename;
use File::Find;

my $argc = @ARGV;
my ($tmail_dir) = @ARGV;

if ($argc < 1) {
    $tmail_dir = `pwd`;
    chop($tmail_dir);
}

if (! -d $tmail_dir) {
    print PUSHCOLOR RED "apply-clang-format.pl: path to tmail root dir should be passed.";
    print POPCOLOR;
    exit 1;
}

sub traverse_files {
    my $filename = $File::Find::name;
    my ($name, $path, $suffix) = fileparse($filename, "\.[ch]");
    if ($suffix eq ".h" || $suffix eq ".c") {
        my $source_file = $path . $name . $suffix;
	system("clang-format", "-i", $source_file);
    }
}

if (basename($tmail_dir) eq "tmail" || basename($tmail_dir) eq "src") {
    my $source_dir = $tmail_dir . "/src/tmail.c";
    my $main_c_file = $tmail_dir . "/tmail.c";
    if (-e $source_dir || -e $main_c_file) {
	find(\&traverse_files, $tmail_dir);
	exit 0;
    }
}

print PUSHCOLOR RED "apply-clang-format.pl: path to tmail root dir should be passed.";
print POPCOLOR;

exit 1;
