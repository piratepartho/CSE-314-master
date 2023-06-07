#!/bin/bash

myArray=()
max_student_id=5

### to find out the point of the player
calculate()
{
    point=$max_score
    while read line; do
        # echo $line
        if [[ $line = \>* || $line = \<* ]]; then
            ((point=point-5))
            if [[ point -lt 0 ]]; then point=0; fi
        fi
    done
    myArray+=($point)
    # echo $point
    return
}

if ! [[ -z $1 ]];then
    max_score=$1
fi

if ! [[ -z $2 ]]; then
    max_student_id=$2
fi

cd Submissions/ || { echo "No submission Folder"; exit 1; }

for ((i=1;i<=max_student_id;i++));do
    cd 180512"${i}" || { myArray+=(0); continue; }
    bash ./180512"${i}".sh > out.txt || { myArray+=(0); continue; }
    diff out.txt ../../AcceptedOutput.txt -w > changes.txt
    calculate i < changes.txt
    rm out.txt changes.txt
    cd ..
done

##copy checker
for ((i=1; i<=max_student_id; i++));do
    for(( j=i+1; j<=max_student_id; j++ )); do
        if [[ j -gt 9 ]]; then continue; fi
        diff 180512${i}/180512${i}.sh 180512${j}/180512${j}.sh -B -Z -q > status.txt && {
        # myArray[i]=-${myArray[i]}; echo "found, $i , $j";
        ind_i=$((i-1)) 
        ind_j=$((j-1))
        if [[ myArray[$ind_i] -gt 0 ]]; then myArray[$ind_i]=-${myArray[$ind_i]}; fi
        if [[ myArray[$ind_j] -gt 0 ]]; then myArray[$ind_j]=-${myArray[$ind_j]}; fi
    }
    done
done

rm status.txt


## create the csv file
cd .. #go to parent directory
echo "student_id,score" > output.csv
for i in "${!myArray[@]}"; do
    echo "180512$((${i}+1)),${myArray[i]}" >> output.csv
done