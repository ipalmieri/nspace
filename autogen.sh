#! /bin/sh

#unamestr=`uname`
#
#if [ $unamestr = "OpenBSD" ] ; then
#
#        echo Configuring for $unamestr
#
#        export CXX=eg++
#        export AUTOMAKE_VERSION=1.9
#        export AUTOCONF_VERSION=2.69
#fi

autoreconf --install || exit 1
