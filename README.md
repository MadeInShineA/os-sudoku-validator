# os-sudoku-validator Lab

## Requirements

Install latest version of [C](https://installc.org/).

## stats.c
This program creates three threads that calculate the min, max and mean of differents numbers passed in arguments.

### How to compile
```bash
gcc stats.c -o stats -pthread
```

### How to run
```bash
./stats 12 34 -5
```

## sudoku_checker.c
This program check if a sudoku is valid with 11 threads. One thread check the rows, one the columns and nine other the squares.

### How to compile
```bash
gcc sudoku_checker.c -o sudoku -pthread
```

### How to run
```bash
./sudoku
```


## producer_consumer.c
This program runs a [producer/consumer pattern](https://en.wikipedia.org/wiki/Producer%E2%80%93consumer_problem) with two threads : one for the consumer and one for producer. The code also uses mutex and condition variable.

### How to compile
```bash
gcc producer_consumer.c -o producer -pthread
```

### How to run
```bash
./producer
```

## search.c
This program creates N threads to check if a character is inside an N*N matrix.

### How to compile
```bash
gcc search.c -o search -pthread
```

### How to run
```bash
./search
```
