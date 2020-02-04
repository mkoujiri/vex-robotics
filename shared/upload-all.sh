#!/bin/bash

slot=1
for d in projects/*/ ; do
	prosv5 upload --project=$d --slot=$slot
	echo "uploading to slot:$slot  $d"
	((slot+=1))
done
