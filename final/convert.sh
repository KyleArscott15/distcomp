#!/bin/bash

cd images
for i in *.bmp
do
	newname="${i%.*}"
	convert $i $newname.jpg
	rm -f $i
done
