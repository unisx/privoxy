#!/usr/bin/perl

############################################################################
#
# Privoxy-Regression-Test
#
# A regression test "framework" for Privoxy. For documentation see:
# perldoc privoxy-regression-test.pl
#
# $Id: privoxy-regression-test.pl,v 1.1 2008/01/18 19:33:00 fabiankeil Exp $
#
# Wish list:
#
# - Update documentation
# - Validate HTTP times.
# - Understand default.action.master comment syntax
#   and verify that we actually block and unblock what
#   the comments claim we do.
# - Implement a HTTP_VERSION directive or allow to
#   specify whole request lines.
# - Support filter regression tests.
# - Add option to fork regression tests and run them in parallel,
#   possibly optional forever.
# - Document magic Expect Header values
# - Internal fuzz support?
#
# Copyright (c) 2007-2008 Fabian Keil <fk@fabiankeil.de>
#
# Permission to use, copy, modify, and distribute this software for any
# purpose with or without fee is hereby granted, provided that the above
# copyright notice and this permission notice appear in all copies.
#
# THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
# WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
# ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
# WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
# ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
# OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
#
############################################################################

use warnings;
use strict;
use Getopt::Long;

use constant {
               PRT_VERSION                => 'Privoxy-Regression-Test 0.2',
 
	       CURL                       => 'curl',

               # CLI option defaults
	       CLI_RETRIES  => 1,
	       CLI_LOOPS    => 1,
	       CLI_MAX_TIME => 5,
	       CLI_MIN_LEVEL => 0,
	       CLI_MAX_LEVEL => 25,

               PRIVOXY_CGI_URL => 'http://p.p/',
               FELLATIO_URL    => 'http://10.0.0.1:8080/',
               LEADING_LOG_DATE => 1,
               LEADING_LOG_TIME => 1,

               DEBUG_LEVEL_FILE_LOADING    => 0,
               DEBUG_LEVEL_PAGE_FETCHING   => 0,

               VERBOSE_TEST_DESCRIPTION    => 1,

               DEBUG_LEVEL_VERBOSE_FAILURE => 1,
               # XXX: Only partly implemented and mostly useless.
               DEBUG_LEVEL_VERBOSE_SUCCESS => 0,
               DEBUG_LEVEL_STATUS          => 1,

               # Internal use, don't modify
               # Available debug bits:
               LL_ERROR                   =>  1,
               LL_VERBOSE_FAILURE         =>  2,
               LL_PAGE_FETCHING           =>  4,
               LL_FILE_LOADING            =>  8,
               LL_VERBOSE_SUCCESS         => 16,
               LL_STATUS                  => 32,
               LL_SOFT_ERROR              => 64,

               CLIENT_HEADER_TEST         =>  1,
               SERVER_HEADER_TEST         =>  2,
               DUMB_FETCH_TEST            =>  3,
               METHOD_TEST                =>  4,
               TRUSTED_CGI_REQUEST        =>  6,
};

sub init_our_variables () {

    our $leading_log_time = LEADING_LOG_TIME;
    our $leading_log_date = LEADING_LOG_DATE;

    our $privoxy_cgi_url  = PRIVOXY_CGI_URL;

    our $verbose_test_description = VERBOSE_TEST_DESCRIPTION;

    our $log_level = get_default_log_level();

}

sub get_default_log_level () {
    
    my $log_level = 0;

    $log_level |= LL_FILE_LOADING    if DEBUG_LEVEL_FILE_LOADING;
    $log_level |= LL_PAGE_FETCHING   if DEBUG_LEVEL_PAGE_FETCHING;
    $log_level |= LL_VERBOSE_FAILURE if DEBUG_LEVEL_VERBOSE_FAILURE;
    $log_level |= LL_VERBOSE_SUCCESS if DEBUG_LEVEL_VERBOSE_SUCCESS;
    $log_level |= LL_STATUS          if DEBUG_LEVEL_STATUS;

    # These are intended to be always on.
    $log_level |= LL_SOFT_ERROR;
    $log_level |= LL_ERROR;

    return $log_level;
}

############################################################################
#
# File loading functions
#
############################################################################

sub parse_tag ($) {

    my $tag = shift;

    # Remove anchors
    $tag =~ s@[\$\^]@@g;
    # Unescape brackets and dots
    $tag =~ s@\\(?=[{}().+])@@g;

    # log_message("Parsed tag: " . $tag);

    check_for_forbidden_characters($tag);

    return $tag;
}

sub check_for_forbidden_characters ($) {

    my $tag = shift; # XXX: also used to check values though.
    my $allowed = '[-=\dA-Za-z{}:.\/();\s,+@"_%\?&]';

    unless ($tag =~ m/^$allowed*$/) {
        my $forbidden = $tag;
        $forbidden =~ s@^$allowed*(.).*@$1@;

        l(LL_ERROR, "'" . $tag . "' contains character '" . $forbidden. "' which is unacceptable.");
    }
}

