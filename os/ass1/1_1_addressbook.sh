#!/bin/bash

################################################################################
# ASSIGNMENT 1.1: ADDRESS BOOK (SIMPLE VERSION)
################################################################################
#
# PROBLEM:
# Implement an address book (address.txt) with ID, Name, Phone
# Functions: Search, Add, Remove, Quit
#
# EXECUTION:
# chmod +x 1_1_addressbook.sh
# ./1_1_addressbook.sh
#
# FILE FORMAT (address.txt):
# ID:Name:Phone
# 1:John Doe:1234567890
# 2:Jane Smith:9876543210
#
################################################################################

ADDRESS_FILE="address.txt"

# Create address file if it doesn't exist
if [ ! -f "$ADDRESS_FILE" ]; then
    touch "$ADDRESS_FILE"
    echo "Created new address book: $ADDRESS_FILE"
fi

# Function to display menu
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

# Function to search address book
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
            # Search by ID
            grep "^$search_term:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
                echo "ID: $id, Name: $name, Phone: $phone"
            done
            ;;
        2)
            # Search by Name (case insensitive)
            grep -i ":$search_term:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
                echo "ID: $id, Name: $name, Phone: $phone"
            done
            ;;
        3)
            # Search by Phone
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

# Function to add entry
add_entry() {
    echo ""
    echo "--- Add New Entry ---"
    
    # Generate new ID (last ID + 1)
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
    
    # Add entry to file
    echo "$new_id:$name:$phone" >> "$ADDRESS_FILE"
    
    echo ""
    echo "✓ Entry added successfully!"
    echo "ID: $new_id, Name: $name, Phone: $phone"
}

# Function to remove entry
remove_entry() {
    echo ""
    echo "--- Remove Entry ---"
    echo "Enter ID to remove: "
    read remove_id
    
    # Check if ID exists
    if grep -q "^$remove_id:" "$ADDRESS_FILE"; then
        # Show entry to be removed
        echo ""
        echo "Entry to be removed:"
        grep "^$remove_id:" "$ADDRESS_FILE" | while IFS=: read id name phone; do
            echo "ID: $id, Name: $name, Phone: $phone"
        done
        
        echo ""
        echo "Are you sure? (y/n): "
        read confirm
        
        if [ "$confirm" = "y" ] || [ "$confirm" = "Y" ]; then
            # Remove entry (create temp file without the entry)
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

# Function to display all entries
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

# Main program loop
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
    
    # Ask to continue
    echo ""
    echo "Press Enter to continue..."
    read
done

################################################################################
# EXAM NOTES:
#
# 1. SHELL SCRIPT BASICS:
#    #!/bin/bash          - Shebang (specifies bash interpreter)
#    chmod +x script.sh   - Make executable
#    ./script.sh          - Run script
#
# 2. VARIABLES:
#    name="John"          - Assignment (no spaces around =)
#    echo $name           - Access variable with $
#    read name            - Read user input
#
# 3. FILE OPERATIONS:
#    [ -f file ]          - Test if file exists
#    [ -s file ]          - Test if file exists and not empty
#    touch file           - Create empty file
#    cat file             - Display file contents
#    >> file              - Append to file
#    > file               - Overwrite file
#
# 4. GREP COMMAND:
#    grep "pattern" file  - Search for pattern
#    grep -i              - Case insensitive search
#    grep -v              - Invert match (exclude)
#    grep -q              - Quiet (no output, just exit status)
#    grep "^pattern"      - Match at start of line
#    grep "pattern$"      - Match at end of line
#
# 5. WHILE LOOP WITH IFS:
#    IFS=:                - Internal Field Separator (delimiter)
#    while IFS=: read a b c; do
#        echo $a $b $c
#    done < file
#    - Reads file line by line, splitting by :
#
# 6. CUT COMMAND:
#    cut -d: -f1          - Cut using : delimiter, field 1
#    tail -1 file         - Last line of file
#
# 7. CASE STATEMENT:
#    case $var in
#        1) command;;
#        2) command;;
#        *) default;;
#    esac
#
# 8. CONDITIONAL:
#    if [ condition ]; then
#        commands
#    fi
#
# 9. ARITHMETIC:
#    $((expression))      - Arithmetic expansion
#    new_id=$((last_id + 1))
#
# 10. FUNCTIONS:
#     function_name() {
#         commands
#     }
#     function_name        - Call function
#
# 11. FILE FORMAT:
#     ID:Name:Phone        - Colon-separated values
#     1:John Doe:1234567890
#     Easy to parse with IFS=:
#
# 12. EXAM TIP:
#     - Use simple delimiters (:, |, ,)
#     - Test file existence before operations
#     - Use grep for searching
#     - Use temp file for deletions
#     - Always validate user input
#     - Show confirmation before delete
#
################################################################################
