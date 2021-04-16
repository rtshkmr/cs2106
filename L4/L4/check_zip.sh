#!/bin/bash

function check_files {
    for i do
        #echo [$i]
        if [ ! -f $i ]; then
            return 1
        fi
    done
}

function check_compile {
    x=1
    cd $1
    make &> /dev/null
    #gcc -std=c99 -Wall -Wextra *.c -o exTest &> /dev/null
    #gcc -std=c99 -Wall -Wextra *.c -o exTest
    x=$?
    cd ../
    [[ $x -ne 0 ]]
}

function run_tests {
    tests=("test1" "test2")
    cd $1
    #get the makefile to produce "exTest"
    gcc -std=c99 -Wall -Wextra *.c -o exTest &> /dev/null
    for test in "${tests[@]}"; do
        #./exTest < $test.in | diff $test.out - 
        ./exTest < $test.in | diff $test.out - &> /dev/null
        res=$?

        if [[ res -ne 0 ]]; then
            echo "Test $test.in failed"
            break
        fi
    done
    cd ../
}

function transfer_files {
    local ex2=("makefile" "mmalloc.h" "mmalloc_*.c") 
    local ex3=("makefile" "mmalloc_*.c") 
    local ex4=("makefile" "mmalloc_*.c") 
    local ex5=("makefile" "mmalloc.h" "mmalloc_*.c") 

    declare -n files="$3"

    for file in "${files[@]}"; do
        cp $1/$2/$3/$file $3 &> /dev/null
    done
}

echo "Checking zip file...."

if [ "$#" -ne 1 ]; then
    echo "You should provide your zip file as a parameter and nothing else!"
    exit 1
fi

if ! [[ $1 =~ ^A[0-9]{7}[A-Z].zip$ ]]; then
    echo "zip file is wrongly named: it should be <STUDNET ID>.zip"
    exit 1
fi

echo "Unzipping file: $1"
if [ ! -f $1 ]; then
    echo "File $1 does not exist."
    exit 1
fi

tmp_folder="test_grading_aeN332Hp"
rm -rf $tmp_folder
mkdir $tmp_folder

cp $1 $tmp_folder

cd $tmp_folder
unzip $1 > /dev/null

echo "Transferring necessary skeleton files"
sk="skeleton"
mkdir $sk
cd $sk
wget "https://www.comp.nus.edu.sg/~sooyj/cs2106_2021s2/lab4.tar.gz" 2> /dev/null
tar -zxvf lab4.tar.gz > /dev/null
cd ../

exercises=(ex2 ex3 ex4 ex5)
ex2=("ex2/ex2.c") 
ex3=("ex3/ex3.c" "ex3/mmalloc.h")
ex4=("ex4/ex4.c" "ex4/mmalloc.h")
ex5=("ex5/ex5.c")
for ex in "${exercises[@]}"; do
    declare -n files=$ex
    if [ ! -d $ex ]
    then
        echo "$ex: Failed - $ex folder missing"
        continue
    fi

    if ! check_files "${files[@]}"
    then
        echo "$ex: Failed - files missing"
        continue
    fi

    transfer_files $sk "L4" $ex 
    if check_compile $ex
    then
        echo "$ex: Failed - does not compile"
        continue
    fi

    echo "$ex: Success"
done

cd ../
rm -rf $tmp_folder