sub load_regressions_tests () {

    our $privoxy_cgi_url;
    my @actionfiles;
    my $curl_url        = '';
    my $file_number = 0;

    $curl_url .= $privoxy_cgi_url;
    $curl_url .= 'show-status';

    l(LL_STATUS, "Asking Privoxy for the number of action files available ...");

    foreach (@{get_cgi_page_or_else($curl_url)}) {

        if (/<td>(.*?)<\/td><td class=\"buttons\"><a href=\"\/show-status\?file=actions&amp;index=(\d+)\">/) {

            my $url = $privoxy_cgi_url . 'show-status?file=actions&index=' . $2;
            $actionfiles[$file_number++] = $url;
        }
    }

    l(LL_FILE_LOADING, "Recognized " . @actionfiles . " actions files");

    load_action_files(\@actionfiles);
}

sub token_starts_new_test ($) {

    my $token = shift;
    my @new_test_directives =
        ('set header', 'fetch test', 'trusted cgi request', 'request header', 'method test');

    foreach my $new_test_directive (@new_test_directives) {
        return 1 if $new_test_directive eq $token;
    }
    return 0;

}

sub tokenize ($) {

    my ($token, $value) = (undef, undef);

    # Remove leading and trailing white space.
    s@^\s*@@;
    s@\s*$@@;

    # Reverse HTML-encoding
    # XXX: Seriously imcomplete. 
    s@&quot;@"@g;

    # Tokenize
    if (/^\#\s*([^=]*?)\s*[=]\s*(.*?)\s*$/) {

        $token = $1;
        $token =~ tr/[A-Z]/[a-z]/;
        $value = $2;

    } elsif (/^TAG\s*:(.*)$/) {

        $token = 'tag';
        $value = $1;

    }

    return ($token, $value);
}

sub load_action_files ($) {

    # initialized here
    our %actions;
    our @regression_tests;

    my $actionfiles_ref = shift;
    my @actionfiles = @{$actionfiles_ref};
    my $number;

    my $si = 0;  # Section index
    my $ri = -1; # Regression test index
    my $number_of_regression_tests = 0;

    my $ignored = 0;

    l(LL_STATUS, "Loading regression tests from action file(s) delivered by Privoxy.");

    for my $file_number (0 .. @actionfiles - 1) {

        my $curl_url = ' "' . $actionfiles[$file_number] . '"';
        my $actionfile = undef;

        foreach (@{get_cgi_page_or_else($curl_url)}) {

            my $no_checks = 0;
            chomp;
            
            if (/<h2>Contents of Actions File (.*?)</) {
                $actionfile = $1;
                next;
            }
            next unless defined $actionfile;

            last if (/<\/pre>/);

            my ($token, $value) = tokenize($_);

            next unless defined $token;

            # Load regression tests

            if (token_starts_new_test($token)) {

                # Beginning of new regression test.
                $ri++;
                $number_of_regression_tests++;

                l(LL_FILE_LOADING, "Regression test " . $number_of_regression_tests . " (section:" . $si . "):");

                if ($token eq 'set header') {

                    l(LL_FILE_LOADING, "Header to set: " . $value);
                    $regression_tests[$si][$ri]{'type'} = CLIENT_HEADER_TEST;
                    # Implicit default
                    $regression_tests[$si][$ri]{'level'} = CLIENT_HEADER_TEST;

                } elsif ($token eq 'request header') {

                    l(LL_FILE_LOADING, "Header to request: " . $value);
                    $regression_tests[$si][$ri]{'type'} = SERVER_HEADER_TEST;
                    # Implicit default
                    $regression_tests[$si][$ri]{'expected-status-code'} = 200;
                    $regression_tests[$si][$ri]{'level'} = SERVER_HEADER_TEST;

                } elsif ($token eq 'trusted cgi request') {

                    l(LL_FILE_LOADING, "CGI URL to test in a dumb way: " . $value);
                    $regression_tests[$si][$ri]{'type'} = TRUSTED_CGI_REQUEST;
                    # Implicit default
                    $regression_tests[$si][$ri]{'expected-status-code'} = 200;
                    $regression_tests[$si][$ri]{'level'} = TRUSTED_CGI_REQUEST;

                } elsif ($token eq 'fetch test') {

                    l(LL_FILE_LOADING, "URL to test in a dumb way: " . $value);
                    $regression_tests[$si][$ri]{'type'} = DUMB_FETCH_TEST;
                    # Implicit default
                    $regression_tests[$si][$ri]{'expected-status-code'} = 200;
                    $regression_tests[$si][$ri]{'level'} = DUMB_FETCH_TEST;

                } elsif ($token eq 'method test') {

                    l(LL_FILE_LOADING, "Method to test: " . $value);
                    $regression_tests[$si][$ri]{'type'} = METHOD_TEST;
                    # Implicit default
                    $regression_tests[$si][$ri]{'expected-status-code'} = 200;
                    $regression_tests[$si][$ri]{'level'} = METHOD_TEST;

                } else {

                    die "Incomplete '" . $token . "' support detected."; 

                }

                check_for_forbidden_characters($value);

                $regression_tests[$si][$ri]{'data'} = $value;

                # For function that only get passed single tests
                $regression_tests[$si][$ri]{'section-id'} = $si;
                $regression_tests[$si][$ri]{'regression-test-id'} = $ri;
                $regression_tests[$si][$ri]{'file'} = $actionfile;

            }
            
            if ($si == -1 || $ri == -1) {
                # No beginning of a test detected yet,
                # so we don't care about any other test
                # attributes.
                next;
            }

            if ($token eq 'expect header') {

                l(LL_FILE_LOADING, "Detected expectation: " . $value);
                $regression_tests[$si][$ri]{'expect-header'} = $value;

            } elsif ($token eq 'tag') {
                
                next if ($ri == -1);

                my $tag = parse_tag($value);

                # We already checked in parse_tag() after filtering
                $no_checks = 1;

                l(LL_FILE_LOADING, "Detected TAG: " . $tag);

                # Save tag for all tests in this section
                do {
                    $regression_tests[$si][$ri]{'tag'} = $tag; 
                } while ($ri-- > 0);

                $si++;
                $ri = -1;

            } elsif ($token eq 'ignore' && $value =~ /Yes/i) {

                l(LL_FILE_LOADING, "Ignoring section: " . test_content_as_string($regression_tests[$si][$ri]));
                $regression_tests[$si][$ri]{'ignore'} = 1;
                $ignored++;

            } elsif ($token eq 'expect status code') {

                l(LL_FILE_LOADING, "Expecting status code: " . $value);
                $regression_tests[$si][$ri]{'expected-status-code'} = $value;

            } elsif ($token eq 'level') { # XXX: stupid name

                $value =~ s@(\d+).*@$1@;
                l(LL_FILE_LOADING, "Level: " . $value);
                $regression_tests[$si][$ri]{'level'} = $value;

            } elsif ($token eq 'method') {

                l(LL_FILE_LOADING, "Method: " . $value);
                $regression_tests[$si][$ri]{'method'} = $value;
            } else {
                # We don't use it, so we don't need
                $no_checks = 1;
            }
            # XXX: Neccessary?
            check_for_forbidden_characters($value) unless $no_checks;
            check_for_forbidden_characters($token);
        }
    }

    l(LL_FILE_LOADING, "Done loading " . $number_of_regression_tests . " regression tests." 
      . " Of which " . $ignored. " will be ignored)\n");
}

