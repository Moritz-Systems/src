#!/bin/sh

#rm -rf /public/netbsd-zones
#./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j8 -U -u -O /public/netbsd-zones tools
./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j8 -U -u -O /public/netbsd-zones kernel=GENERIC
