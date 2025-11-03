#!/bin/bash

################################################################################
# ASSIGNMENT 1.3: ADDRESS BOOK WITH FUNCTIONS (BOURNE-AGAIN SHELL)
################################################################################
#
# PROBLEM:
# Menu-based address book using functions:
# - Search address book
# - Add entries
# - Remove/Edit entries
#
# EXECUTION:
# chmod +x 1_3_addressbook_functions.sh
# ./1_3_addressbook_functions.sh
#
# FILE FORMAT (addressbook.txt):
# ID|Name|Address|Phone|Email
# 1|John Doe|123 Main St|1234567890|john@email.com
#
################################################################################

ADDRESSBOOK="addressbook.txt"

# Initialize file
initialize_file() {
    if [ ! -f "$ADDRESSBOOK" ]; then
        touch "$ADDRESSBOOK"
        echo "Address book created: $ADDRESSBOOK"
    fi
}

# Display menu
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

# Search function
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

# Add entry function
add_entry() {
    echo ""
    echo "=== ADD NEW ENTRY ==="
    echo ""
    
    # Generate new ID
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
    
    # Add to file
    echo "$new_id|$name|$address|$phone|$email" >> "$ADDRESSBOOK"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "ID: $new_id"
    echo "Name: $name"
    echo "Address: $address"
    echo "Phone: $phone"
    echo "Email: $email"
}

# Remove entry function
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
    
    # Check if exists
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

# Edit entry function
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
    
    # Check if exists
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
        
        # Get current values
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
        
        # Update entry
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

# Display all entries function
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

# Main program
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

# Run main program
main

################################################################################
# EXAM NOTES:
#
# 1. FUNCTION DEFINITION:
#    function_name() {
#        commands
#    }
#    
#    Or:
#    function function_name {
#        commands
#    }
#
# 2. CALLING FUNCTIONS:
#    function_name           - Just write function name
#    No () needed when calling
#
# 3. PRINTF FOR FORMATTING:
#    printf "%-5s" "value"   - Left-aligned, 5 chars wide
#    printf "%5s" "value"    - Right-aligned, 5 chars wide
#    
#    Format specifiers:
#    %s - String
#    %d - Integer
#    %f - Float
#    \n - Newline
#
# 4. DEFAULT VALUES:
#    read input
#    [ -z "$input" ] && input="default"
#    
#    - If input is empty, use default
#    - -z tests for empty string
#
# 5. CLEAR COMMAND:
#    clear                   - Clear terminal screen
#    Good for menu-based programs
#
# 6. PIPE DELIMITER:
#    Using | as field separator
#    ID|Name|Address|Phone|Email
#    
#    - Easy to type
#    - Unlikely to appear in data
#    - Works well with cut/awk
#
# 7. CUT COMMAND:
#    cut -d'|' -f1           - Field 1 (| delimiter)
#    cut -d'|' -f2           - Field 2
#    echo "a|b|c" | cut -d'|' -f2  → "b"
#
# 8. SORTING NUMERICALLY:
#    sort -t'|' -k1 -n
#    -t'|'  : Delimiter is |
#    -k1    : Sort by field 1
#    -n     : Numeric sort (1,2,10 not 1,10,2)
#
# 9. EDIT PATTERN:
#    - Get current values
#    - Show as defaults
#    - Allow Enter to keep existing
#    - Update only if changed
#
# 10. MAIN FUNCTION PATTERN:
#     main() {
#         initialization
#         while true; do
#             show_menu
#             process_choice
#         done
#     }
#     main  # Call main
#
# 11. RETURN IN FUNCTIONS:
#     return              - Exit function
#     return 0            - Exit with success
#     return 1            - Exit with failure
#
# 12. LOCAL VARIABLES:
#     local var="value"   - Variable local to function
#     Without local, all variables are global
#
# 13. EXAM TIP:
#     - Use functions for each menu option
#     - Initialize file at start
#     - Use clear for better UX
#     - Format output with printf
#     - Allow edit with defaults
#     - Confirm before delete
#     - Use meaningful function names
#
################################################################################
