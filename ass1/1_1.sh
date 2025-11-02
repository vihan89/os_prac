#!/bin/bash

# 1.1 Write a shell script to implement an address book (address.txt) that contains 
# ID, Name, and Phone with the following functions:
# - Search Address Book
# - Add an address book entry
# - Remove an address book entry
# - Quit the program.
# Suggested record format: Use semi-colon (;) to separate fields.

FILE="address.txt"

# Create file if it doesn't exist
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi

# Function to display menu
display_menu() {
    echo "================================"
    echo "     ADDRESS BOOK MENU"
    echo "================================"
    echo "1. Search Address Book"
    echo "2. Add Entry"
    echo "3. Remove Entry"
    echo "4. Quit"
    echo "================================"
}

# Function to search address book
search_entry() {
    echo "Enter search term (ID/Name/Phone):"
    read search_term
    
    result=$(grep -i "$search_term" "$FILE")
    
    if [ -z "$result" ]; then
        echo "No matching entries found!"
    else
        echo "Matching entries:"
        echo "ID;Name;Phone"
        echo "$result"
    fi
}

# Function to add entry
add_entry() {
    echo "Enter ID:"
    read id
    echo "Enter Name:"
    read name
    echo "Enter Phone:"
    read phone
    
    echo "$id;$name;$phone" >> "$FILE"
    echo "Entry added successfully!"
}

# Function to remove entry
remove_entry() {
    echo "Enter ID to remove:"
    read id
    
    if grep -q "^$id;" "$FILE"; then
        grep -v "^$id;" "$FILE" > temp.txt
        mv temp.txt "$FILE"
        echo "Entry removed successfully!"
    else
        echo "Entry not found!"
    fi
}

# Main program
while true; do
    display_menu
    echo "Enter your choice:"
    read choice
    
    case $choice in
        1) search_entry ;;
        2) add_entry ;;
        3) remove_entry ;;
        4) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
