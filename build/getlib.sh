#!/bin/sh

TAR=/bin/tar
ZCAT=/bin/gunzip
WGET=/usr/bin/wget
CURL=

if [ -f "$WGET" ] ; then
    DOWNLOAD_CMD=$WGET
else
    if [ -f "$CURL" ] ; then
        DOWNLOAD_CMD="$CURL -O"
    fi
fi

base=http://files.freeswitch.org/downloads/libs/
tarfile=$1
url=`echo $tarfile | grep "://"`

if [ ! -z $url ] ; then
    base=$tarfile/
    tarfile=$2
fi

if [ ! -d $tarfile ] ; then
    uncompressed=`echo $tarfile | sed "s/\.tar\.gz//g"`
    uncompressed=`echo $uncompressed | sed "s/\.tgz//g"`

    if [ ! -f $tarfile ] ; then
	rm -fr $uncompressed
	$DOWNLOAD_CMD $base$tarfile
	if [ ! -f $tarfile ] ; then
	    echo cannot find $tarfile
	    exit 1
	fi
    fi
    if [ ! -d $uncompressed ] ; then
	$ZCAT -c -d $tarfile | $TAR xf -
    fi
fi

exit 0

