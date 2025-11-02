#!/bin/bash

# 1.2 Write a script program to create a phonebook. Using the phonebook, 
# perform the following tasks:
# - Add a new name to a phone book file
# - Display all matches to a name or phone number
# - Sort the phone book by the last name
# - Delete an entry
# Suggested record format: Use tabs to separate fields.

FILE="phonebook.txt"

# Create file if it doesn't exist
if [ ! -f "$FILE" ]; then
    touch "$FILE"
fi

# Function to display menu
display_menu() {
    echo "================================"
    echo "       PHONEBOOK MENU"
    echo "================================"
    echo "1. Add New Entry"
    echo "2. Search Entry"
    echo "3. Sort by Last Name"
    echo "4. Delete Entry"
    echo "5. Display All Entries"
    echo "6. Exit"
    echo "================================"
}

# Function to add entry
add_entry() {
    echo "Enter First Name:"
    read first_name
    echo "Enter Last Name:"
    read last_name
    echo "Enter Phone Number:"
    read phone
    
    echo -e "$first_name\t$last_name\t$phone" >> "$FILE"
    echo "Entry added successfully!"
}

# Function to search entry
search_entry() {
    echo "Enter search term (Name/Phone):"
    read search_term
    
    result=$(grep -i "$search_term" "$FILE")
    
    if [ -z "$result" ]; then
        echo "No matching entries found!"
    else
        echo "Matching entries:"
        echo -e "First Name\tLast Name\tPhone"
        echo "$result"
    fi
}

# Function to sort by last name
sort_phonebook() {
    sort -t$'\t' -k2 "$FILE" -o "$FILE"
    echo "Phonebook sorted by last name!"
    echo -e "\nFirst Name\tLast Name\tPhone"
    cat "$FILE"
}

# Function to delete entry
delete_entry() {
    echo "Enter phone number to delete:"
    read phone
    
    if grep -q "$phone" "$FILE"; then
        grep -v "$phone" "$FILE" > temp.txt
        mv temp.txt "$FILE"
        echo "Entry deleted successfully!"
    else
        echo "Entry not found!"
    fi
}

# Function to display all entries
display_all() {
    if [ -s "$FILE" ]; then
        echo -e "First Name\tLast Name\tPhone"
        cat "$FILE"
    else
        echo "Phonebook is empty!"
    fi
}

# Main program
while true; do
    display_menu
    echo "Enter your choice:"
    read choice
    
    case $choice in
        1) add_entry ;;
        2) search_entry ;;
        3) sort_phonebook ;;
        4) delete_entry ;;
        5) display_all ;;
        6) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
