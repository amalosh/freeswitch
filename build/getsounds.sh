#!/bin/sh

TAR=/bin/tar
ZCAT=/bin/gunzip
WGET=/usr/bin/wget
CURL=

DIR=`pwd`

if [ -x "$WGET" ] ; then
    DOWNLOAD_CMD=$WGET
else
    if [ -x "$CURL" ] ; then
        DOWNLOAD_CMD="$CURL -O"
    fi
fi

base=http://files.freeswitch.org/
tarfile=$1
install=$2

if [ ! -f $tarfile ] ; then
    $DOWNLOAD_CMD $base$tarfile
    if [ ! -f $tarfile ] ; then
        echo cannot find $tarfile
	exit 1
    fi
fi

if [ ! -z $install ] ; then
   test -d $install || mkdir $install
   cd $install && $ZCAT -c -d $DIR/$tarfile | $TAR xf -
fi

exit 0