############################################################################
#
# Regression test executing functions
#
############################################################################

sub execute_regression_tests () {

    our @regression_tests;
    my $loops = get_cli_option('loops');
    my $all_tests    = 0;
    my $all_failures = 0;
    my $all_successes = 0;

    unless (@regression_tests) {

        l(LL_STATUS, "No regression tests found.");
        return;
    }

    l(LL_STATUS, "Executing regression tests ...");

    while ($loops-- > 0) {

        my $successes = 0;
        my $tests = 0;
        my $failures;
        my $skipped = 0;
        my $test_number = 0;

        for my $s (0 .. @regression_tests - 1) {

            my $r = 0;

            while (defined $regression_tests[$s][$r]) {

                die "Section id mismatch" if ($s != $regression_tests[$s][$r]{'section-id'});
                die "Regression test id mismatch" if ($r != $regression_tests[$s][$r]{'regression-test-id'});

                if ($regression_tests[$s][$r]{'ignore'}
                    or level_is_unacceptable($regression_tests[$s][$r]{'level'})
                    or test_number_is_unacceptable($test_number)) {

                    $skipped++;

                } else {

                    my $result = execute_regression_test($regression_tests[$s][$r]);

                    log_result($regression_tests[$s][$r], $result, $tests);

                    $successes += $result;
                    $tests++;
                }
                $r++;
                $test_number++;
            }
        }
        $failures = $tests - $successes;

        log_message("Executed " . $tests . " regression tests. " .
            'Skipped ' . $skipped . '. ' . 
            $successes . " successes, " . $failures . " failures.");

        $all_tests    += $tests;
        $all_failures += $failures;
        $all_successes += $successes;

    }


    if (get_cli_option('loops') > 1) {
        log_message("Total: Executed " . $all_tests . " regression tests. " .
            $all_successes . " successes, " . $all_failures . " failures.");
    }
}

sub level_is_unacceptable ($) {
    my $level = shift;
    return ((cli_option_is_set('level') and get_cli_option('level') != $level)
            or ($level < get_cli_option('min-level'))
            or ($level > get_cli_option('max-level'))
            );
}

sub test_number_is_unacceptable ($) {
    my $test_number = shift;
    return (cli_option_is_set('test-number')
            and get_cli_option('test-number') != $test_number)
}


# XXX: somewhat misleading name
sub execute_regression_test ($) {

    my $test_ref = shift;
    my %test = %{$test_ref};
    my $result = 0;

    if ($test{'type'} == CLIENT_HEADER_TEST) {

        $result = execute_client_header_regression_test($test_ref);

    } elsif ($test{'type'} == SERVER_HEADER_TEST) {

        $result = execute_server_header_regression_test($test_ref);

    } elsif ($test{'type'} == DUMB_FETCH_TEST
          or $test{'type'} == TRUSTED_CGI_REQUEST) {

        $result = execute_dumb_fetch_test($test_ref);

    } elsif ($test{'type'} == METHOD_TEST) {

        $result = execute_method_test($test_ref);

    } else {

        die "Unsuported test type detected: " . $test{'type'};

    }


    return $result;
}

