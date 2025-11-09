#!/bin/bash

FILE="addressbook.txt"
touch "$FILE"

search() {
    echo -n "Search: "; read term
    grep -i "$term" "$FILE" || echo "Not found"
}

add() {
    echo -n "ID: "; read id
    echo -n "Name: "; read name
    echo -n "Address: "; read addr
    echo -n "Phone: "; read phone
    echo -n "Email: "; read email
    echo "$id|$name|$addr|$phone|$email" >> "$FILE"
    echo "Added ID=$id"
}

remove() {
    echo -n "ID: "; read id
    sed -i "/^$id|/d" "$FILE"
    echo "Deleted"
}

edit() {
    echo -n "ID: "; read id
    grep "^$id|" "$FILE" || { echo "Not found"; return; }
    echo -n "Name: "; read name
    echo -n "Address: "; read addr
    echo -n "Phone: "; read phone
    echo -n "Email: "; read email
    sed -i "/^$id|/d" "$FILE"
    echo "$id|$name|$addr|$phone|$email" >> "$FILE"
    sort -t'|' -k1 -n "$FILE" -o "$FILE"
    echo "Updated"
}

display() {
    [ -s "$FILE" ] && cat "$FILE" || echo "Empty"
}

while true; do
    echo -e "\n1.Search 2.Add 3.Remove 4.Edit 5.Display 6.Quit"
    read ch
    [ "$ch" = 1 ] && search
    [ "$ch" = 2 ] && add
    [ "$ch" = 3 ] && remove
    [ "$ch" = 4 ] && edit
    [ "$ch" = 5 ] && display
    [ "$ch" = 6 ] && exit
done