#!/usr/bin/env perl
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

my @libutils_tests = qw(
    ./list_test
);

my $list_test_result = <<EOF;
list append test: 1 2 3 4 5 
list prepend test: 5 1 2 3 4 5 
list lookup test: 5 0 
list nth test: 5 3
list length test: 6
list remove test: 1 2 3 4 5 1 3 4 5 
list length after remove test: 4
EOF

for my $test (@libutils_tests) {
        print "Running: libutils tests:\n";

        my $result=`$test`;

        if ("$?" != 0) {
            die "$test failed";
        }

        if ($test eq "./list_test") {
               print "list_test...............";
               
               if ($result ne $list_test_result) {
                      print_diff($result, $list_test_result);
               }

               print PUSHCOLOR GREEN "ok\n";
               print POPCOLOR;
        }
}

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
            }
    }

    print POPCOLOR;
    exit 1;
}

exit 0;
