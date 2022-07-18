// Author: Dennis Yakovlev

#!/bin/sh

# build node-gyp files

gyp_output="node-gyp-log"
config_log="gyp-config.txt"
build_log="gyp-build.txt"
log_dir=./$gyp_output

# flag: -L dir
# directory to output log files to

while getopts L: flag
do
    case "${flag}" in
            L) log_dir=${OPTARG};;   
            *) echo "Invalid option: -$flag" ;;
    esac
done

if [[ -d $gyp_output ]];
then
    echo Deleting old log dir...
    rm -d -r $gyp_output
fi

if [[ $log_dir == ./$gyp_output ]];
then
    echo Making log dir $gyp_output...
    mkdir $gyp_output
fi

echo Logging configure out to $config_log...
node-gyp configure &> $log_dir/$config_log

echo Logging build to $build_log...
node-gyp build &> $log_dir/$build_log

echo -e DONE '\n'