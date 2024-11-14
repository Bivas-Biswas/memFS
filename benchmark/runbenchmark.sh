#!/usr/bin/bash
make clean && make

# operations=(100 1000 10000)
operations=(100 1000 10000)
noOfInputFiles=1
noOfThreads=(1 2 4 8 16)
# noOfThreads=(1 2)
rootDir="input_files"
outLog="log.txt"
programfile="./benchmark"

if [ -f "$outLog" ]; then
    # echo "$outLog exists, removing it."
    rm "$outLog"
fi

for thread in "${noOfThreads[@]}"; do
    for op in "${operations[@]}"; do
        for ((i=1; i<=noOfInputFiles; i++)); do
            inputFile="./$rootDir/$op/$i.txt"
            echo "------------------------------------------------------------------------------" 2>&1 | tee -a "$outLog"
            echo "Number of Thread: $thread, Number of Operation: $op, InputFile: $inputFile" 2>&1 | tee -a "$outLog"
            echo "------------------------------------------------------------------------------" 2>&1 | tee -a "$outLog"
            "$programfile" "$inputFile" "$thread" 2>&1 | tee -a "$outLog"
            # sleep 5  # Pauses for 5 seconds
            wait $!
            # echo "------------------------------------------------------------------------------"
        done
    done
done
