#!/bin/bash

FILE="addressbook.txt"
[ ! -f "$FILE" ] && touch "$FILE"

search() {
    echo "Search term: "; read term
    grep -i "$term" "$FILE" || echo "Not found"
}

add() {
    id=$(($([ -s "$FILE" ] && tail -1 "$FILE" | cut -d'|' -f1 || echo 0) + 1))
    echo "Name: "; read name
    echo "Address: "; read addr
    echo "Phone: "; read phone
    echo "Email: "; read email
    echo "$id|$name|$addr|$phone|$email" >> "$FILE"
    echo "Added ID=$id"
}

remove() {
    echo "Enter ID: "; read id
    if grep -q "^$id|" "$FILE"; then
        grep "^$id|" "$FILE"
        echo "Delete? (y/n): "; read confirm
        [ "$confirm" = "y" ] && grep -v "^$id|" "$FILE" > temp && mv temp "$FILE" && echo "Deleted"
    else
        echo "Not found"
    fi
}

edit() {
    echo "Enter ID: "; read id
    if grep -q "^$id|" "$FILE"; then
        line=$(grep "^$id|" "$FILE")
        echo "Current: $line"
        echo "Name: "; read name
        echo "Address: "; read addr
        echo "Phone: "; read phone
        echo "Email: "; read email
        grep -v "^$id|" "$FILE" > temp
        echo "$id|$name|$addr|$phone|$email" >> temp
        sort -t'|' -k1 -n temp > "$FILE"
        rm temp
        echo "Updated"
    else
        echo "Not found"
    fi
}

display() {
    [ -s "$FILE" ] && cat "$FILE" || echo "Empty"
}

while true; do
    echo -e "\n1.Search 2.Add 3.Remove 4.Edit 5.Display 6.Quit"
    read choice
    case $choice in
        1) search ;;
        2) add ;;
        3) remove ;;
        4) edit ;;
        5) display ;;
        6) exit 0 ;;
    esac
done