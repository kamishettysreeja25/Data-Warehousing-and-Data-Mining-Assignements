#!/bin/bash
if [ $# -eq 2 ]; then
     python 201402200_1.py $1 $2
elif [ $# -eq 1 ]; then
   python 201402200_2.py $1
else
   echo "Wrong Number of aruguments. Try again"
fi
