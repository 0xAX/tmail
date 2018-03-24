#!/usr/bin/env perl
#
# Copyright (c) 2017-2018, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Term::ANSIColor 2.00 qw(:pushpop);
use File::Basename;
use File::Find;

my $argc = @ARGV;

if (! -e "/var/run/docker.sock") {
    print PUSHCOLOR RED "test-smtp-relay.pl: Docker daemon should be started\n";
    print POPCOLOR;
    exit 1;
}

if ($argc != 2) {
    print PUSHCOLOR RED "test-smtp-relay.pl: You should pas you GMAIL email as the first parameter and password as second.\n";
    print POPCOLOR;
    exit 1;
}

my $test_docker = basename(`whereis docker | awk \'{print \$2}\'`);

chomp $test_docker;

if ($test_docker ne "docker") {
    print PUSHCOLOR RED "test-smtp-relay.pl: Error: docker isn't available on your machine.\n";
    print POPCOLOR;
    exit 1;
}

my $email = "EXIM_GMAIL_LOGIN=" . $ARGV[0];
my $password = "EXIM_GMAIL_PASSWORD=" . $ARGV[1];

my $exim_relay_launched = `docker inspect exim-gmail-relay 2>/dev/null`;

if ($exim_relay_launched =~ "running") {
    system("docker", "stop", "exim-gmail-relay");
}

# check is smtp-net network exists or not
my $network = `docker network inspect smtp-net 2>/dev/null`;
if ($network =~ "100.100.0.0/16") {
    system("docker", "network", "rm", "smtp-net");
}

# create network
system("docker", "network", "create",
       "--subnet", "100.100.0.0/16",
       "--driver", "bridge",
       "smtp-net");

# start container
system("docker", "rm", "exim-gmail-relay");
system("docker", "run", "--name", "exim-gmail-relay",
       "--net", "smtp-net",
       "-e", $email,
       "-e", $password,
       "-d", "selim13/exim-gmail-relay:latest");

exit 0;
