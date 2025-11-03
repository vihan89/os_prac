#!/bin/bash

ADDRESSBOOK="addressbook.txt"

initialize_file() {
    if [ ! -f "$ADDRESSBOOK" ]; then
        touch "$ADDRESSBOOK"
        echo "Address book created: $ADDRESSBOOK"
    fi
}

show_menu() {
    clear
    echo "========================================"
    echo "     ADDRESS BOOK - BASH FUNCTIONS"
    echo "========================================"
    echo "1. Search Address Book"
    echo "2. Add Entry"
    echo "3. Remove Entry"
    echo "4. Edit Entry"
    echo "5. Display All Entries"
    echo "6. Exit"
    echo "========================================"
}

search_addressbook() {
    echo ""
    echo "=== SEARCH ADDRESS BOOK ==="
    echo ""
    
    if [ ! -s "$ADDRESSBOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo "Enter search term (name, phone, or email): "
    read search_term
    
    echo ""
    echo "Search Results:"
    echo "----------------------------------------------------------------"
    printf "%-5s %-20s %-25s %-15s %-25s\n" "ID" "Name" "Address" "Phone" "Email"
    echo "----------------------------------------------------------------"
    
    grep -i "$search_term" "$ADDRESSBOOK" | while IFS='|' read id name address phone email; do
        printf "%-5s %-20s %-25s %-15s %-25s\n" "$id" "$name" "$address" "$phone" "$email"
    done
    
    if ! grep -qi "$search_term" "$ADDRESSBOOK"; then
        echo "No results found!"
    fi
    
    echo "----------------------------------------------------------------"
}

add_entry() {
    echo ""
    echo "=== ADD NEW ENTRY ==="
    echo ""
    
    if [ -s "$ADDRESSBOOK" ]; then
        last_id=$(tail -1 "$ADDRESSBOOK" | cut -d'|' -f1)
        new_id=$((last_id + 1))
    else
        new_id=1
    fi
    
    echo "Enter Name: "
    read name
    
    echo "Enter Address: "
    read address
    
    echo "Enter Phone: "
    read phone
    
    echo "Enter Email: "
    read email
    
    echo "$new_id|$name|$address|$phone|$email" >> "$ADDRESSBOOK"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "ID: $new_id"
    echo "Name: $name"
    echo "Address: $address"
    echo "Phone: $phone"
    echo "Email: $email"
}

remove_entry() {
    echo ""
    echo "=== REMOVE ENTRY ==="
    echo ""
    
    if [ ! -s "$ADDRESSBOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo "Enter ID to remove: "
    read remove_id
    
    if grep -q "^$remove_id|" "$ADDRESSBOOK"; then
        echo ""
        echo "Entry to be removed:"
        echo "----------------------------------------------------------------"
        grep "^$remove_id|" "$ADDRESSBOOK" | while IFS='|' read id name address phone email; do
            echo "ID: $id"
            echo "Name: $name"
            echo "Address: $address"
            echo "Phone: $phone"
            echo "Email: $email"
        done
        echo "----------------------------------------------------------------"
        
        echo ""
        echo "Are you sure? (yes/no): "
        read confirm
        
        if [ "$confirm" = "yes" ] || [ "$confirm" = "y" ]; then
            grep -v "^$remove_id|" "$ADDRESSBOOK" > temp.txt
            mv temp.txt "$ADDRESSBOOK"
            echo "✓ Entry removed successfully!"
        else
            echo "Removal cancelled."
        fi
    else
        echo "Error: ID $remove_id not found!"
    fi
}

edit_entry() {
    echo ""
    echo "=== EDIT ENTRY ==="
    echo ""
    
    if [ ! -s "$ADDRESSBOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo "Enter ID to edit: "
    read edit_id
    
    if grep -q "^$edit_id|" "$ADDRESSBOOK"; then
        echo ""
        echo "Current Entry:"
        echo "----------------------------------------------------------------"
        grep "^$edit_id|" "$ADDRESSBOOK" | while IFS='|' read id name address phone email; do
            echo "ID: $id"
            echo "Name: $name"
            echo "Address: $address"
            echo "Phone: $phone"
            echo "Email: $email"
        done
        echo "----------------------------------------------------------------"
        
        echo ""
        echo "Enter new details (press Enter to keep existing):"
        
        current_line=$(grep "^$edit_id|" "$ADDRESSBOOK")
        current_name=$(echo "$current_line" | cut -d'|' -f2)
        current_address=$(echo "$current_line" | cut -d'|' -f3)
        current_phone=$(echo "$current_line" | cut -d'|' -f4)
        current_email=$(echo "$current_line" | cut -d'|' -f5)
        
        echo "Name [$current_name]: "
        read new_name
        [ -z "$new_name" ] && new_name="$current_name"
        
        echo "Address [$current_address]: "
        read new_address
        [ -z "$new_address" ] && new_address="$current_address"
        
        echo "Phone [$current_phone]: "
        read new_phone
        [ -z "$new_phone" ] && new_phone="$current_phone"
        
        echo "Email [$current_email]: "
        read new_email
        [ -z "$new_email" ] && new_email="$current_email"
        
        grep -v "^$edit_id|" "$ADDRESSBOOK" > temp.txt
        echo "$edit_id|$new_name|$new_address|$new_phone|$new_email" >> temp.txt
        sort -t'|' -k1 -n temp.txt > "$ADDRESSBOOK"
        rm temp.txt
        
        echo ""
        echo "✓ Entry updated successfully!"
    else
        echo "Error: ID $edit_id not found!"
    fi
}

display_all() {
    echo ""
    echo "=== ALL ENTRIES ==="
    echo ""
    
    if [ ! -s "$ADDRESSBOOK" ]; then
        echo "Address book is empty!"
        return
    fi
    
    echo "----------------------------------------------------------------"
    printf "%-5s %-20s %-25s %-15s %-25s\n" "ID" "Name" "Address" "Phone" "Email"
    echo "----------------------------------------------------------------"
    
    while IFS='|' read id name address phone email; do
        printf "%-5s %-20s %-25s %-15s %-25s\n" "$id" "$name" "$address" "$phone" "$email"
    done < "$ADDRESSBOOK"
    
    echo "----------------------------------------------------------------"
}

main() {
    initialize_file
    
    while true; do
        show_menu
        echo -n "Enter your choice [1-6]: "
        read choice
        
        case $choice in
            1) search_addressbook ;;
            2) add_entry ;;
            3) remove_entry ;;
            4) edit_entry ;;
            5) display_all ;;
            6) 
                echo ""
                echo "Thank you for using Address Book!"
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
}

main

