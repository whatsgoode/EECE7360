#!/bin/bash

for file in ../../instances/*
do
	./p1 "$file" 600
done 
