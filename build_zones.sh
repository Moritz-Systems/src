#!/bin/sh

#./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j8 -U -u -O /public/netbsd-zones tools
./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j1 -U -u -O /public/netbsd-zones kernel=GENERIC