sub execute_method_test ($) {

    my $test_ref = shift;
    my %test = %{$test_ref};
    my $buffer_ref;
    my $result = 0;
    my $status_code;
    my $method = $test{'data'};

    my $curl_parameters = '';
    my $expected_status_code = $test{'expected-status-code'};

    if ($method =~ /HEAD/i) {

        $curl_parameters .= '--head ';

    } else {

        $curl_parameters .= '-X ' . $method . ' ';
    }

    $curl_parameters .= PRIVOXY_CGI_URL;

    $buffer_ref = get_page_with_curl($curl_parameters);
    $status_code = get_status_code($buffer_ref);

    $result = check_status_code_result($status_code, $expected_status_code);

    return $result;
}


sub execute_dumb_fetch_test ($) {

    my $test_ref = shift;
    my %test = %{$test_ref};
    my $buffer_ref;
    my $result = 0;
    my $status_code;

    my $curl_parameters = '';
    my $expected_status_code = $test{'expected-status-code'};

    if (defined $test{method}) {
        $curl_parameters .= '-X ' . $test{method} . ' ';
    }
    if ($test{type} == TRUSTED_CGI_REQUEST) {
        $curl_parameters .= '--referer ' . PRIVOXY_CGI_URL . ' ';
    }

    $curl_parameters .= $test{'data'};

    $buffer_ref = get_page_with_curl($curl_parameters);
    $status_code = get_status_code($buffer_ref);

    $result = check_status_code_result($status_code, $expected_status_code);

    return $result;
}

sub check_status_code_result ($$) {

    my $status_code = shift;
    my $expected_status_code = shift;
    my $result = 0;

    if ($expected_status_code == $status_code) {

        $result = 1;
        l(LL_VERBOSE_SUCCESS,
          "Yay. We expected status code " . $expected_status_code . ", and received: " . $status_code . '.');

    } elsif (cli_option_is_set('fuzzer-feeding') and $status_code == 123) {

        l(LL_VERBOSE_FAILURE,
          "Oh well. Status code lost while fuzzing. Can't check if it was " . $expected_status_code . '.');

    } else {

        l(LL_VERBOSE_FAILURE,
          "Ooops. We expected status code " . $expected_status_code . ", but received: " . $status_code . '.');

    }
    
    return $result;
}

sub execute_client_header_regression_test ($) {

    my $test_ref = shift;
    my $buffer_ref;
    my $header;
    my $result = 0;

    $buffer_ref = get_show_request_with_curl($test_ref);

    $header = get_header($buffer_ref, $test_ref);
    $result = check_header_result($test_ref, $header);

    return $result;
}

sub execute_server_header_regression_test ($) {

    my $test_ref = shift;
    my $buffer_ref;
    my $header;
    my $result = 0;

    $buffer_ref = get_head_with_curl($test_ref);

    $header = get_server_header($buffer_ref, $test_ref);
    $result = check_header_result($test_ref, $header);

    return $result;
}


sub interpret_result ($) {
    my $success = shift;
    return $success ? "Success" : "Failure";
}

sub check_header_result ($$) {

    my $test_ref = shift;
    my $header = shift;

    my %test = %{$test_ref};
    my $expect_header = $test{'expect-header'};
    my $success = 0;

    $header =~ s@   @ @g if defined($header);

    if ($expect_header eq 'NO CHANGE') {

        if (defined($header) and $header eq $test{'data'}) {

            $success = 1;

        } else {

            $header //= "REMOVAL";
            l(LL_VERBOSE_FAILURE,
              "Ooops. Got: " . $header . " while expecting: " . $expect_header);
        }

    } elsif ($expect_header eq 'REMOVAL') {

        if (defined($header) and $header eq $test{'data'}) {

            l(LL_VERBOSE_FAILURE,
              "Ooops. Expected removal but: " . $header . " is still there.");

        } else {

            # XXX: Use more reliable check here and make sure
            # the header has a different name.
            $success = 1;

        }

    } elsif ($expect_header eq 'SOME CHANGE') {

        if (defined($header) and not $header eq $test{'data'}) {

            $success = 1;

        } else {

            $header //= "REMOVAL";
            l(LL_VERBOSE_FAILURE,
              "Ooops. Got: " . $header . " while expecting: SOME CHANGE");
        }


    } else {

        if (defined($header) and $header eq $expect_header) {

            $success = 1;

        } else {

            $header //= "'No matching header'"; # XXX: No header detected to be precise
            l(LL_VERBOSE_FAILURE,
              "Ooops. Got: " . $header . " while expecting: " . $expect_header);
        }
    }
    return $success;
}


sub get_header_name ($) {

    my $header = shift;

    $header =~ s@(.*?: ).*@$1@;

    return $header;
}

