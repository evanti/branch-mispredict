# branch-mispredict
Tests CPU architecture capability to handle branch mispredicts.

Test sequence:
1. Unconditional
2. 50% probability of branch mispredict
3. 90% probability of branch mispredict

## Overview

This repository contains a few small C benchmarks for measuring the impact of branch misprediction during matrix multiplication. The programs allocate large matrices, perform repeated multiply-add operations, and record performance statistics. Scripts are included for building/running these tests and capturing results.

```
README.md          – brief project description
branch.c           – benchmark with optional random branch checks
easyb.c            – benchmark with probability-controlled branch
matrix.c           – plain matrix multiplication benchmark
launch.sh          – compile/run easyb.c with several Q values
sh_branch.sh       – run branch.c for a short duration
easybstat.txt      – sample output from running launch.sh
```

The code does not use any external libraries beyond the standard C library.

## Key Implementation Points

1. **Control of randomness and branching** – `branch.c` uses macros such as `NORAND`, `NORAND1`, etc. to decide whether each loop level should execute unconditionally or use a random branch. `easyb.c` skips computations depending on a threshold `Q`.
2. **Allocation approach** – matrices are allocated either statically or dynamically depending on `N` to avoid large stack allocations.
3. **Usage patterns** – shell scripts compile the benchmarks with optimization and run them for a set time, printing statistics when interrupted.
4. **Sample output** – `easybstat.txt` contains example results from running `launch.sh`.

## What to Explore Next

1. Compile-time parameters controlling branches.
2. Changing matrix size `N` to test scaling.
3. Profiling loops using tools like `perf`.
4. Vectorization and optimization flags.
5. Comparing with branchless versions.
6. Adding automated tests.
