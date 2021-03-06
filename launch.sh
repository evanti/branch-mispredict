#!/bin/bash
#lscpu >> easybstat.txt
gcc -O3 -march=native -DQ=1.0 -mtune=native -o easyb easyb.c
echo $'\ngcc -O3 -march=native -DQ=01 -mtune=native -o easyb easyb.c' >> easybstat.txt
./easyb >> easybstat.txt &
sleep 30
pgrep easyb | xargs kill -2

gcc -O3 -march=native -mtune=native -o easyb easyb.c
echo $'\ngcc -O3 -march=native -mtune=native -o easyb easyb.c' >> easybstat.txt
./easyb >> easybstat.txt &
sleep 30
pgrep easyb | xargs kill -2

gcc -O3 -march=native -DQ=0.1 -mtune=native -o easyb easyb.c
echo $'\ngcc -O3 -march=native -DQ=0.1 -mtune=native -o easyb easyb.c' >> easybstat.txt
./easyb >> easybstat.txt &
sleep 30
pgrep easyb | xargs kill -2
