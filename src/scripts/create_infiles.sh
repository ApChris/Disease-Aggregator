#!/bin/bash

# Check arguments
if [ "$#" -ne 5 ]; then                             # if arguments are not equal with 5 arguments then exit
    echo "Error Input: Please give the correct number of arguments"
    exit 1
fi

# after arguments checking we should have these values
# $1 = diseaseFile, $2 = countriesFile, $3 = input_dir , $4 = numFilesPerDirectory, $5 = numRecordsPerFile

if [ "$4" -le 0 ]; then                             # check 4th argument
    echo "Error Input:  4/5th arguments have to be > 0 "
    exit 1
fi

if [ "$5" -le 0 ]; then                             # check 5th argument
    echo "Error Input:  4/5th arguments have to be > 0 "
    exit 1
fi

path=$3

names=('John' 'David' 'Mary' 'Timothy' 'James' 'Eric' 'Jennifer' 'Andrew' 'Michael' 'Ronald' 'Maria' 'Evan' 'Mark' 'Alexa' 'Joseph')
surnames=('Williams' 'Sanders' 'Miller' 'Hale' 'Jacobs' 'Newman' 'Green' 'Aquirre' 'Gordon' 'Fisher' 'Morris' 'Lee' 'Gonzalez' 'Collins' 'Wood')
type=('ENTER' 'EXIT')
# read lines from countriesFile and create directory and subdirectories
i=1

# Initial starting date
starting_day=$((1+RANDOM%30))
starting_month=$((1+RANDOM%12))
starting_year=$((2010+RANDOM%10))

dates=()

starting_day="24"
starting_month="12"
starting_year="2018"

# Initial variables that are going to hold current date
rand_day=$starting_day
rand_month=$starting_month
rand_year=$starting_year

counterOfCountries=0

# Sort countries File
sort $2 > ../../etc/tmp.txt
cp ../../etc/tmp.txt $2
rm -r ../../etc/tmp.txt

# For every line from countriesFile
while read line; do
    mkdir -p ../../etc/$path/$line # Create a directory/subdirectory
    # echo "$i) $line"

    # For numFilesPerDirectory
    for (( depth = 0; depth < $4; depth++)); do
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
        dates+=($date)
        # Create file with date as a name
        touch "../../etc/$path2/$date"

        # echo $date
    done
    counterOfCountries=$((counterOfCountries+1)) # We need this counter to know the total records
    i=$((i+1))
done < $2

# -------------------------- Create Recods ----------------------------------

# Initial arrays that are going to hold records for ENTER/EXIT
array_Enter_RecordID=()
array_Enter_Names=()
array_Enter_Surnames=()
array_Enter_Diseases=()
array_Enter_Ages=()

array_Exit_RecordID=()
array_Exit_Names=()
array_Exit_Surnames=()
array_Exit_Diseases=()
array_Exit_Ages=()



# Unique Record id
record_id=0


totalRecords=$(($4*$5 *$counterOfCountries)) # total Records

# For totalRecords
for (( i = 0; i < $totalRecords; i++)); do

    record_id=$((record_id+1)) # += 1 the record_id because it must be a unique number
    name=${names[$((RANDOM%15))]}
    surname=${surnames[$((RANDOM%15))]}
    disease=$(shuf -n 1 $1) # Pick a random line from diseaseFile
    age=$((1+RANDOM%120))

    # Push in Enter array
    array_Enter_RecordID+=($record_id)
    array_Enter_Names+=($name)
    array_Enter_Surnames+=($surname)
    array_Enter_Diseases+=($disease)
    array_Enter_Ages+=($age)

    # Push in Exit Array
    array_Exit_RecordID+=($record_id)
    array_Exit_Names+=($name)
    array_Exit_Surnames+=($surname)
    array_Exit_Diseases+=($disease)
    array_Exit_Ages+=($age)


done

counterOfEnters=0
counterOfExits=0
counterOfDate=0
halfrecords=$((totalRecords/2))
# Fill the files with records
while read line; do

    path2=$path/$line
    for (( j = 0; j < $4; j++)); do

        date=$(ls ../../etc/$path2 | awk 'NR=='$((j+1)))
        date=${dates[$counterOfDate]}
        counterOfDate=$((counterOfDate+1))
        path3=$path2/$date

        for (( k = 0; k < $5; k++)); do
            if [ "$counterOfEnters" -eq 0 ]; then # Only for first record
                echo ${array_Enter_RecordID[$counterOfEnters]} 'ENTER' ${array_Enter_Names[$counterOfEnters]} ${array_Enter_Surnames[$counterOfEnters]} ${array_Enter_Diseases[$counterOfEnters]} ${array_Enter_Ages[$counterOfEnters]}>> "../../etc/$path3"
                counterOfEnters=$((counterOfEnters+1))
            else
                coin=$((RANDOM%2))
                # echo $coin
                if [ "$coin" -eq 0 ]; then   # ENTER
                    if [[ $counterOfEnters -lt $halfrecords ]]; then # if current enters are less than half records then you can push
                        echo ${array_Enter_RecordID[$counterOfEnters]} 'ENTER' ${array_Enter_Names[$counterOfEnters]} ${array_Enter_Surnames[$counterOfEnters]} ${array_Enter_Diseases[$counterOfEnters]} ${array_Enter_Ages[$counterOfEnters]}>> "../../etc/$path3"
                        counterOfEnters=$((counterOfEnters+1))
                    else # else you can't put more so you have to push an exit record
                        if [[ $counterOfExits -lt $halfrecords ]]; then
                            echo ${array_Exit_RecordID[$counterOfExits]} 'EXIT' ${array_Exit_Names[$counterOfExits]} ${array_Exit_Surnames[$counterOfExits]} ${array_Exit_Diseases[$counterOfExits]} ${array_Exit_Ages[$counterOfExits]}>> "../../etc/$path3"
                            counterOfExits=$((counterOfExits+1))
                        fi
                    fi

                else                        # EXIT
                    if [[ $counterOfExits -lt $counterOfEnters ]]; then   # if exit records are less than enter's
                        echo ${array_Exit_RecordID[$counterOfExits]} 'EXIT' ${array_Exit_Names[$counterOfExits]} ${array_Exit_Surnames[$counterOfExits]} ${array_Exit_Diseases[$counterOfExits]} ${array_Exit_Ages[$counterOfExits]}>> "../../etc/$path3"
                        counterOfExits=$((counterOfExits+1))
                    else # else put that records in enters
                        echo ${array_Enter_RecordID[$counterOfEnters]} 'ENTER' ${array_Enter_Names[$counterOfEnters]} ${array_Enter_Surnames[$counterOfEnters]} ${array_Enter_Diseases[$counterOfEnters]} ${array_Enter_Ages[$counterOfEnters]}>> "../../etc/$path3"
                        counterOfEnters=$((counterOfEnters+1))
                    fi

                fi
            fi

        done
    done
    i=$((i+1))
done < $2
