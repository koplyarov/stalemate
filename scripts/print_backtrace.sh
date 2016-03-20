#!/bin/bash

BINARY="$1"
SYMS=$(nm "$1")

while read DATA; do
	for ADDR in $DATA; do
		IFS=$'\n'
		echo "$SYMS" | grep "$ADDR"
	done
done
