#!/bin/bash

# 1.3 Create an address book program using the bourne-again shell. 
# It should use functions to perform the required tasks. 
# It should be menu-based, allowing you the options of:
# - Search address book
# - Add entries
# - Remove / edit entries.
# You will also need a "display" function to display a record or records when selected.
# Suggested record format: Use colons to separate fields.

FILE="addressbook.txt"

# Create file if it doesn't exist
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi

# Function to display menu
display_menu() {
    echo "================================"
    echo "   ADDRESS BOOK PROGRAM"
    echo "================================"
    echo "1. Search Address Book"
    echo "2. Add Entry"
    echo "3. Remove Entry"
    echo "4. Edit Entry"
    echo "5. Display All Entries"
    echo "6. Exit"
    echo "================================"
}

# Function to display records
display_records() {
    echo "ID:Name:Phone:Email"
    cat "$FILE"
}

# Function to search
search_entry() {
    echo "Enter search term:"
    read search_term
    
    result=$(grep -i "$search_term" "$FILE")
    
    if [ -z "$result" ]; then
        echo "No matching entries found!"
    else
        echo "Matching entries:"
        echo "ID:Name:Phone:Email"
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
    echo "Enter Email:"
    read email
    
    echo "$id:$name:$phone:$email" >> "$FILE"
    echo "Entry added successfully!"
}

# Function to remove entry
remove_entry() {
    echo "Enter ID to remove:"
    read id
    
    if grep -q "^$id:" "$FILE"; then
        grep -v "^$id:" "$FILE" > temp.txt
        mv temp.txt "$FILE"
        echo "Entry removed successfully!"
    else
        echo "Entry not found!"
    fi
}

# Function to edit entry
edit_entry() {
    echo "Enter ID to edit:"
    read id
    
    if grep -q "^$id:" "$FILE"; then
        echo "Current entry:"
        grep "^$id:" "$FILE"
        
        echo "Enter new Name:"
        read name
        echo "Enter new Phone:"
        read phone
        echo "Enter new Email:"
        read email
        
        grep -v "^$id:" "$FILE" > temp.txt
        echo "$id:$name:$phone:$email" >> temp.txt
        mv temp.txt "$FILE"
        echo "Entry updated successfully!"
    else
        echo "Entry not found!"
    fi
}

# Function to display all entries
display_all() {
    if [ -s "$FILE" ]; then
        display_records
    else
        echo "Address book is empty!"
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
        4) edit_entry ;;
        5) display_all ;;
        6) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
