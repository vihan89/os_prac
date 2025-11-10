#!/bin/bash

test_exists() {
    echo -n "File: "; read file
    [ -e "$file" ] && echo "EXISTS" || echo "NOT EXISTS"
    [ -r "$file" ] && echo "Readable"
    [ -w "$file" ] && echo "Writable"
    [ -x "$file" ] && echo "Executable"
}

read_file() {
    echo -n "File: "; read file
    [ ! -f "$file" ] && echo "Not a file" && return
    [ ! -s "$file" ] && echo "Empty" && return
    cat "$file"
    wc -l < "$file"
}

delete_file() {
    echo -n "File: "; read file
    rm "$file" 2>/dev/null && echo "Deleted" || echo "Failed"
}

list_files() {
    echo -n "1.Current 2.Path 3.Pattern: "; read ch
    [ "$ch" = 1 ] && ls -lh | grep "^-"
    [ "$ch" = 2 ] && { echo -n "Path: "; read dir; ls -lh "$dir" 2>/dev/null | grep "^-"; }
    [ "$ch" = 3 ] && { echo -n "Pattern: "; read pat; ls $pat 2>/dev/null; }
}

create_file() {
    echo -n "File: "; read file
    echo "Content (EOF to end):"
    > "$file"
    while read line; do
        [ "$line" = "EOF" ] && break
        echo "$line" >> "$file"
    done
    echo "Created"
}

file_info() {
    echo -n "File: "; read file
    [ ! -e "$file" ] && echo "Not found" && return
    ls -lh "$file"
    [ -f "$file" ] && [ -s "$file" ] && echo "Lines: $(wc -l < "$file")"
}

while true; do
    echo -e "\n1.Test 2.Read 3.Delete 4.List 5.Create 6.Info 7.Quit"
    read ch
    [ "$ch" = 1 ] && test_exists
    [ "$ch" = 2 ] && read_file
    [ "$ch" = 3 ] && delete_file
    [ "$ch" = 4 ] && list_files
    [ "$ch" = 5 ] && create_file
    [ "$ch" = 6 ] && file_info
    [ "$ch" = 7 ] && exit
done