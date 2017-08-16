#!/usr/bin/env perl
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of libands application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

sub print_diff {
    my ($result, $expected) = @_;
    my @result_splitted = split(/\n/, $result);
    my @expected_splitted = split(/\n/, $expected);
    
    for my $res (0 .. $#result_splitted) {
            if ($result_splitted[$res] ne $expected_splitted[$res]) {
                   print PUSHCOLOR RED "failed\n";
                   print "ERROR:\n";
                   print "  expected: $expected_splitted[$res]\n";
                   print "  got: $result_splitted[$res]\n";
		   print POPCOLOR;
		   exit 1;
            }
    }
}

1;
