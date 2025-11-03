#!/bin/bash

PHONEBOOK_FILE="phonebook.txt"

if [ ! -f "$PHONEBOOK_FILE" ]; then
    touch "$PHONEBOOK_FILE"
    echo "Created new phonebook: $PHONEBOOK_FILE"
fi

display_menu() {
    echo ""
    echo "========================================"
    echo "         PHONEBOOK MENU"
    echo "========================================"
    echo "1. Add New Entry"
    echo "2. Display Matches (Search)"
    echo "3. Sort by Last Name"
    echo "4. Delete Entry"
    echo "5. Display All Entries"
    echo "6. Quit"
    echo "========================================"
}

add_entry() {
    echo ""
    echo "--- Add New Entry ---"
    
    echo "Enter First Name: "
    read first_name
    
    echo "Enter Last Name: "
    read last_name
    
    echo "Enter Phone Number: "
    read phone
    
    echo "$first_name $last_name:$phone" >> "$PHONEBOOK_FILE"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "Name: $first_name $last_name, Phone: $phone"
}

display_matches() {
    echo ""
    echo "--- Search Phonebook ---"
    echo "Enter name or phone number to search: "
    read search_term
    
    echo ""
    echo "Search Results:"
    echo "----------------------------------------"
    
    grep -i "$search_term" "$PHONEBOOK_FILE" | while IFS=: read name phone; do
        echo "Name: $name, Phone: $phone"
    done
    
    if ! grep -qi "$search_term" "$PHONEBOOK_FILE"; then
        echo "No matches found!"
    fi
    
    echo "----------------------------------------"
}

sort_by_lastname() {
    echo ""
    echo "--- Sort by Last Name ---"
    
    if [ ! -s "$PHONEBOOK_FILE" ]; then
        echo "Phonebook is empty!"
        return
    fi
    
    echo ""
    echo "Sorted Phonebook (by Last Name):"
    echo "----------------------------------------"
    
    sort -t' ' -k2 "$PHONEBOOK_FILE" | while IFS=: read name phone; do
        echo "Name: $name, Phone: $phone"
    done
    
    echo "----------------------------------------"
    echo ""
    echo "Save sorted version? (y/n): "
    read save_choice
    
    if [ "$save_choice" = "y" ] || [ "$save_choice" = "Y" ]; then
        sort -t' ' -k2 "$PHONEBOOK_FILE" > temp_sorted.txt
        mv temp_sorted.txt "$PHONEBOOK_FILE"
        echo "✓ Phonebook sorted and saved!"
    fi
}

delete_entry() {
    echo ""
    echo "--- Delete Entry ---"
    echo "Enter name or phone to delete: "
    read delete_term
    
    if grep -qi "$delete_term" "$PHONEBOOK_FILE"; then
        echo ""
        echo "Matching entries:"
        echo "----------------------------------------"
        grep -i "$delete_term" "$PHONEBOOK_FILE" | while IFS=: read name phone; do
            echo "Name: $name, Phone: $phone"
        done
        echo "----------------------------------------"
        
        echo ""
        echo "Delete all matching entries? (y/n): "
        read confirm
        
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            grep -vi "$delete_term" "$PHONEBOOK_FILE" > temp.txt
            mv temp.txt "$PHONEBOOK_FILE"
            echo "✓ Entries deleted successfully!"
        else
            echo "Deletion cancelled."
        fi
    else
        echo "No matching entries found!"
    fi
}

display_all() {
    echo ""
    echo "--- All Phonebook Entries ---"
    echo "----------------------------------------"
    
    if [ -s "$PHONEBOOK_FILE" ]; then
        while IFS=: read name phone; do
            echo "Name: $name, Phone: $phone"
        done < "$PHONEBOOK_FILE"
    else
        echo "Phonebook is empty!"
    fi
    
    echo "----------------------------------------"
}

echo "========================================"
echo "    PHONEBOOK MANAGEMENT SYSTEM"
echo "========================================"

while true; do
    display_menu
    echo -n "Enter your choice: "
    read choice
    
    case $choice in
        1)
            add_entry
            ;;
        2)
            display_matches
            ;;
        3)
            sort_by_lastname
            ;;
        4)
            delete_entry
            ;;
        5)
            display_all
            ;;
        6)
            echo ""
            echo "Thank you for using Phonebook!"
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice! Please enter 1-6."
            ;;
    esac
    
    echo ""
    echo "Press Enter to continue..."
    read
done

