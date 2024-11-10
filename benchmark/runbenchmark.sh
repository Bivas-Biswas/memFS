#!/usr/bin/bash
# make clean && make

operations=(100 1000 10000)
noOfInputFiles=1
noOfThreads=(1 2)

for thread in "${noOfThreads[@]}"; do
    for op in "${operations[@]}"; do
        for ((i=1; i<=noOfInputFiles; i++)); do
            inputFile="./$op/$i.txt"
            echo "------------------------------------------------------------------------------"
            echo "Number of Thread: $thread, Number of Operation: $op, InputFile: $inputFile"
            echo "------------------------------------------------------------------------------"
            ./benchmark "$inputFile" "$thread"
            sleep 5  # Pauses for 5 seconds
            wait $!
            # echo "------------------------------------------------------------------------------"
        done
    done
done
