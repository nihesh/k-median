#!/bin/bash

for i in {1..1000}
do
echo $i
python3 gentest-kmedian.py > in.txt
cat in.txt | ./$1 > out1.txt
cat in.txt | ./$2 > out2.txt
diff out1.txt out2.txt
echo "---------------"
done