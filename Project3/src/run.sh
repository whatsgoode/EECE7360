#!/bin/bash

for file in ../../instances/*
do
	./p3 "$file"
done 
