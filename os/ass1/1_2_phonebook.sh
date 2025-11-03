#!/bin/bash

################################################################################
# ASSIGNMENT 1.2: PHONEBOOK WITH ADVANCED FEATURES
################################################################################
#
# PROBLEM:
# Create phonebook with: Add, Display matches, Sort by last name, Delete
#
# EXECUTION:
# chmod +x 1_2_phonebook.sh
# ./1_2_phonebook.sh
#
# FILE FORMAT (phonebook.txt):
# FirstName LastName:Phone
# John Doe:1234567890
# Jane Smith:9876543210
#
################################################################################

PHONEBOOK_FILE="phonebook.txt"

# Create phonebook file if it doesn't exist
if [ ! -f "$PHONEBOOK_FILE" ]; then
    touch "$PHONEBOOK_FILE"
    echo "Created new phonebook: $PHONEBOOK_FILE"
fi

# Function to display menu
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

# Function to add new entry
add_entry() {
    echo ""
    echo "--- Add New Entry ---"
    
    echo "Enter First Name: "
    read first_name
    
    echo "Enter Last Name: "
    read last_name
    
    echo "Enter Phone Number: "
    read phone
    
    # Add entry to file
    echo "$first_name $last_name:$phone" >> "$PHONEBOOK_FILE"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "Name: $first_name $last_name, Phone: $phone"
}

# Function to display matches (search)
display_matches() {
    echo ""
    echo "--- Search Phonebook ---"
    echo "Enter name or phone number to search: "
    read search_term
    
    echo ""
    echo "Search Results:"
    echo "----------------------------------------"
    
    # Search for matches (case insensitive)
    grep -i "$search_term" "$PHONEBOOK_FILE" | while IFS=: read name phone; do
        echo "Name: $name, Phone: $phone"
    done
    
    # Check if any matches found
    if ! grep -qi "$search_term" "$PHONEBOOK_FILE"; then
        echo "No matches found!"
    fi
    
    echo "----------------------------------------"
}

# Function to sort by last name
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
    
    # Sort by last name (field 2 before the colon)
    # awk extracts last name, sort sorts, then display
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

# Function to delete entry
delete_entry() {
    echo ""
    echo "--- Delete Entry ---"
    echo "Enter name or phone to delete: "
    read delete_term
    
    # Check if entry exists
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
            # Delete matching entries
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

# Function to display all entries
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

# Main program loop
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

################################################################################
# EXAM NOTES:
#
# 1. SORT COMMAND:
#    sort file              - Sort lines alphabetically
#    sort -t' ' -k2         - Sort by field 2 (space delimiter)
#    sort -r                - Reverse order
#    sort -n                - Numeric sort
#    
#    Example:
#    "John Doe:123" sorted by last name (field 2)
#    -t' ' means delimiter is space
#    -k2 means sort by 2nd field (Doe)
#
# 2. AWK FOR FIELD EXTRACTION:
#    awk -F: '{print $1}'   - Print first field (: delimiter)
#    awk '{print $2}'       - Print second field (space delimiter)
#    awk '{print $NF}'      - Print last field
#
# 3. GREP OPTIONS:
#    grep -i                - Case insensitive
#    grep -v                - Invert (exclude matches)
#    grep -q                - Quiet (no output)
#    grep -c                - Count matches
#
# 4. FILE NAME FORMAT:
#    "FirstName LastName:Phone"
#    Example: "John Doe:1234567890"
#    
#    - Name before :
#    - Phone after :
#    - Space separates first and last name
#
# 5. SORTING BY LAST NAME:
#    Input:
#    John Doe:123
#    Alice Brown:456
#    Bob Adams:789
#    
#    After sort -t' ' -k2:
#    Bob Adams:789        (Adams)
#    Alice Brown:456      (Brown)
#    John Doe:123         (Doe)
#
# 6. TEMP FILE PATTERN:
#    grep -v "pattern" file > temp.txt
#    mv temp.txt file
#    
#    - Creates new file without matching lines
#    - Replaces original
#    - Safe way to delete from file
#
# 7. WHILE READ LOOP:
#    while IFS=: read name phone; do
#        echo "Name: $name"
#        echo "Phone: $phone"
#    done < file
#    
#    - Reads line by line
#    - Splits by : into variables
#    - < redirects file as input
#
# 8. USER CONFIRMATION:
#    echo "Are you sure? (y/n): "
#    read confirm
#    if [ "$confirm" = "y" ]; then
#        # do action
#    fi
#
# 9. CASE-INSENSITIVE SEARCH:
#    grep -i "john" file
#    - Matches: john, John, JOHN, JoHn
#
# 10. EXAM TIP:
#     - Use sort -t for custom delimiter
#     - Use -k for field number
#     - Always show before delete
#     - Confirm destructive operations
#     - Use temp files for modifications
#     - Test for empty file before operations
#
################################################################################
