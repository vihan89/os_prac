#!/bin/bash

# 1.6 Write a shell script:
# a) that uses find to look for a file and echo a suitable msg if the file is not found. 
#    You must not store the find output in a file.
# b) that takes a command–line argument and reports on whether it is directory, 
#    a file, or something else.
# c) that accepts one or more file name as arguments and converts all of them to 
#    uppercase, provided they exist in the current directory.

# Function to display menu
display_menu() {
    echo "================================"
    echo "     FILE UTILITIES MENU"
    echo "================================"
    echo "1. Find a file"
    echo "2. Check file type"
    echo "3. Convert filenames to uppercase"
    echo "4. Exit"
    echo "================================"
}

# a) Find a file using find command
find_file() {
    echo "Enter filename to search:"
    read filename
    
    echo "Searching for '$filename'..."
    
    if find . -name "$filename" -print -quit | grep -q .; then
        echo "File found:"
        find . -name "$filename"
    else
        echo "File '$filename' not found!"
    fi
}

# b) Check file type
check_file_type() {
    echo "Enter path to check:"
    read path
    
    if [ -e "$path" ]; then
        if [ -f "$path" ]; then
            echo "'$path' is a regular file"
        elif [ -d "$path" ]; then
            echo "'$path' is a directory"
        elif [ -L "$path" ]; then
            echo "'$path' is a symbolic link"
        elif [ -b "$path" ]; then
            echo "'$path' is a block device"
        elif [ -c "$path" ]; then
            echo "'$path' is a character device"
        elif [ -p "$path" ]; then
            echo "'$path' is a named pipe"
        elif [ -S "$path" ]; then
            echo "'$path' is a socket"
        else
            echo "'$path' is something else"
        fi
    else
        echo "'$path' does not exist!"
    fi
}

# c) Convert filenames to uppercase
convert_to_uppercase() {
    echo "Enter filename(s) separated by space:"
    read -a files
    
    for file in "${files[@]}"; do
        if [ ! -e "$file" ]; then
            echo "File '$file' does not exist! Skipping..."
            continue
        fi
        
        # Get directory and filename
        dir=$(dirname "$file")
        basename=$(basename "$file")
        
        # Convert to uppercase
        uppercase=$(echo "$basename" | tr '[:lower:]' '[:upper:]')
        
        if [ "$basename" = "$uppercase" ]; then
            echo "'$file' is already in uppercase!"
        else
            new_path="$dir/$uppercase"
            
            if [ -e "$new_path" ]; then
                echo "Cannot rename '$file' - '$new_path' already exists!"
            else
                mv "$file" "$new_path"
                echo "Renamed: '$file' -> '$new_path'"
            fi
        fi
    done
}

# Main program
while true; do
    display_menu
    echo "Enter your choice:"
    read choice
    
    case $choice in
        1) find_file ;;
        2) check_file_type ;;
        3) convert_to_uppercase ;;
        4) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
