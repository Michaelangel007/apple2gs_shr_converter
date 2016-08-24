#!/bin/sh
# kill.sh - house-cleaning script
# cleans unwanted input and output from working directory
# must be run from the command line
# killpalette.sh must also be run after kill.sh to remove unused external palettes
if [ ! -z "$1" ]; then
    find -iname $1* -exec rm {} \;
else 
    echo "no args"
fi
# this is the end
