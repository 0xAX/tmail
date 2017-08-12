#!/usr/bin/env perl
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

require "../../scripts/test_diff.pl";

my @mime_tests = qw(
    ./mime_test
);

my $mime_test_result = <<EOF;
result_test_1 application/json
result_test_2 application/vnd.sun.xml.calc
result_test_3 text/plain
result_test_4 text/plain
result_test_5 application/x-perl
result_test_6 application/excel
EOF

print "Running: tmailc code tests \n";

for my $test (@mime_tests) {
    my $result=`$test`;

    if ("$?" != 0) {
	die "$test failed";
    }

    if ($test eq "./mime_test") {
	print "mime_test...............";

	# We may have multiline result
	$result =~ s/\r\n//g;

	if ($result ne $mime_test_result) {
	    print_diff($result, $mime_test_result);
	}

	print PUSHCOLOR GREEN "ok\n";
	print POPCOLOR;
    }
}

exit 0;
