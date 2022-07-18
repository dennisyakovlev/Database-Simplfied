// Author: Dennis Yakovlev

#!/bin/sh

# build files to debug and text files in /database

# flag: -c true
# to run cmake files 

run_cmake=""

while getopts c: flag
do
    case "${flag}" in
            c) run_cmake=${OPTARG};;   
            *) echo "Invalid option: -$flag" ;;
    esac
done

if [[ $run_cmake = "true" ]];
then
    rm -r out/build
    mkdir -p out/build
    cmake -S . -B out/build #assume in directory .../cpp
fi

echo Running make...
make --silent -C out/build

echo Running database initialization...
./out/build/PROB