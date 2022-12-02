# Advent of Code 2022

## Requirements

* GCC 12 (older GCC compatible with C++20 should work with little adjustments on `Makefile`)
* Make

## Build

For each day there is a folder `Day<number of the day>`

In each folder there is a `Makefile` with the following targets:

* `p1`, `p2`: build part one and part two with debug enabled
* `p1_fast`, `p2_fast`: build part one and part two with optimizations
* `p1_alt{_fast}`, `p2_alt__fast_`: build an alternative part one/two (with optimization if `_fast`)
* `all`: builds `p1` and `p2` together
* `alt`: builds `p1_alt` and `p1_alt` together
* `fast`: builds `p1_fast` and `p2_fast` together
* `alt_fast`: builds `p1_alt_fast` and `p2_alt_fast` together

## Run

`./part[1,2]{_alt}.out input{.test}`

Examples:

* `./part1.out input.test`  
Runs part one with test input
* `./part1_alt.out input`  
Runs alternative part one with real input
* `./part2.out input`  
Runs part two with real input

## Note

There is a utility library under `Utils/`. It gets recycled every year, not the best stuff around, but I use it to process input file in a fast (and dirty) way.