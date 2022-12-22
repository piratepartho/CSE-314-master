#!/bin/bash
IFS= read commands;

fd=()
filename=()

for ((i=0;i<commands;i++)); do
    fd+=(-1)
    filename+=(-1)
done

doOpenFile(){
    touch "$1"
    for ((j=0;j<commands;j++)); do
        if [[ ${fd[$j]} == -1 ]]; then
            fd[j]=1
            filename[j]=$1
            echo "open filename ${filename[$j]}"
            break
        fi
    done 
}

doCloseFile(){
    for ((p=0;p<commands;p++)); do
        if [[ ${filename[$p]} == "$1" ]]; then
            fd[$p]=-1
        fi 
    done 
}

doAppendFile(){
    file=${filename[$1]}
    echo "$file to be written with "$2""
    echo "$2" >> "$file"
}


for ((i=0;i<commands;i++));do
    read -r cmd1 cmd2 cmd3
    if [[ $cmd1 == "open" ]];then
        doOpenFile "$cmd2"
        echo "trying to open $cmd2"
    fi
    if [[ $cmd1 == "close" ]]; then
        doCloseFile "$cmd2"
    fi 
    if [[ $cmd1 == "append" ]]; then
        file=${filename[$cmd2]}
        doAppendFile "$cmd2" "$cmd3"
    fi 
done 