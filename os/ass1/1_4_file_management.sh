#!/bin/bash

################################################################################
# ASSIGNMENT 1.4: FILE MANAGEMENT PROGRAM (BOURNE-AGAIN SHELL)
################################################################################
#
# PROBLEM:
# Menu-driven file management utility:
# - Test if file exists
# - Read file contents
# - Delete file
# - Display file list
#
# EXECUTION:
# chmod +x 1_4_file_management.sh
# ./1_4_file_management.sh
#
################################################################################

# Display menu
show_menu() {
    clear
    echo "========================================"
    echo "     FILE MANAGEMENT UTILITY"
    echo "========================================"
    echo "1. Test if File Exists"
    echo "2. Read File Contents"
    echo "3. Delete File"
    echo "4. Display File List"
    echo "5. Create New File"
    echo "6. File Information"
    echo "7. Exit"
    echo "========================================"
}

# Test if file exists
test_file_exists() {
    echo ""
    echo "=== TEST FILE EXISTENCE ==="
    echo ""
    
    echo "Enter filename: "
    read filename
    
    if [ -e "$filename" ]; then
        echo ""
        echo "✓ File '$filename' EXISTS"
        
        if [ -f "$filename" ]; then
            echo "  Type: Regular file"
        elif [ -d "$filename" ]; then
            echo "  Type: Directory"
        elif [ -L "$filename" ]; then
            echo "  Type: Symbolic link"
        fi
        
        # Check permissions
        echo "  Permissions:"
        [ -r "$filename" ] && echo "    - Readable" || echo "    - Not readable"
        [ -w "$filename" ] && echo "    - Writable" || echo "    - Not writable"
        [ -x "$filename" ] && echo "    - Executable" || echo "    - Not executable"
        
        # Check if empty
        if [ -f "$filename" ]; then
            if [ -s "$filename" ]; then
                echo "  Status: File has content"
            else
                echo "  Status: File is empty"
            fi
        fi
    else
        echo "✗ File '$filename' DOES NOT EXIST"
    fi
}

# Read file contents
read_file() {
    echo ""
    echo "=== READ FILE CONTENTS ==="
    echo ""
    
    echo "Enter filename: "
    read filename
    
    if [ ! -e "$filename" ]; then
        echo "Error: File '$filename' does not exist!"
        return
    fi
    
    if [ ! -f "$filename" ]; then
        echo "Error: '$filename' is not a regular file!"
        return
    fi
    
    if [ ! -r "$filename" ]; then
        echo "Error: No read permission for '$filename'!"
        return
    fi
    
    if [ ! -s "$filename" ]; then
        echo "Warning: File is empty!"
        return
    fi
    
    echo ""
    echo "Contents of '$filename':"
    echo "----------------------------------------------------------------"
    cat "$filename"
    echo "----------------------------------------------------------------"
    
    # Show line count
    line_count=$(wc -l < "$filename")
    echo ""
    echo "Total lines: $line_count"
}

# Delete file
delete_file() {
    echo ""
    echo "=== DELETE FILE ==="
    echo ""
    
    echo "Enter filename: "
    read filename
    
    if [ ! -e "$filename" ]; then
        echo "Error: File '$filename' does not exist!"
        return
    fi
    
    if [ ! -f "$filename" ]; then
        echo "Error: '$filename' is not a regular file!"
        return
    fi
    
    echo ""
    echo "File to be deleted: $filename"
    
    # Show file info
    if [ -s "$filename" ]; then
        size=$(wc -c < "$filename")
        lines=$(wc -l < "$filename")
        echo "Size: $size bytes"
        echo "Lines: $lines"
    else
        echo "Size: 0 bytes (empty)"
    fi
    
    echo ""
    echo "Are you sure you want to delete this file? (yes/no): "
    read confirm
    
    if [ "$confirm" = "yes" ] || [ "$confirm" = "y" ]; then
        rm "$filename"
        
        if [ $? -eq 0 ]; then
            echo "✓ File '$filename' deleted successfully!"
        else
            echo "✗ Error deleting file!"
        fi
    else
        echo "Deletion cancelled."
    fi
}

