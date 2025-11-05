#!/bin/bash

FILE="phonebook.txt"
[ ! -f "$FILE" ] && touch "$FILE"

add() {
    echo "First Name: "; read first
    echo "Last Name: "; read last
    echo "Phone: "; read phone
    echo "$first $last:$phone" >> "$FILE"
    echo "Added"
}

search() {
    echo "Search term: "; read term
    grep -i "$term" "$FILE" || echo "Not found"
}

sort_lastname() {
    if [ -s "$FILE" ]; then
        sort -t' ' -k2 "$FILE"
        echo "Save sorted? (y/n): "; read save
        [ "$save" = "y" ] && sort -t' ' -k2 "$FILE" > temp && mv temp "$FILE"
    else
        echo "Empty"
    fi
}

delete() {
    echo "Delete term: "; read term
    if grep -qi "$term" "$FILE"; then
        grep -i "$term" "$FILE"
        echo "Delete? (y/n): "; read confirm
        [ "$confirm" = "y" ] && grep -vi "$term" "$FILE" > temp && mv temp "$FILE" && echo "Deleted"
    else
        echo "Not found"
    fi
}

display_all() {
    [ -s "$FILE" ] && cat "$FILE" || echo "Empty"
}

while true; do
    echo -e "\n1.Add 2.Search 3.Sort 4.Delete 5.Display 6.Quit"
    read choice
    case $choice in
        1) add ;;
        2) search ;;
        3) sort_lastname ;;
        4) delete ;;
        5) display_all ;;
        6) exit 0 ;;
    esac
done