sub get_header ($$) {

    our $filtered_request = '';

    my $buffer_ref = shift;
    my $test_ref = shift;

    my %test = %{$test_ref};
    my @buffer = @{$buffer_ref};

    my $expect_header = $test{'expect-header'};

    my $line;
    my $processed_request_reached = 0;
    my $read_header = 0;
    my $processed_request = '';
    my $header;
    my $header_to_get;

    if ($expect_header eq 'REMOVAL'
     or $expect_header eq 'NO CHANGE'
     or  $expect_header eq 'SOME CHANGE') {

        $expect_header = $test{'data'};

    }

    $header_to_get = get_header_name($expect_header);

    foreach (@buffer) {

        # Skip everything before the Processed request
        if (/Processed Request/) {
            $processed_request_reached = 1;
            next;
        }
        next unless $processed_request_reached;

        # End loop after the Processed request
        last if (/<\/pre>/);

        # Ditch tags and leading/trailing white space.
        s@^\s*<.*?>@@g;
        s@\s*$@@g;

        $filtered_request .=  "\n" . $_;
         
        if (/^$header_to_get/) {
            $read_header = 1;
            $header = $_;
            last;
        }
    }

    return $header;
}

sub get_server_header ($$) {

    my $buffer_ref = shift;
    my $test_ref = shift;

    my %test = %{$test_ref};
    my @buffer = @{$buffer_ref};

    my $expect_header = $test{'expect-header'};
    my $header;
    my $header_to_get;

    if ($expect_header eq 'REMOVAL'
     or $expect_header eq 'NO CHANGE'
     or  $expect_header eq 'SOME CHANGE') {

        $expect_header = $test{'data'};

    }

    $header_to_get = get_header_name($expect_header);

    foreach (@buffer) {

        # XXX: shoul probably verify that the request
        # was actually answered by Fellatio.
        if (/^$header_to_get/) {
            $header = $_;
            $header =~ s@\s*$@@g;
            last;
        }
    }

    return $header;
}

sub get_header_to_check ($) {

    # No longer in use but not removed yet.

    my $buffer_ref = shift;
    my $header;
    my @buffer = @{$buffer_ref}; 
    my $line;
    my $processed_request_reached = 0;
    my $read_header = 0;
    my $processed_request = '';

    l(LL_ERROR, "You are not supposed to use get_header_to_()!");

    foreach (@buffer) {

        # Skip everything before the Processed request
        if (/Processed Request/) {
            $processed_request_reached = 1;
            next;
        }
        next unless $processed_request_reached;

        # End loop after the Processed request
        last if (/<\/pre>/);

        # Ditch tags and leading/trailing white space.
        s@^\s*<.*?>@@g;
        s@\s*$@@g;

        $processed_request .= $_;
         
        if (/^X-Privoxy-Regression-Test/) {
            $read_header = 1;
            next;
        }

        if ($read_header) {
            $header = $_;
            $read_header = 0;
        }

    }

    return $header;
}

sub get_status_code ($) {

    my $buffer_ref = shift;
    my @buffer = @{$buffer_ref}; 

    foreach (@buffer) {

        if (/^HTTP\/\d\.\d (\d{3})/) {

            return $1;

        } else {

            return '123' if cli_option_is_set('fuzzer-feeding');
            chomp;
            l(LL_ERROR, 'Unexpected buffer line: "' . $_ . '"');
        }
    }
}

sub get_test_keys () {
    return ('tag', 'data', 'expect-header', 'ignore');
}

# XXX: incomplete
sub test_content_as_string ($) {

    my $test_ref = shift;
    my %test = %{$test_ref};

    my $s = "\n\t";

    foreach my $key (get_test_keys()) {
        #$test{$key} = $test{$key} // 'undefined';
        $test{$key} = 'Not set' unless (defined $test{$key});
    }

    $s .= 'Tag: ' . $test{'tag'};
    $s .= "\n\t";
    $s .= 'Set header: ' . $test{'data'}; # XXX: adjust for other test types
    $s .= "\n\t";
    $s .= 'Expected header: ' . $test{'expect-header'};
    $s .= "\n\t";
    $s .= 'Ignore: ' . $test{'ignore'};

    return $s;
}

############################################################################
#
# HTTP fetch functions
#
############################################################################

sub check_for_curl () {
    my $curl = CURL;
    l(LL_ERROR, "No curl found.") unless (`which $curl`);
}

sub get_cgi_page_or_else ($) {

    my $cgi_url = shift;
    my $content_ref = get_page_with_curl($cgi_url);
    my $status_code = get_status_code($content_ref);

    if (200 != $status_code) {

        my $log_message = "Failed to fetch Privoxy CGI Page. " .
                          "Received status code ". $status_code .
                          " while only 200 is acceptable.";

        if (cli_option_is_set('fuzzer-feeding')) {

            $log_message .= " Ignored due to fuzzer feeding.";
            l(LL_SOFT_ERROR, $log_message)

        } else {

            l(LL_ERROR, $log_message);

        }
    }
    
    return $content_ref;
}

