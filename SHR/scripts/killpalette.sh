#!/bin/sh
# killpalette.sh - house-cleaning script
# removes external segmented palette files left-over from deleted images and related output
# then removes the associated palette directory
# if the done directory does not contain a processed bmp, assumes the palettes are no longer needed
cd sh0pcx
for i in $(ls); do 
   tgt=$(basename $i)
   if [ ! -e ../done/$tgt.bmp ]; then
       if [ -e ./$tgt/0.pcx ]; then
           rm ./$tgt/*.pcx
       fi   
       rmdir ./$tgt
   fi
   done
cd ..   
cd sh2pcx
for i in $(ls); do 
   tgt=$(basename $i)
   if [ ! -e ../done/$tgt.bmp ]; then
       if [ -e ./$tgt/0.pcx ]; then
           rm ./$tgt/*.pcx
       fi   
       rmdir ./$tgt
   fi
   done
cd ..   
cd sh3pcx
for i in $(ls); do 
   tgt=$(basename $i)
   if [ ! -e ../done/$tgt.bmp ]; then
       if [ -e ./$tgt/0.pcx ]; then
           rm ./$tgt/*.pcx
       fi   
       rmdir ./$tgt
   fi
   done
cd ..   
# this is the end
