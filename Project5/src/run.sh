#!/bin/bash

for file in ../../instances/*
do
	./p5 "$file" 60
done 
