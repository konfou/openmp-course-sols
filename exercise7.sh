#!/usr/bin/env bash

N=5
n=500

compile() {
    if [[ ! -f $1 ]]; then
        echo $1 " not found; compiling now..."
        make $1
    fi
}

run-multiple() {
    for i in $(seq $N); do ./$1 $n &>/dev/null; done
}

compile ex7-imbalance
compile ex7-imbalance-openmp
echo

echo "CPU: " $(awk -F: '/model name/{print $2; exit}' /proc/cpuinfo)
echo

echo "using " $N " loops with length " $n

echo "running serial version..."
time run-multiple ex7-imbalance
echo

echo "running parall version..."
for s in 'static' 'dynamic' 'static,1'; do
    echo "-- using schedule " $s
    export OMP_SCHEDULE=$s
    time run-multiple ex7-imbalance-openmp
    echo
done
