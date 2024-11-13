#!/usr/bin/bash
make clean && make

operations=(100 1000 10000)
noOfInputFiles=1

# generate filenames
rootDir="input_files"
fileNamesRoot="$rootDir/filenames"
mkdir -p "$fileNamesRoot"

for op in "${operations[@]}"; do
    ./generateinputfilenames "$op" "$fileNamesRoot/$op.txt"
done

# generate operations

for op in "${operations[@]}"; do
    mkdir -p "./$rootDir/$op"
    for ((i=1; i<=noOfInputFiles; i++)); do
        ./generateOperationFileInput "./$fileNamesRoot/$op.txt" "./$rootDir/$op/$i.txt"
    done
done
