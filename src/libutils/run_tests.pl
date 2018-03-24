#!/usr/bin/env perl
#
# Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

require "../../scripts/test_diff.pl";

my @libutils_tests = qw(
    ./list_test
    ./stack_test
    ./hashmap_test
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

my $stack_test_result = <<EOF;
stack size test: 5
stack pop test #1: string5
stack pop test #2: string4
stack push `string4` and pop again test #3: string4
stack size test: 3
EOF

my $hashmap_test_result = <<EOF;
hashmap created successfully
hashmap put test 1
hashmap_get test 1 OK
hashmap_get test 2 OK
hashmap_get test 5 OK
hashmap_get test 6 OK
remove key_test_1
hashmap_remove test 1 OK
put key_test_1 again
hashmap_get test 7 OK
hashmap_get test 8 OK
hashmap_get test 9 OK
val of user2\@server.com is val_9
val of user3\@server.com is val_10
removing user2\@server.com
hashmap_remove test 2 OK
EOF

print "Running: libutils tests:\n";

for my $test (@libutils_tests) {
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

	if ($test eq "./stack_test") {
               print "stack_test...............";
               
               if ($result ne $stack_test_result) {
                      print_diff($result, $stack_test_result);
               }

               print PUSHCOLOR GREEN "ok\n";
               print POPCOLOR;
        }

	if ($test eq "./hashmap_test") {
               print "hashmap_test.............";
               
               if ($result ne $hashmap_test_result) {
                      print_diff($result, $hashmap_test_result);
               }

               print PUSHCOLOR GREEN "ok\n";
               print POPCOLOR;	    
        }
}

exit 0;
