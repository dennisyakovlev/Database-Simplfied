// Author: Dennis Yakovlev

#!/bin/sh

# create doxygen documentation

# flag: -c y
# to make config file
# just best to set to yes everytime

# flag: -I dir
# directory to document

# flag: -L dir
# directory to output log files to
shopt -s extglob

doxy_dir="doxygen"
config_name="config.doxy"
doxy_output="doxy-log"
config_log="doxy-config.txt"
output_log="doxy-output.txt"
log_dir="$PWD/$doxy_output"
log_name="doxy-warnings.txt"

doxy_input="INPUT                  ="
doxy_quiet="QUIET                  ="
doxy_extract="EXTRACT_ALL            ="
doxy_warn="WARN_LOGFILE           ="
doxy_dot="HAVE_DOT               ="
doxy_latex="GENERATE_LATEX         ="

if [[ -d $doxy_dir ]];
then
    echo Changing folders...
    cd $doxy_dir

    echo Deleting old non config files...
    touch ./$doxy_dir/temp.txt
    rm -d -r !(config.doxy)

    echo Changing folders...
    cd ..
else
    echo Making doxygen file output dir...
    mkdir $doxy_dir
fi

dir=""

while getopts I:L: flag
do
    case "${flag}" in
            I) dir=${OPTARG};; # possible automatically remove ending / if was added
            L) log_dir=${OPTARG};;
            *) echo "Invalid option: -$flag";;
    esac
done

if [[ -d $doxy_output ]];
then
    echo Deleting old log dir...
    rm -d -r $doxy_output
fi

if [[ $log_dir == $PWD/$doxy_output ]];
then
    echo Making log dir $doxy_output...
    mkdir $doxy_output
fi

if [[ -e ./$doxy_dir/$config_name ]];
then
    echo Deleteing old config file...
    rm ./$doxy_dir/$config_name
fi

echo Logging configure out to $config_log...
doxygen -g ./$doxy_dir/$config_name &> $log_dir/$config_log

# extract all by default
echo Extracting all 'set' to yes...
sed -i "s/$doxy_extract.*/$doxy_extract YES/" ./$doxy_dir/$config_name

# Set log file
echo Setting log file name to $log_name...
new_log_dir=$(echo $log_dir | sed 's|\/|\\/|g')
sed -i "s/$doxy_warn/$doxy_warn$new_log_dir\/$log_name/" ./$doxy_dir/$config_name

# Do not have dot by default
echo Setting doxygen dot graph visualizer to no...
sed -i "s/$doxy_dot.*/$doxy_dot NO/" ./$doxy_dir/$config_name

# Do not generate LaTeX files by default
echo Do not generate latex by default...
sed -i "s/$doxy_latex.*/$doxy_latex NO/" ./$doxy_dir/$config_name

if [[ $dir != "" ]];
then
    new_input=".\/..\/"${dir//\//\\\/}
    echo Setting input directory 'in' config to $dir...
    sed -i "s/$doxy_input.*/$doxy_input$new_input/" ./$doxy_dir/$config_name
fi

echo Changing folders...
cd $doxy_dir

echo Running doxygen...
echo Logging doxygen output to $output_log...
echo Logging doxygen warnings and errors to $log_dir/$log_name...
doxygen $config_name &> $log_dir/$output_log

echo Changing folders...
cd ..

echo Deleting config file...
rm ./$doxy_dir/$config_name

echo -e DONE '\n'