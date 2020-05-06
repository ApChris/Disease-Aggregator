#!/bin/bash

# Check arguments
if [ "$#" -ne 5 ]; then                             # if arguments are not equal with 5 arguments then exit
    echo "Error: Please give the correct number of arguments"
    exit 1
fi

# after arguments checking we should have these values
# $1 = diseaseFile, $2 = countriesFile, $3 = input_dir , $4 = numFilesPerDirectory, $5 = numRecordsPerFile
echo "Script name: $0"


input_dir=("$3") # store the name of directory

input_dir[1]="$4"
echo ${input_dir[@]}

path=$3



# read lines from countriesFile and create directory and subdirectories
i=1
echo "countriesFile"
while read line; do
    mkdir -p $path/$line
    echo "$i) $line"
    for (( depth = 1; depth < $4; depth++)); do


    done
    i=$((i+1))
done < $2





# read lines from diseaseFile
i=1
echo "diseaseFile"
while read line; do
echo "$i) $line"
i=$((i+1))
done < $1
