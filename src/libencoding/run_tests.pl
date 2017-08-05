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

my @libencoding_tests = qw(
    ./base64_test
);

my $base64_test_result = <<EOF;
result_test_encoding_1 YQ==
result_test_encoding_2 YWI=
result_test_encoding_3 YWJj
result_test_encoding_4 YWJjZA==
result_test_encoding_5 VGhlIHRlc3Qgb2YgdGhlIGxvbmcgbG9uZywgdmVyeSBsb25nIGxpbmUsIGEgbGl0dGxlIGJpdCBsb25nZXIgdGhhbiBzZXZlbnR5IHNpeCBieXRlcyBsZW5ndGg=
EOF

print "Running: libencoding tests:\n";

for my $test (@libencoding_tests) {
        my $result=`$test`;

        if ("$?" != 0) {
            die "$test failed";
        }

        if ($test eq "./base64_test") {
	    print "base64_test...............";

	    # We may have multiline result
	    $result =~ s/\r\n//g;

	    if ($result ne $base64_test_result) {
		print_diff($result, $base64_test_result);
	    }

	    print PUSHCOLOR GREEN "ok\n";
	    print POPCOLOR;
	}
}

exit 0;
