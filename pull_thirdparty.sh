#!/bin/bash

this_folder=$(pwd)

for i in ${this_folder}/thirdparty/*
do
	if [ -d $i ]
	then
		echo $i
		cd $i
		git pull
	fi
done

