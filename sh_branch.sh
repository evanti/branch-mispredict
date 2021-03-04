#!/bin/bash
./branch >> branch.out &
sleep 10
pgrep branch | xargs kill -2
