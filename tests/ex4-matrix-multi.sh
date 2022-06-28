#!/usr/bin/env bash

N=10
n=1000

compile() {
    if [[ ! -f $1 ]]; then
        echo $1 " not found; compiling now..."
        make $1
    fi
}

run-multiple() {
    for i in $(seq $N); do ./$1 $n &>/dev/null; done
}

compile ex4-matrix-multi
compile ex4-matrix-multi-openmp
echo

echo "CPU: " $(awk -F: '/model name/{print $2; exit}' /proc/cpuinfo)
echo

echo "using " $N " loops with length " $n

echo "running serial version..."
time run-multiple ex4-matrix-multi
echo
echo "running parallel version..."
time run-multiple ex4-matrix-multi-openmp
echo
