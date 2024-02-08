#!/bin/bash

function print_tree() {
    local directory=$1
    local prefix=$2

    local subdirectories=$(find "$directory" -maxdepth 1 -type d -not -path "$directory")
    local files=$(find "$directory" -maxdepth 1 -type f -not -path "$directory")

    for subdir in $subdirectories; do
        echo "${prefix}|-- $(basename "$subdir (d)")"
        print_tree "$subdir" "${prefix}|   "
    done

    for file in $files; do
        echo "${prefix}|-- $(basename "$file (f)")"
    done
}

if [ $# -ne 1 ]; then
    echo "Usage error, example: $0 <dir>"
    exit 1
fi

directory=$1

if [ ! -d "$directory" ]; then
    echo "'$directory' does not exist."
    exit 1
fi

print_tree "$directory" ""

total=$(find "$directory" | wc -l)
total=$((total - 1))
echo "Total number of files and subdirectories: $total"