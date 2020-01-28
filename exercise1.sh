#!/usr/bin/env bash

N=100
n=10000000

compile() {
    if [[ ! -f $1 ]]; then
        echo $1 " not found; compiling now..."
        make $1
    fi
}

run-multiple() {
    for i in $(seq $N); do ./$1 $n &>/dev/null; done
}

compile ex1-vector-addition
compile ex1-vector-addition-openmp
echo

echo "CPU: " $(awk -F: '/model name/{print $2; exit}' /proc/cpuinfo)
echo

echo "using " $N " loops with length " $n

echo "running serial version..."
time run-multiple ex1-vector-addition
echo

echo "running parallel version..."
time run-multiple ex1-vector-addition-openmp
echo
