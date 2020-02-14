#!/bin/bash

for d in projects/*/ ; do
	echo "Compiling @ $d"
	cd $d
	make -j clean
	make -j
	cd ..
	cd ..
done
