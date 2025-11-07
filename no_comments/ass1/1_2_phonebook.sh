#!/bin/bash

FILE="phonebook.txt"
touch "$FILE"

add() {
    echo -n "First Name: "; read first
    echo -n "Last Name: "; read last
    echo -n "Phone: "; read phone
    echo "$first $last:$phone" >> "$FILE"
    echo "Added"
}

search() {
    echo -n "Search: "; read term
    grep -i "$term" "$FILE" || echo "Not found"
}

sort_lastname() {
    [ -s "$FILE" ] && sort -t' ' -k2 "$FILE" || echo "Empty"
    echo -n "Save? (y/n): "; read s
    [ "$s" = "y" ] && sort -t' ' -k2 "$FILE" -o "$FILE"
}

delete() {
    echo -n "Term: "; read term
    sed -i "/$term/Id" "$FILE"
    echo "Deleted"
}

display() {
    [ -s "$FILE" ] && cat "$FILE" || echo "Empty"
}

while true; do
    echo -e "\n1.Add 2.Search 3.Sort 4.Delete 5.Display 6.Quit"
    read ch
    [ "$ch" = 1 ] && add
    [ "$ch" = 2 ] && search
    [ "$ch" = 3 ] && sort_lastname
    [ "$ch" = 4 ] && delete
    [ "$ch" = 5 ] && display
    [ "$ch" = 6 ] && exit
done