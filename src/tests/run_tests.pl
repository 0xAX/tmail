#!/usr/bin/env perl
#
# Copyright (c) 2017-2019, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license,
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);

require "../../scripts/test_diff.pl";

my @tmail_tests = qw(
    ./mime_test
    ./config_test
);

my $mime_test_result = <<EOF;
result_test_1 application/json
result_test_2 application/vnd.sun.xml.calc
result_test_3 text/plain
result_test_4 text/plain
result_test_5 application/x-perl
result_test_6 application/excel
EOF

my $config_test_result = <<EOF;
username smtprc test
username SMTP server is - smtp.example.com
username SMTP port is - 587
username SMTP realname is - Name
username SMTP from is - username\@server.com
username SMTP password is - password
username smtprc test is done
test smtprc test
test SMTP server is - test.smtp.example.com
test SMTP port is - 25
test SMTP realname is - Name1 Name2
test SMTP from is - test\@server.com
test SMTP password is - my_test_pass
test smtprc test is done
EOF

print "Running: tmail code tests \n";

for my $test (@tmail_tests) {
    my $result=`$test`;

    if ("$?" != 0) {
	die "$test failed. Exit code is $?";
    }

    # We may have multiline result
    $result =~ s/\r\n//g;

    if ($test eq "./mime_test") {
	print "mime_test...............";

	if ($result ne $mime_test_result) {
	    print_diff($result, $mime_test_result);
	}

	print PUSHCOLOR GREEN "ok\n";
	print POPCOLOR;
    }

    if ($test eq "./config_test") {
	print "config_test...............";

	if ($result ne $config_test_result) {
	    print_diff($result, $config_test_result);
	}

	print PUSHCOLOR GREEN "ok\n";
	print POPCOLOR;
    }
}

exit 0;
