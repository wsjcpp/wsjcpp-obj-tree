#!/bin/bash

check_ret() {
    if [ $1 -ne 0 ]; then
        echo ""
        echo "!!! FAIL: $3"
        echo "********************************************************************************"
        echo ""
        exit $1
    else
        echo ""
        echo "*** SUCCESS: $2"
        echo "********************************************************************************"
        echo ""
    fi
} 

./build_simple.sh
check_ret $? "build wsjcpp-obj-core"
./wsjcpp-obj-tree -e -o some.wsjcpp-obj-tree
check_ret $? "example write to some.wsjcpp-obj-tree"
./wsjcpp-obj-tree -r 1000 -o random.wsjcpp-obj-tree
check_ret $? "example write to random.wsjcpp-obj-tree"
./wsjcpp-obj-tree -i random.wsjcpp-obj-tree -o some2.wsjcpp-obj-tree
check_ret $? "example read from random.wsjcpp-obj-tree write to some2.wsjcpp-obj-tree"
rm some.wsjcpp-obj-tree
check_ret $? "rm some.wsjcpp-obj-tree"
rm some2.wsjcpp-obj-tree
check_ret $? "rm some2.wsjcpp-obj-tree"
rm random.wsjcpp-obj-tree
check_ret $? "rm random.wsjcpp-obj-tree"

cd ./unit-tests.wsjcpp
check_ret $? "change directory to ./unit-tests.wsjcpp"
./build_simple.sh
check_ret $? "build unit-tests"
./unit-tests
check_ret $? "unit-tests"
cd ..