sub get_show_request_with_curl ($) {

    our $privoxy_cgi_url;
    my $test_ref = shift;
    my %test = %{$test_ref};

    my $curl_parameters = ' ';

    # Enable the action to test
    $curl_parameters .= '-H \'X-Privoxy-Control: ' . $test{'tag'} . '\' ';
    # The header to filter
    $curl_parameters .= '-H \'' . $test{'data'} . '\' ';

    $curl_parameters .= ' ';
    $curl_parameters .= $privoxy_cgi_url;
    $curl_parameters .= 'show-request';

    return get_cgi_page_or_else($curl_parameters);
}


sub get_head_with_curl ($) {

    our $fellatio_url = FELLATIO_URL;
    my $test_ref = shift;
    my %test = %{$test_ref};

    my $curl_parameters = ' ';

    # Enable the action to test
    $curl_parameters .= '-H \'X-Privoxy-Control: ' . $test{'tag'} . '\' ';
    # The header to filter
    $curl_parameters .= '-H \'X-Gimme-Head-With: ' . $test{'data'} . '\' ';
    $curl_parameters .= '--head ';

    $curl_parameters .= ' ';
    $curl_parameters .= $fellatio_url;

    return get_page_with_curl($curl_parameters);
}


sub get_page_with_curl ($) {

    my $parameters = shift;
    my @buffer;
    my $curl_line = CURL;
    my $retries_left = get_cli_option('retries') + 1;
    my $failure_reason;

    if (cli_option_is_set('privoxy-address')) {
        $curl_line .= ' --proxy ' . get_cli_option('privoxy-address');
    }

    # Let Privoxy emit two log messages less.
    $curl_line .= ' -H \'Proxy-Connection:\' ' unless $parameters =~ /Proxy-Connection:/;
    $curl_line .= ' -H \'Connection: close\' ' unless $parameters =~ /Connection:/;
    # We don't care about fetch statistic.
    $curl_line .= " -s ";
    # We do care about the failure reason if any.
    $curl_line .= " -S ";
    # We want to see the HTTP status code
    $curl_line .= " --include ";
    # We want to advertise ourselves
    $curl_line .= " --user-agent '" . PRT_VERSION . "' ";
    # We aren't too patient
    $curl_line .= " --max-time '" . get_cli_option('max-time') . "' ";

    $curl_line .= $parameters;
    # XXX: still necessary?
    $curl_line .= ' 2>&1';

    l(LL_PAGE_FETCHING, "Executing: " . $curl_line);

    do {
        @buffer = `$curl_line`;

        if ($?) {
            $failure_reason = array_as_string(\@buffer);
            chomp $failure_reason;
            l(LL_SOFT_ERROR, "Fetch failure: '" . $failure_reason . $! ."'");
        }
    } while ($? && --$retries_left);

    unless ($retries_left) {
        l(LL_ERROR,
          "Running curl failed " . get_cli_option('retries') .
          " times in a row. Last error: '" . $failure_reason . "'.");
    }

    return \@buffer;
}


############################################################################
#
# Log functions
#
############################################################################

sub array_as_string ($) {
    my $array_ref = shift;
    my $string = '';

    foreach (@{$array_ref}) {
        $string .= $_;
    }

    return $string;
}


sub show_test ($) {
    my $test_ref = shift;
    log_message('Test is:' . test_content_as_string($test_ref));
}

# Conditional log
sub l ($$) {
    our $log_level;
    my $this_level = shift;
    my $message = shift;

    return unless ($log_level & $this_level);

    if (LL_ERROR & $this_level) {
        $message = 'Oh noes. ' . $message . ' Fatal error. Exiting.';
    }

    log_message($message);

    if (LL_ERROR & $this_level) {
        exit;
    }
}

sub log_message ($) {

    my $message = shift;

    our $logfile;
    our $no_logging;
    our $leading_log_date;
    our $leading_log_time;

    my $time_stamp = '';
    my ( $sec, $min, $hour, $mday, $mon, $year, $wday, $yday, $isdst ) = localtime time;

    if ($leading_log_date || $leading_log_time) {

        if ($leading_log_date) {
            $year += 1900;
            $mon  += 1;
            $time_stamp = sprintf("%i/%.2i/%.2i", $year, $mon, $mday);
        }

        if ($leading_log_time) {
            $time_stamp .= ' ' if $leading_log_date;
            $time_stamp.= sprintf("%.2i:%.2i:%.2i", $hour, $min, $sec);
        }
        
        $message = $time_stamp . ": " . $message;
    }


    printf(STDERR "%s\n", $message);

}

