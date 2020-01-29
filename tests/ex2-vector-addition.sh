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

compile ex1-vector-addition-openmp
compile ex2-vector-addition-openmp-ws
echo

echo "CPU: " $(awk -F: '/model name/{print $2; exit}' /proc/cpuinfo)
echo

echo "using " $N " loops with length " $n

echo "running explicit parallel version..."
time run-multiple ex1-vector-addition-openmp
echo

echo "running work-sharing parallel version..."
for s in 'static' 'dynamic' 'guided'; do
    echo "-- using schedule " $s
    export OMP_SCHEDULE=$s
    time run-multiple ex2-vector-addition-openmp-ws
    echo
done
