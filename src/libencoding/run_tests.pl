#!/usr/bin/env perl
#
# Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use MIME::Base64;
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

#
# Generate 10 random strings and pass it to base64 util
# and base_64_test to test them.
#
print "Running libencoding fuzzing tests:\n";
my @chars = ("A".."Z", "a".."z");
for (my $i=0; $i <= 100; $i++) {
    my $result = '';
    my $orig_str = '';
    my $base64_str = '';

    # Generate random string
    $orig_str .= $chars[rand @chars] for 1..int(rand(200) + 1);
    # Encode it with base64
    $base64_str .= encode_base64($orig_str, "\r\n");

    # Perl adds \r\n to the end of an encoded string everytime,
    # let's remove it for a test
    chop $base64_str;
    chop $base64_str;

    # pass them to base64_test
    $result = `./base64_test $orig_str \'$base64_str\'`;
    chomp $result;

    # skip empty lines
    if (length($orig_str) == 0) {
	next;
    }

    if ($result ne $base64_str) {
	print "Pass string: $orig_str\n";
	print "Expected: " . $base64_str . "\n";
	print "Got:      " . $result . "\n\n";

	exit 1;
    }

}

print "base64_test...............";
print PUSHCOLOR GREEN "ok\n";
print POPCOLOR;

exit 0;
