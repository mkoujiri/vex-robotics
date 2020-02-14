#!/bin/bash

cd projects/post-state-code
make -j clean
make -j
#for d in projects/*/ ; do
#	echo "Compiling @ $d"
#	cd $d
#	make -j clean
#	make -j
#	cd ..
#	cd ..
#done
