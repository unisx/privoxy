#!/usr/bin/perl -w

# $Id: filter2docs.pl,v 1.2 2006/07/18 14:49:14 david__schmidt Exp $
# $Source: /cvsroot/ijbswa/current/utils/filter2docs.pl,v $

# Parse the filter names and descriptions from a filter file and
# spit out copy&paste-ready markup for the various places in
# configuration and documentation where all filters are listed.

die "Usage: $0 filter-file\n" unless (@ARGV == 1) ;
open(INPUT, "< $ARGV[0]") or die "Coudln't open input file $ARGV[0] because $!\n";

while (<INPUT>) {
  if (/^FILTER: ([-\w]+) (.*)$/) {
    $comment_lines .= "#     $1:" . (" " x (20-length($1))) . "$2\n";
    $action_lines  .= "-filter{$1} \\\n";
    $sgml_source_1 .= "   <para>\n    <anchor id=\"filter-$1\">\n    <screen>+filter{$1}" .
                      (" " x (20-length($1))) . "# $2</screen>\n   </para>\n";
    $sgml_source_2 .= " -<link linkend=\"FILTER-" . uc($1) . "\">filter{$1}</link> \\\n";
  }
}

print("Comment lines for default.action:\n\n$comment_lines\n\nBlock of filter actions for"
     ." standard.action:\n\n$action_lines\n\nSGML Source for AF chapter in U-M:" .
      "\n\n$sgml_source_1\n\nSGML Source for AF Tutorial chapter in U-M\n\n$sgml_source_2\n");
