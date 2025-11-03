#!/bin/bash

ADDRESS_FILE="address.txt"

if [ ! -f "$ADDRESS_FILE" ]; then
    touch "$ADDRESS_FILE"
    echo "Created new address book: $ADDRESS_FILE"
fi

display_menu() {
    echo ""
    echo "========================================"
    echo "       ADDRESS BOOK MENU"
    echo "========================================"
    echo "1. Search Address Book"
    echo "2. Add Entry"
    echo "3. Remove Entry"
    echo "4. Quit"
    echo "========================================"
}

search_address() {
    echo ""
    echo "--- Search Address Book ---"
    echo "Search by (1) ID, (2) Name, (3) Phone: "
    read search_choice
    
    echo "Enter search term: "
    read search_term
    
    echo ""
    echo "Search Results:"
    echo "----------------------------------------"
    
    case $search_choice in
        1)
            grep "^$search_term:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
                echo "ID: $id, Name: $name, Phone: $phone"
            done
            ;;
        2)
            grep -i ":$search_term:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
                echo "ID: $id, Name: $name, Phone: $phone"
            done
            ;;
        3)
            grep ":$search_term$" "$ADDRESS_FILE" | while IFS=: read id name phone; do
                echo "ID: $id, Name: $name, Phone: $phone"
            done
            ;;
        *)
            echo "Invalid search option!"
            ;;
    esac
    
    echo "----------------------------------------"
}

add_entry() {
    echo ""
    echo "--- Add New Entry ---"
    
    if [ -s "$ADDRESS_FILE" ]; then
        last_id=$(tail -1 "$ADDRESS_FILE" | cut -d: -f1)
        new_id=$((last_id + 1))
    else
        new_id=1
    fi
    
    echo "Enter Name: "
    read name
    
    echo "Enter Phone: "
    read phone
    
    echo "$new_id:$name:$phone" >> "$ADDRESS_FILE"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "ID: $new_id, Name: $name, Phone: $phone"
}

remove_entry() {
    echo ""
    echo "--- Remove Entry ---"
    echo "Enter ID to remove: "
    read remove_id
    
    if grep -q "^$remove_id:" "$ADDRESS_FILE"; then
        echo ""
        echo "Entry to be removed:"
        grep "^$remove_id:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
            echo "ID: $id, Name: $name, Phone: $phone"
        done
        
        echo ""
        echo "Are you sure? (y/n): "
        read confirm
        
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            grep -v "^$remove_id:" "$ADDRESS_FILE" > temp.txt
            mv temp.txt "$ADDRESS_FILE"
            echo "✓ Entry removed successfully!"
        else
            echo "Removal cancelled."
        fi
    else
        echo "Error: ID $remove_id not found!"
    fi
}

display_all() {
    echo ""
    echo "--- All Entries ---"
    echo "----------------------------------------"
    
    if [ -s "$ADDRESS_FILE" ]; then
        while IFS=: read id name phone; do
            echo "ID: $id, Name: $name, Phone: $phone"
        done < "$ADDRESS_FILE"
    else
        echo "Address book is empty!"
    fi
    
    echo "----------------------------------------"
}

echo "========================================"
echo "   ADDRESS BOOK MANAGEMENT SYSTEM"
echo "========================================"

while true; do
    display_menu
    echo -n "Enter your choice: "
    read choice
    
    case $choice in
        1)
            search_address
            ;;
        2)
            add_entry
            ;;
        3)
            remove_entry
            ;;
        4)
            echo ""
            echo "Thank you for using Address Book!"
            echo "Goodbye!"
            exit 0
            ;;
        *)
            echo "Invalid choice! Please enter 1-4."
            ;;
    esac
    
    echo ""
    echo "Press Enter to continue..."
    read
done

