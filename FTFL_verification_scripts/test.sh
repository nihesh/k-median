#/bin/sh

for i in {1..1000}
do
python3 gentest-fl.py > in.txt
./fl < in.txt > out.txt
./mcmf < in.txt > out1.txt

diff out.txt out1.txt
echo ====================
# cat in.txt
# cat out.txt
done