# Display file list
display_file_list() {
    echo ""
    echo "=== FILE LIST ==="
    echo ""
    
    echo "Select option:"
    echo "1. List files in current directory"
    echo "2. List files in specific directory"
    echo "3. Search files by pattern"
    echo ""
    echo -n "Enter choice [1-3]: "
    read list_choice
    
    case $list_choice in
        1)
            echo ""
            echo "Files in current directory:"
            echo "----------------------------------------------------------------"
            ls -lh | grep "^-"
            echo "----------------------------------------------------------------"
            
            file_count=$(ls -l | grep "^-" | wc -l)
            echo "Total files: $file_count"
            ;;
            
        2)
            echo ""
            echo "Enter directory path: "
            read dirpath
            
            if [ ! -d "$dirpath" ]; then
                echo "Error: '$dirpath' is not a valid directory!"
                return
            fi
            
            echo ""
            echo "Files in '$dirpath':"
            echo "----------------------------------------------------------------"
            ls -lh "$dirpath" | grep "^-"
            echo "----------------------------------------------------------------"
            
            file_count=$(ls -l "$dirpath" | grep "^-" | wc -l)
            echo "Total files: $file_count"
            ;;
            
        3)
            echo ""
            echo "Enter file pattern (e.g., *.txt, test*, *.sh): "
            read pattern
            
            echo ""
            echo "Files matching '$pattern':"
            echo "----------------------------------------------------------------"
            
            if ls $pattern 2>/dev/null; then
                echo "----------------------------------------------------------------"
                match_count=$(ls $pattern 2>/dev/null | wc -l)
                echo "Total matches: $match_count"
            else
                echo "No files matching pattern!"
                echo "----------------------------------------------------------------"
            fi
            ;;
            
        *)
            echo "Invalid choice!"
            ;;
    esac
}

# Create new file
create_file() {
    echo ""
    echo "=== CREATE NEW FILE ==="
    echo ""
    
    echo "Enter filename: "
    read filename
    
    if [ -e "$filename" ]; then
        echo "Warning: File '$filename' already exists!"
        echo "Overwrite? (yes/no): "
        read overwrite
        
        if [ "$overwrite" != "yes" ] && [ "$overwrite" != "y" ]; then
            echo "File creation cancelled."
            return
        fi
    fi
    
    echo ""
    echo "Enter content (type 'EOF' on a new line to finish):"
    echo "----------------------------------------------------------------"
    
    > "$filename"  # Clear file
    
    while true; do
        read line
        if [ "$line" = "EOF" ]; then
            break
        fi
        echo "$line" >> "$filename"
    done
    
    echo "----------------------------------------------------------------"
    echo "✓ File '$filename' created successfully!"
    
    if [ -s "$filename" ]; then
        line_count=$(wc -l < "$filename")
        echo "Lines written: $line_count"
    else
        echo "File is empty."
    fi
}

# File information
file_information() {
    echo ""
    echo "=== FILE INFORMATION ==="
    echo ""
    
    echo "Enter filename: "
    read filename
    
    if [ ! -e "$filename" ]; then
        echo "Error: File '$filename' does not exist!"
        return
    fi
    
    echo ""
    echo "Information for: $filename"
    echo "----------------------------------------------------------------"
    
    # File type
    if [ -f "$filename" ]; then
        echo "Type: Regular file"
    elif [ -d "$filename" ]; then
        echo "Type: Directory"
    elif [ -L "$filename" ]; then
        echo "Type: Symbolic link"
    else
        echo "Type: Special file"
    fi
    
    # Permissions
    perms=$(ls -ld "$filename" | awk '{print $1}')
    echo "Permissions: $perms"
    
    # Owner
    owner=$(ls -ld "$filename" | awk '{print $3}')
    echo "Owner: $owner"
    
    # Group
    group=$(ls -ld "$filename" | awk '{print $4}')
    echo "Group: $group"
    
    # Size
    if [ -f "$filename" ]; then
        size=$(wc -c < "$filename")
        size_human=$(ls -lh "$filename" | awk '{print $5}')
        echo "Size: $size bytes ($size_human)"
        
        if [ -s "$filename" ]; then
            line_count=$(wc -l < "$filename")
            word_count=$(wc -w < "$filename")
            echo "Lines: $line_count"
            echo "Words: $word_count"
        fi
    fi
    
    # Modification time
    mod_time=$(ls -l "$filename" | awk '{print $6, $7, $8}')
    echo "Last Modified: $mod_time"
    
    echo "----------------------------------------------------------------"
}

