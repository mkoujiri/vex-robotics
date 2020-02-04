#!/bin/bash

if [ -z $1 ]
then
	echo "You must supply project name"
else
	cp -rf projects/post-state-code projects/$1 
	sed -i "s/post-state-code/$1/" -- "projects/$1/project.pros"
fi
