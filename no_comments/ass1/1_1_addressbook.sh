#!/bin/bash

FILE="address.txt"
touch "$FILE"

search() {
    echo -n "Search (1)ID (2)Name (3)Phone: "; read ch
    echo -n "Term: "; read term
    [ "$ch" = 1 ] && grep "^$term:" "$FILE"
    [ "$ch" = 2 ] && grep -i ":$term:" "$FILE"
    [ "$ch" = 3 ] && grep ":$term$" "$FILE"
}

add() {
    id=1
    [ -s "$FILE" ] && id=$(($(tail -1 "$FILE" | cut -d: -f1) + 1))
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
    [ "$ch" = 1 ] && search
    [ "$ch" = 2 ] && add
    [ "$ch" = 3 ] && remove
    [ "$ch" = 4 ] && exit
done