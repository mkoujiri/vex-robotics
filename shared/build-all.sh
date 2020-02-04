#!/bin/bash

for d in projects/*/ ; do
	echo "Compiling @ $d"
	cd $d
	make -j
	cd ..
	cd ..
done
