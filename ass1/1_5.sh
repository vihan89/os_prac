#!/bin/bash

# 1.5 Write a shell script:
# a) that accepts a file name, starting and ending line numbers as arguments 
#    and displays all the lines between the given line numbers.
# b) that deletes all lines containing a specified word in one or more files 
#    supplied as arguments to it.
# c) that take two numbers as arguments and output their sum using 
#    i) bc ii) expr. Include error checking to test if two arguments were entered.

# Function to display menu
display_menu() {
    echo "================================"
    echo "     UTILITY SCRIPTS MENU"
    echo "================================"
    echo "1. Display lines from file (between line numbers)"
    echo "2. Delete lines containing a word"
    echo "3. Add two numbers (using bc)"
    echo "4. Add two numbers (using expr)"
    echo "5. Exit"
    echo "================================"
}

# a) Display lines between given line numbers
display_lines() {
    echo "Enter filename:"
    read filename
    
    if [ ! -f "$filename" ]; then
        echo "File not found!"
        return
    fi
    
    echo "Enter starting line number:"
    read start
    echo "Enter ending line number:"
    read end
    
    if ! [[ "$start" =~ ^[0-9]+$ ]] || ! [[ "$end" =~ ^[0-9]+$ ]]; then
        echo "Error: Line numbers must be integers!"
        return
    fi
    
    echo "Lines from $start to $end:"
    sed -n "${start},${end}p" "$filename"
}

# b) Delete lines containing a word
delete_lines_with_word() {
    echo "Enter the word to delete:"
    read word
    
    echo "Enter filename(s) separated by space:"
    read -a files
    
    for file in "${files[@]}"; do
        if [ ! -f "$file" ]; then
            echo "File '$file' not found! Skipping..."
            continue
        fi
        
        sed -i.bak "/$word/d" "$file"
        echo "Lines containing '$word' deleted from '$file'"
        echo "Backup saved as '${file}.bak'"
    done
}

# c) Add two numbers using bc
add_using_bc() {
    echo "Enter first number:"
    read num1
    echo "Enter second number:"
    read num2
    
    if ! [[ "$num1" =~ ^-?[0-9]+\.?[0-9]*$ ]] || ! [[ "$num2" =~ ^-?[0-9]+\.?[0-9]*$ ]]; then
        echo "Error: Please enter valid numbers!"
        return
    fi
    
    sum=$(echo "$num1 + $num2" | bc)
    echo "Sum (using bc): $sum"
}

# c) Add two numbers using expr
add_using_expr() {
    echo "Enter first number:"
    read num1
    echo "Enter second number:"
    read num2
    
    if ! [[ "$num1" =~ ^-?[0-9]+$ ]] || ! [[ "$num2" =~ ^-?[0-9]+$ ]]; then
        echo "Error: expr only works with integers!"
        return
    fi
    
    sum=$(expr $num1 + $num2)
    echo "Sum (using expr): $sum"
}

# Main program
while true; do
    display_menu
    echo "Enter your choice:"
    read choice
    
    case $choice in
        1) display_lines ;;
        2) delete_lines_with_word ;;
        3) add_using_bc ;;
        4) add_using_expr ;;
        5) echo "Exiting..."; exit 0 ;;
        *) echo "Invalid choice! Please try again." ;;
    esac
    
    echo ""
done
