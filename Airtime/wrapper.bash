#!/bin/bash

r="0"
while [ $r != "index\.html" ]
do
    wget  http://challenge.airtime.com/$r?email=harsh@nyu.edu
    # Invoke the python script for computation
    r=`python airtime.py $r\?email\=harsh@nyu.edu`
    # Store the number in a file
    echo $r  >> sequence
    #Cleanup
    rm *\?email\=harsh@nyu.edu
    rm ./-*\?email\=harsh@nyu.edu
done


# Invoke the lis program on sequence

  g++ -o lis lis.cpp
  ./lis > ans


#End Script
