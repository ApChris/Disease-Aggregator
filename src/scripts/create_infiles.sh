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

names=('John' 'David' 'Mary' 'Timothy' 'James' 'Eric' 'Jennifer' 'Andrew' 'Michael' 'Ronald' 'Maria' 'Evan' 'Mark' 'Alexa' 'Joseph')
surnames=('Williams' 'Sanders' 'Miller' 'Hale', 'Jacobs' 'Newman' 'Green' 'Aquirre' 'Gordon' 'Fisher' 'Morris' 'Lee' 'Gonzalez' 'Collins' 'Wood')
type=('ENTER' 'EXIT')
# read lines from countriesFile and create directory and subdirectories
i=1

# Initial starting date
starting_day=$((1+RANDOM%30))
starting_month=$((1+RANDOM%12))
starting_year=$((2010+RANDOM%10))

starting_day="24"
starting_month="12"
starting_year="2018"


# Initial variables that are going to hold current date
rand_day=$starting_day
rand_month=$starting_month
rand_year=$starting_year

# Initial arrays that are going to hold records for EXIT
array_exit_ids=()
array_exit_names=()
array_exit_surnames=()
array_exit_diseases=()
array_exit_ages=()

# Unique Record id
record_id=0
# For every line from countriesFile
while read line; do
    mkdir -p ../../etc/$path/$line # Create a subdirectory
    echo "$i) $line"

    # For numFilesPerDirectory
    for (( depth = 1; depth < $4; depth++)); do
        path2=$path/$line

        # if day is less or equal than 30
        if [ "$((rand_day+1))" -le 30 ]; then
            rand_day=$((rand_day+1))
            date="$rand_day-$rand_month-$rand_year"
        # if day is greater than 30 , it means that we have to change month, and set day again to 1
        elif [ "$((rand_month+1))" -le 12 ]; then
            rand_day="1"
            rand_month=$((rand_month+1))
            date="$rand_day-$rand_month-$rand_year"
        # else new year
        else
            rand_day="1"
            rand_month="1"
            rand_year=$((rand_year+1))
        fi

        date="$rand_day-$rand_month-$rand_year"
        # Create file with date as a name
        touch "../../etc/$path2/$date"

        # For numRecordsPerFile
        currentnumRecordsPerFile=0
        if [ "$currentnumRecordsPerFile" -lt "$5" ]; then
            if [ ${type[$((RANDOM%2))]} == 'ENTER' ]; then
                echo 'ENTER'
                record_id=$((record_id+1)) # += 1 the record_id because it must be a unique number

                disease=$(shuf -n 1 $1)

                echo $record_id 'ENTER' ${names[$((RANDOM%15))]} ${surnames[$((RANDOM%15))]} $disease $((1+RANDOM%120))>> "../../etc/$path2/$date"

                currentnumRecordsPerFile=$((currentnumRecordsPerFile+1))
            else

                # if array is empty then we can't pop something so we have to push
                if [ -z ${array_exit_ids[0]} ]; then
                    echo 'Not' ${array_exit_ids[0]} 'Y'
                else
                    echo 'Hello2'
                fi
            fi
            echo ${names[$((RANDOM%15))]} >> "../../etc/$path2/$date"
            currentnumRecordsPerFile=$((currentnumRecordsPerFile+1))
        fi

    done
    i=$((i+1))
done < $2
