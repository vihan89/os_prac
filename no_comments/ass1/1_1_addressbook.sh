#!/bin/bash

FILE="address.txt"
touch "$FILE"

search() {
    echo -n "Search (1)ID (2)Name (3)Phone: "; read ch
    echo -n "Term: "; read term
    if [ "$ch" = 1 ]; then
        grep "^$term:" "$FILE"
    elif [ "$ch" = 2 ]; then
        grep -i ":$term:" "$FILE"
    elif [ "$ch" = 3 ]; then
        grep ":$term$" "$FILE"
    fi
}

add() {
    echo -n "ID: "; read id
    echo -n "Name: "; read name
    echo -n "Phone: "; read phone
    echo "$id:$name:$phone" >> "$FILE"
    echo "Added ID=$id"
}

remove() {
    echo -n "ID: "; read id
    sed -i "/^$id:/d" "$FILE"
    echo "Deleted"
}

while true; do
    echo -e "\n1.Search 2.Add 3.Remove 4.Quit"
    read ch
    if [ "$ch" = 1 ]; then
        search
    elif [ "$ch" = 2 ]; then
        add
    elif [ "$ch" = 3 ]; then
        remove
    elif [ "$ch" = 4 ]; then
        exit
    fi
done