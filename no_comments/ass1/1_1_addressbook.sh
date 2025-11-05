#!/bin/bash

FILE="address.txt"
[ ! -f "$FILE" ] && touch "$FILE"

search() {
    echo "Search by (1)ID (2)Name (3)Phone: "
    read choice
    echo "Enter term: "
    read term
    case $choice in
        1) grep "^$term:" "$FILE" ;;
        2) grep -i ":$term:" "$FILE" ;;
        3) grep ":$term$" "$FILE" ;;
    esac
}

add() {
    if [ -s "$FILE" ]; then
        id=$(($(tail -1 "$FILE" | cut -d: -f1) + 1))
    else
        id=1
    fi
    echo "Name: "; read name
    echo "Phone: "; read phone
    echo "$id:$name:$phone" >> "$FILE"
    echo "Added: ID=$id"
}

remove() {
    echo "Enter ID: "; read id
    if grep -q "^$id:" "$FILE"; then
        grep "^$id:" "$FILE"
        echo "Delete? (y/n): "; read confirm
        [ "$confirm" = "y" ] && grep -v "^$id:" "$FILE" > temp && mv temp "$FILE" && echo "Deleted"
    else
        echo "Not found"
    fi
}

while true; do
    echo -e "\n1.Search 2.Add 3.Remove 4.Quit"
    read choice
    case $choice in
        1) search ;;
        2) add ;;
        3) remove ;;
        4) exit 0 ;;
    esac
done