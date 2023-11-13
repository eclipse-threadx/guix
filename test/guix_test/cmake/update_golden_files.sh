#!/bin/bash

# This script is used to update the golden files for the failed tests.

# get the result file name from the first argument.
result_file=$1

# generate help message.
if [ "$result_file" == "-h" ] || [ "$result_file" == "--help" ]; then
    echo "Usage: $0 <result_file>"
    exit 1
fi

# generate help message if the result file name is not provided.
if [ -z "$result_file" ]; then
    echo "Usage: $0 <result_file>"
    exit 1
fi

# loop over lines in result file that contain "(Failed)".
for line in $(grep -o " - .* (Failed)" $result_file); do
    #if line contains "::".
    if [[ $line == *"::"* ]]; then
        # Split the line with character "::" and put the result in an array.
        IFS='::' read -ra array <<< "$line"

        # Set first element of the array to build_name.
        build_name=${array[0]}

        # Set third element of the array to test_name.
        test_name=${array[2]}

        # generate the output files directory name.
        output_files_dir=build/$build_name/regression/output_files

        # generate the golden files directory name.
        golden_files_dir=../golden_files

        # generate the test running directory name.
        test_runing_dir=build/$build_name/regression
        echo $test_runing_dir

        # for each test name, run the test like this "test_name -r -generate -checksum" from the test_runing_dir directory.
        echo "Running test $test_name"
        cd $test_runing_dir
        ./$test_name -r -generate -checksum
        cd ../../../

        # zip the generated binary file.
        7z a $output_files_dir/$test_name.7z $output_files_dir/$test_name.bin

        # move the generated binary file to the golden files directory.
        mv $output_files_dir/$test_name.7z $golden_files_dir
        mv $output_files_dir/$test_name.checksum $golden_files_dir
    fi
done