sub log_result ($$) {

    our $verbose_test_description;
    our $filtered_request;

    my $test_ref = shift;
    my $result = shift;
    my $number = shift;

    my %test = %{$test_ref};
    my $message = '';

    $message .= interpret_result($result);
    $message .= " for test ";
    $message .= $number;
    $message .= '/';
    $message .= $test{'section-id'};
    $message .= '/';
    $message .= $test{'regression-test-id'};
    $message .= '.';

    if ($verbose_test_description) {

        if ($test{'type'} == CLIENT_HEADER_TEST) {

            $message .= ' Header ';
            $message .= quote($test{'data'});
            $message .= ' and tag ';
            $message .= quote($test{'tag'});

        } elsif ($test{'type'} == SERVER_HEADER_TEST) {

            $message .= ' Request Header ';
            $message .= quote($test{'data'});
            $message .= ' and tag ';
            $message .= quote($test{'tag'});

        } elsif ($test{'type'} == DUMB_FETCH_TEST) {

            $message .= ' URL ';
            $message .= quote($test{'data'});
            $message .= ' and expected status code ';
            $message .= quote($test{'expected-status-code'});

        } elsif ($test{'type'} == TRUSTED_CGI_REQUEST) {

            $message .= ' CGI URL ';
            $message .= quote($test{'data'});
            $message .= ' and expected status code ';
            $message .= quote($test{'expected-status-code'});

        } elsif ($test{'type'} == METHOD_TEST) {

            $message .= ' HTTP method ';
            $message .= quote($test{'data'});
            $message .= ' and expected status code ';
            $message .= quote($test{'expected-status-code'});

        } else {

            die "Incomplete support for test type " . $test{'type'} .  " detected.";

        }
    }

    log_message($message) unless ($result && cli_option_is_set('silent'));
}

sub quote ($) {
    my $s = shift;
    return '\'' . $s . '\'';
}

sub print_version () {
    printf PRT_VERSION . "\n" . 'Copyright (C) 2007-2008 Fabian Keil <fk@fabiankeil.de>' . "\n";
}

sub help () {

    our %cli_options;

    print_version();

    print << "    EOF"

Options and their default values if they have any:
    [--debug $cli_options{'debug'}]
    [--fuzzer-feeding]
    [--help]
    [--level]
    [--loops $cli_options{'loops'}]
    [--max-level $cli_options{'max-level'}]
    [--max-time $cli_options{'max-time'}]
    [--min-level $cli_options{'min-level'}]
    [--privoxy-address]
    [--retries $cli_options{'retries'}]
    [--silent]
    [--version]
see "perldoc $0" for more information
    EOF
    ;
    exit(0);
}

sub init_cli_options () {

    our %cli_options;
    our $log_level;

    $cli_options{'min-level'} = CLI_MIN_LEVEL;
    $cli_options{'max-level'} = CLI_MAX_LEVEL;
    $cli_options{'debug'}  = $log_level;
    $cli_options{'loops'}  = CLI_LOOPS;
    $cli_options{'max-time'}  = CLI_MAX_TIME;
    $cli_options{'retries'}  = CLI_RETRIES;
}

sub parse_cli_options () {

    our %cli_options;
    our $log_level;

    init_cli_options();

    GetOptions (
                'debug=s' => \$cli_options{'debug'},
                'help'     => sub { help },
                'silent' => \$cli_options{'silent'},
                'min-level=s' => \$cli_options{'min-level'},
                'max-level=s' => \$cli_options{'max-level'},
                'privoxy-address=s' => \$cli_options{'privoxy-address'},
                'level=s' => \$cli_options{'level'},
                'loops=s' => \$cli_options{'loops'},
                'test-number=s' => \$cli_options{'test-number'},
                'fuzzer-feeding' => \$cli_options{'fuzzer-feeding'},
                'retries=s' => \$cli_options{'retries'},
                'max-time=s' => \$cli_options{'max-time'},
                'version'  => sub { print_version && exit(0) }
    );
    $log_level |= $cli_options{'debug'};
}

sub cli_option_is_set ($) {

    our %cli_options;
    my $cli_option = shift;

    return defined $cli_options{$cli_option};
}


sub get_cli_option ($) {

    our %cli_options;
    my $cli_option = shift;

    die "Unknown CLI option: $cli_option" unless defined $cli_options{$cli_option};

    return $cli_options{$cli_option};
}


sub main () {

    init_our_variables();
    parse_cli_options();
    check_for_curl();
    load_regressions_tests();
    execute_regression_tests();
}

main();

=head1 NAME

B<privoxy-regression-test> - A regression test "framework" for Privoxy.

=head1 SYNOPSIS

B<privoxy-regression-test> [B<--debug bitmask>] [B<--fuzzer-feeding>] [B<--help>]
[B<--level level>] [B<--loops count>] [B<--max-level max-level>]
[B<--max-time max-time>] [B<--min-level min-level>] B<--privoxy-address proxy-address>
[B<--retries retries>] [B<--silent>] [B<--version>]

=head1 DESCRIPTION

Privoxy-Regression-Test is supposed to one day become
a regression test suite for Privoxy. It's not quite there
yet, however, and can currently only test client header
actions and check the returned status code for requests
to arbitrary URLs.

Client header actions are tested by requesting
B<http://config.privoxy.org/show-request> and checking whether
or not Privoxy modified the original request as expected.

The original request contains both the header the action-to-be-tested
acts upon and an additional tagger-triggering header that enables
the action to test.

=head1 CONFIGURATION FILE SYNTAX

Privoxy-Regression-Test's configuration is embedded in
Privoxy action files and loaded through Privoxy's web interface.

It makes testing a Privoxy version running on a remote system easier
and should prevent you from updating your tests without updating Privoxy's
configuration accordingly.

