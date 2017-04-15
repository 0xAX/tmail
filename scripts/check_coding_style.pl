#!/usr/bin/env perl
#
# Copyright (c) 2017, Alexander Kuleshov <kuleshovmail at gmail dot com>
#
# This file is part of tmail application and released under the BSD license, 
# see the COPYING file

use strict;
use warnings;
use Cwd;
use File::Basename;
use File::Spec;

my $curr_dir = getcwd;
my $curr_dir_basename = basename($curr_dir);
my $tmail_dir = get_tmail_source_dir();
my $source_dir = $tmail_dir . "/src";
my $root_makefile = $tmail_dir . "/Makefile";
    
# Try to get correct path to tmail root
# directory.
sub get_tmail_source_dir {
    if ($curr_dir_basename =~ "tmail") {
        return $curr_dir;
    }
    elsif ($curr_dir_basename =~ "scripts") {
        my @dirs = File::Spec->splitdir($curr_dir);

        if ($dirs[scalar @dirs - 2] =~ "tmail") {
	    return dirname($curr_dir);
        }
    }
    elsif ($#ARGV > -1) {
        return $ARGV[0];
    }
    else {
        print "The script should be runned from `tmail` or tmail/scripts directory.\n",
              "In other way, the path to the `tmail` directory should be passed as first argument.\n";
	exit 1;
    }
}

# Last sanity check for tmail source code directory
if (! -e $source_dir && ! -d $source_dir && ! -e $root_makefile) {
    print "Wrong pass of tmail's root directory: $tmail_dir\n\n";
    print "The script should be runned from `tmail` or tmail/scripts directory.\n",
          "In other way, the path to the `tmail` directory should be passed as first argument.\n";

    exit 1;
}

# Go to the tmail root dir
chdir $tmail_dir;

# Go through modified/untracked files
# and check them.
my $status = `git status --porcelain`;
while ($status =~ /([^\n]+)\n?/g) {
    my $line = $1;

    # moved files
    if ($line =~ /^R.*$/ && $line =~ /^.*\.c/) {
        print $line, "\n"
    }

    # untracked and modified files
    if ($line =~ /\?\?/ || $line =~ /^.M/) {

        my $file = $line =~ / /;
        print $file, "\n"
    }
}


# Go to the directory where we came from before exit
chdir $curr_dir;

exit 0;
