#!/usr/bin/perl -w
#
# Script to upload compiled Python file to Telit GM862
#
# Copyright (c) 2010 John Graham-Cumming
#
# Usage: upload.pl <file> (<enable>)
#
# Uploads the file to the GM862.  If enable is 1 then enables it for
# execution.

use strict;
use warnings;

use Device::SerialPort;

die "Need at least a filename\n" if ( $#ARGV < 0 );
die "Can't find file $ARGV[0]\n" if ( !-e $ARGV[0] );

# The device that is talking to the GM862 when connected via USB
my $gm862 = '/dev/tty.usbserial-A800crM0';

# Open the GM862 device, make sure we can talk to it by asking it to
# identify itself, if so then upload the file

my $rc = 0;
my $eol = "\015";
my $port = Device::SerialPort->new($gm862);

die "Can't access GM862 module\n" if ( !defined( $port ) );

$port->databits(8);
$port->baudrate(115200);
$port->parity("none");
$port->stopbits(1);
$port->handshake("rts");

my $oke = qr/OK|ERROR/;

my $r = cmd( $port, 'AT+GMI', $oke );
die "Failed to get manufacturer ID from GM862\n" if ( $r !~ /Telit/ );

my $file = `cat $ARGV[0]`;
my $size = length($file);

$r = cmd( $port, "AT#WSCRIPT=\"$ARGV[0]\",$size,0", qr/>>>/ );
die "Didn't get the upload prompt\n" if ( $r !~ />>>/ );

$r = cmd( $port, $file, $oke, 1 );
die "Didn't get OK to upload file\n" if ( $r !~ /OK/ );

if ( defined($ARGV[1]) && ( $ARGV[1] eq '1' ) ) {
  $r = cmd( $port, "AT#ESCRIPT=\"$ARGV[0]\"", $oke );
  die "Didn't get OK to uploaded file\n" if ( $r !~ /OK/ );
}

$r = cmd( $port, "AT#LSCRIPT", $oke );

if ( $r !~ /#LSCRIPT: \"$ARGV[0]\",$size/ ) {
  die "Failed to verify that script was uploaded\n";
}

exit 0;

sub cmd
{
  my ( $p, $c, $re, $raw ) = @_;

  if ( !defined( $raw ) ) {
    $raw = 0;
  }

  if ( !$raw ) {
    print "-> $c\n";
    $c .= $eol;
  } else {
    print "-> ", length($c), " bytes \n";
  }

  # Writes are chunked because was getting failures writing more than
  # 1,024 bytes at a time

  my $chunk = 256;
  my $position = 0;
  my $left = length($c);

  while ( $left > 0 ) {
    my $amt;
    if ( $left < $chunk ) {
      $amt = $left;
    } else {
      $amt = $chunk;
    }

    my $wc = $p->write(substr($c,$position,$amt));
    if ( !defined( $wc ) || ( $wc != $amt ) ) {
      die "Writing to GM862 failed\n";
    }

    $left -= $amt;
    $position += $amt;
  }

  my ($co,$r) = (0,'');
  my $timeout = 30;
  while ( $r !~ $re ) {
    my ( $c0, $r0 ) = $p->read(255);
    if ( $c0 != 0 ) {
      $co += $c0;
      $r  .= $r0;
      my $r1 = $r0;
      $r1 =~ s/[^[:print:]]/./g;
      print "<- $r1\n";
    }
    last if ( $timeout == 0 );
    $timeout--;
    sleep(1);
  }

  return $r;
}
