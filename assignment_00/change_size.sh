#!/usr/bin/bash


if [ "$#" -ne 2 ]
then
	echo "illegal number of argument"
	exit 1
fi
echo $STACK_SIZE
regexp='#define .*'
while read -r line
do


	       if [[ $line =~ $regexp  ]]
	       then 
		       echo "#define SIZE $2" >> tmp
	 	else
		echo $line >> tmp
	       fi


done <  $1

mv tmp $1
rm -f tmp	 