A client-header-action test section looks like this:

    # Set Header    = Referer: http://www.example.org.zwiebelsuppe.exit/
    # Expect Header = Referer: http://www.example.org/
    {+client-header-filter{hide-tor-exit-notation} -hide-referer}
    TAG:^client-header-filter\{hide-tor-exit-notation\}$

The example above causes Privoxy-Regression-Test to set
the header B<Referer: http://www.example.org.zwiebelsuppe.exit/>
and to expect it to be modified to
B<Referer: http://www.example.org/>.

When testing this section, Privoxy-Regression-Test will set the header
B<X-Privoxy-Control: client-header-filter{hide-tor-exit-notation}>
causing the B<privoxy-control> tagger to create the tag
B<client-header-filter{hide-tor-exit-notation}> which will finally
cause Privoxy to enable the action section.

Note that the actions itself are only used by Privoxy,
Privoxy-Regression-Test ignores them and will be happy
as long as the expectations are satisfied.

A fetch test looks like this:

    # Fetch Test = http://p.p/user-manual
    # Expect Status Code = 302

It tells Privoxy-Regression-Test to request B<http://p.p/user-manual>
and to expect a response with the HTTP status code B<302>. Obviously that's
not a very thorough test and mainly useful to get some code coverage
for Valgrind or to verify that the templates are installed correctly.

If you want to test CGI pages that require a trusted
referer, you can use:

    # Trusted CGI Request =  http://p.p/edit-actions

It works like ordinary fetch tests, but sets the referer
header to a trusted value.

If no explicit status code expectation is set, B<200> is used.

Additionally all tests have test levels to let the user
control which ones to execute (see I<OPTIONS> below). 
Test levels are either set with the B<Level> directive,
or implicitly through the test type.

Fetch tests default to level 6, tests for trusted
CGI requests to level 3 and client-header-action tests
to level 1.

=head1 OPTIONS

B<--debug bitmask> Add the bitmask provided as integer
to the debug settings.

B<--fuzzer-feeding> Ignore some errors that would otherwise
cause Privoxy-Regression-Test to abort the test because
they shouldn't happen in normal operation. This option is
intended to be used if Privoxy-Regression-Test is only
used to feed a fuzzer in which case there's a high chance
that Privoxy gets an invalid request and returns an error
message.

B<--help> Shows available command line options.

B<--level level> Only execute tests with the specified B<level>. 

B<--loop count> Loop through the regression tests B<count> times. 
Useful to feed a fuzzer, or when doing stress tests with
several Privoxy-Regression-Test instances running at the same
time.

B<--max-level max-level> Only execute tests with a B<level>
below or equal to the numerical B<max-level>.

B<--max-time max-time> Give Privoxy B<max-time> seconds
to return data. Increasing the default may make sense when
Privoxy is run through Valgrind, decreasing the default may
make sense when Privoxy-Regression-Test is used to feed
a fuzzer.

B<--min-level min-level> Only execute tests with a B<level>
above or equal to the numerical B<min-level>.

B<--privoxy-address proxy-address> Privoxy's listening address.
If it's not set, the value of the environment variable http_proxy
will be used. B<proxy-address> has to be specified in http_proxy
syntax.

B<--retries retries> Retry B<retries> times.

B<--silent> Don't log succesful test runs.

B<--version> Print version and exit.

The second dash is optional, options can be shortened,
as long as there are no ambiguities.

=head1 PRIVOXY CONFIGURATION

Privoxy-Regression-Test is shipped with B<regression-tests.action>
which aims to test all official client-header modifying actions
and can be used to verify that the templates and the user manual
files are installed correctly.

To use it, it has to be copied in Privoxy's configuration
directory, and afterwards referenced in Privoxy's configuration
file with the line:

    actionsfile regression-tests.action

In general, its tests are supposed to work without changing
any other action files, unless you already added lots of
taggers yourself. If you are using taggers that cause problems,
you might have to temporary disable them for Privoxy's CGI pages.

Some of the regression tests rely on Privoxy features that
may be disabled in your configuration. Tests with a level below
7 are supposed to work with all Privoxy configurations (provided
you didn't build with FEATURE_GRACEFUL_TERMINATION).

Tests with level 9 require Privoxy to deliver the User Manual,
tests with level 12 require the CGI editor to be enabled.

=head1 CAVEATS

Expect the configuration file syntax to change with future releases.

=head1 LIMITATIONS

As Privoxy's B<show-request> page only shows client headers,
Privoxy-Regression-Test can't use it to test Privoxy actions
that modify server headers.

As Privoxy-Regression-Test relies on Privoxy's tag feature to
control the actions to test, it currently only works with
Privoxy 3.0.7 or later.

At the moment Privoxy-Regression-Test fetches Privoxy's
configuration page through I<curl>(1), therefore you have to
have I<curl> installed, otherwise you won't be able to run
Privoxy-Regression-Test in a meaningful way.

=head1 SEE ALSO

privoxy(1) curl(1)

=head1 AUTHOR

Fabian Keil <fk@fabiankeil.de>

=cut
