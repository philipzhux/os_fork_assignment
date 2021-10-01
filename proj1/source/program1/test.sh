#!/bin/sh
ls|grep -v -e .c -e .h -e program1 -e Makefile -e .sh |
while read line; do
  # for example, split on whitespace once we have one line
  echo "############## Testing $line ############## "
  ./program1 ./$line
  echo "##############Testing $line completed ##############\n\n"
done