#!/bin/bash

if [ "$#" -ne 2 ]; then
  echo "Usage: $0 in.txt out.txt"
  exit 1
fi

in="$1"
out="$2"

if [ ! -f "$in" ]; then
  echo "Input file not found: $in"
  exit 1
fi

cat "$1" | sed -E ':a; N; $!ba; s/(^|[.?!]\s*)([a-z])/\1\U\2/g' > "$2"