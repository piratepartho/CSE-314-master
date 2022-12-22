#!/bin/bash

recfile(){
    for file in *; do
        if [[ -d $file ]]; then
            cd "$file" || exit 1;
            recfile
            lastDir=$(pwd)
            noOfFiles=$(ls  | wc )
            cd ..
            [[ $noOfFiles -eq 0 ]] && (rmdir "$lastDir")
        elif [[ $file == *.txt ]]; then
            name=$(cat "$file" | head -1)
            country=$(cat "$file" | head -2 | tail -1)
            role=$(cat "$file" | head -4 | tail -1)
            mkdir -p "$mainDir/$country/$role"
            mv "$file" "$mainDir/$country/$role"
        fi
    done 
}

cd players
mainDir=$(pwd)
recfile