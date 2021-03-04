# branch-mispredict
Tests CPU architecture capability to handle branch mispredicts.

Test sequence:
1. Download matrix code, compile with -O2, execute and log.
2. Download branch code, compile with -DNORAND3, execute and log
3. Compile branch code without -DNORAND3, execute and log.

To download:
1. Matrix.c
2. Branch.c

To compile:
Matrix.c with -O3, -O2, -Ofast
branch.c with -O3 / -O2 / -Ofast, -DNORAND3

Executables
1. Matrix-O3
2. Matrix-O2
3. Matrix-Ofast
4. Branch-O3
5. Branch-O2
6. Branch-Ofast
7. 
