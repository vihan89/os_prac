#!/bin/bash

# 1.4 Create a file management program using the bourne-again shell. 
# It should use functions to perform the required tasks. 
# It should be menu-based, allowing you the options of:
# - Test if file exists
# - Read a file
# - Delete a file
# - Display a list of files

# Function to display menu
display_menu() {
    echo "================================"
    echo "   FILE MANAGEMENT PROGRAM"
    echo "================================"
    echo "1. Test if File Exists"
    echo "2. Read a File"
    echo "3. Delete a File"
    echo "4. Display List of Files"
    echo "5. Exit"
    echo "================================"
}

# Function to test if file exists
test_file() {
    echo "Enter filename:"
    read filename
    
    if [ -e "$filename" ]; then
        echo "File '$filename' exists!"
        
        if [ -f "$filename" ]; then
            echo "Type: Regular file"
        elif [ -d "$filename" ]; then
            echo "Type: Directory"
        else
            echo "Type: Special file"
        fi
        
        echo "Size: $(du -h "$filename" | cut -f1)"
        echo "Permissions: $(ls -lh "$filename" | awk '{print $1}')"
    else
        echo "File '$filename' does not exist!"
    fi
}

# Function to read a file
read_file() {
    echo "Enter filename:"
    read filename
    
    if [ -f "$filename" ]; then
        echo "Contents of '$filename':"
        echo "================================"
        cat "$filename"
        echo "================================"
    else
        echo "File '$filename' not found or is not a regular file!"
    fi
}

# Function to delete a file
delete_file() {
    echo "Enter filename to delete:"
    read filename
    
    if [ -e "$filename" ]; then
        echo "Are you sure you want to delete '$filename'? (y/n)"
        read confirm
        
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            rm -r "$filename"
            echo "File '$filename' deleted successfully!"
        else
            echo "Deletion cancelled!"
        fi
    else
        echo "File '$filename' not found!"
    fi
}

# Function to display list of files
list_files() {
    echo "Files in current directory:"
    echo "================================"
    ls -lh
    echo "================================"
    echo "Total files: $(ls -1 | wc -l)"
}

# Main program
while true; do
    display_menu
    echo "Enter your choice:"
    read choice
    
    case $choice in
        1) test_file ;;
        2) read_file ;;
        3) delete_file ;;
        4) list_files ;;
        5) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
