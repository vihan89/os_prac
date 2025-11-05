#!/bin/bash

test_exists() {
    echo "Filename: "; read file
    if [ -e "$file" ]; then
        echo "EXISTS"
        [ -r "$file" ] && echo "Readable" || echo "Not readable"
        [ -w "$file" ] && echo "Writable" || echo "Not writable"
        [ -x "$file" ] && echo "Executable" || echo "Not executable"
    else
        echo "Does not exist"
    fi
}

read_file() {
    echo "Filename: "; read file
    if [ ! -f "$file" ]; then
        echo "Not a file or doesn't exist"
    elif [ ! -r "$file" ]; then
        echo "Not readable"
    elif [ ! -s "$file" ]; then
        echo "Empty file"
    else
        cat "$file"
        echo "Lines: $(wc -l < "$file")"
    fi
}

delete_file() {
    echo "Filename: "; read file
    if [ ! -f "$file" ]; then
        echo "Not a file"
    else
        echo "Delete '$file'? (y/n): "; read confirm
        [ "$confirm" = "y" ] && rm "$file" && echo "Deleted" || echo "Cancelled"
    fi
}

list_files() {
    echo "1.Current dir 2.Specific dir 3.Pattern"
    read choice
    case $choice in
        1) ls -lh | grep "^-" ;;
        2) echo "Path: "; read dir; [ -d "$dir" ] && ls -lh "$dir" | grep "^-" || echo "Invalid dir" ;;
        3) echo "Pattern: "; read pat; ls $pat 2>/dev/null || echo "No match" ;;
    esac
}

create_file() {
    echo "Filename: "; read file
    if [ -e "$file" ]; then
        echo "Exists. Overwrite? (y/n): "; read over
        [ "$over" != "y" ] && echo "Cancelled" && return
    fi
    echo "Content (EOF to end):"
    > "$file"
    while read line; do
        [ "$line" = "EOF" ] && break
        echo "$line" >> "$file"
    done
    echo "Created"
}

file_info() {
    echo "Filename: "; read file
    [ ! -e "$file" ] && echo "Not found" && return
    ls -lh "$file"
    [ -f "$file" ] && [ -s "$file" ] && echo "Lines: $(wc -l < "$file"), Words: $(wc -w < "$file")"
}

while true; do
    echo -e "\n1.Test 2.Read 3.Delete 4.List 5.Create 6.Info 7.Quit"
    read choice
    case $choice in
        1) test_exists ;;
        2) read_file ;;
        3) delete_file ;;
        4) list_files ;;
        5) create_file ;;
        6) file_info ;;
        7) exit 0 ;;
    esac
done