# Main program
main() {
    while true; do
        show_menu
        echo -n "Enter your choice [1-7]: "
        read choice
        
        case $choice in
            1) test_file_exists ;;
            2) read_file ;;
            3) delete_file ;;
            4) display_file_list ;;
            5) create_file ;;
            6) file_information ;;
            7) 
                echo ""
                echo "Thank you for using File Management Utility!"
                exit 0
                ;;
            *) 
                echo "Invalid choice! Please enter 1-7."
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
# 1. FILE TEST OPERATORS:
#    [ -e file ]    - File exists (any type)
#    [ -f file ]    - Regular file exists
#    [ -d file ]    - Directory exists
#    [ -L file ]    - Symbolic link exists
#    [ -s file ]    - File exists and not empty
#    [ -r file ]    - File is readable
#    [ -w file ]    - File is writable
#    [ -x file ]    - File is executable
#
# 2. CAT COMMAND:
#    cat filename            - Display file contents
#    cat file1 file2         - Display multiple files
#    cat > file              - Create/overwrite file
#    cat >> file             - Append to file
#
# 3. WC COMMAND:
#    wc filename             - Count lines, words, bytes
#    wc -l                   - Count lines only
#    wc -w                   - Count words only
#    wc -c                   - Count bytes only
#    wc -l < file            - Count lines (no filename in output)
#
# 4. RM COMMAND:
#    rm filename             - Delete file
#    rm -r directory         - Delete directory recursively
#    rm -f file              - Force delete (no confirmation)
#    rm -i file              - Interactive delete
#
# 5. LS COMMAND OPTIONS:
#    ls                      - List files
#    ls -l                   - Long format (permissions, size, etc.)
#    ls -lh                  - Human-readable sizes (KB, MB)
#    ls -a                   - Show hidden files (.files)
#    ls *.txt                - List with pattern
#
# 6. GREP WITH LS:
#    ls -l | grep "^-"       - Filter regular files
#    ls -l | grep "^d"       - Filter directories
#    
#    File type indicators in ls -l:
#    - : Regular file
#    d : Directory
#    l : Symbolic link
#
# 7. EXIT STATUS:
#    $?                      - Exit status of last command
#    0  : Success
#    1+ : Error
#    
#    if [ $? -eq 0 ]; then
#        echo "Success"
#    fi
#
# 8. REDIRECTING ERRORS:
#    command 2>/dev/null     - Suppress error messages
#    command 2>&1            - Redirect errors to stdout
#    command > out 2> err    - Separate output/error files
#
# 9. AWK FOR FIELD EXTRACTION:
#    ls -l | awk '{print $1}'    - Print 1st field (permissions)
#    ls -l | awk '{print $5}'    - Print 5th field (size)
#    ls -l | awk '{print $3}'    - Print 3rd field (owner)
#    
#    Fields are space-separated by default
#
# 10. OUTPUT REDIRECTION:
#     > file         - Overwrite file
#     >> file        - Append to file
#     < file         - Read input from file
#     
#     Example:
#     > newfile      - Create empty file
#     echo "text" > file  - Write to file
#
# 11. EOF PATTERN:
#     Read until user types "EOF":
#     while true; do
#         read line
#         [ "$line" = "EOF" ] && break
#         echo "$line" >> file
#     done
#
# 12. FILE PATTERNS:
#     *.txt          - All .txt files
#     test*          - Files starting with "test"
#     *test*         - Files containing "test"
#     ?.txt          - Single char + .txt
#     [abc]*         - Files starting with a, b, or c
#
# 13. LOGICAL OPERATORS:
#     ||             - OR (execute if previous fails)
#     &&             - AND (execute if previous succeeds)
#     
#     Examples:
#     [ -r file ] && cat file
#     [ -f file ] || echo "Not found"
#
# 14. EXAM TIP:
#     - Always check if file exists before operations
#     - Check file type (regular file vs directory)
#     - Check permissions before reading/writing
#     - Confirm before destructive operations (delete)
#     - Use -s to check if file is not empty
#     - Redirect errors with 2>/dev/null
#     - Use human-readable formats (ls -lh)
#     - Extract fields with awk or cut
#     - Test operators are essential for file management
#
################################################################################
