#!/bin/sh

#rm -rf /public/netbsd-zones
#./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j2 -U -u -O /public/netbsd-zones tools
./build.sh -V MAKECONF=/dev/null -V MKLLVMRT=no -N0 -j1 -U -u -O /public/netbsd-zones kernel=GENERIC
