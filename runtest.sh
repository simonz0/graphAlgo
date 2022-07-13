#!/bin/bash

g++ -std=c++14 prog2.cpp -O3 -o prog2

for file in $(cat ${1}); do
	if [ -r ${file}.args ]; then
		if [ -r ${file}.in ]; then
			${2} $(cat ${file}.args) < ${file}.in > ${file}.out
		else
			${2} $(cat ${file}.args) > ${file}.out
		fi
	else
		if [ -r tests/${file} ]; then
						echo "running test ${file} ..."
                        timeout 2 ./prog2 < tests/${file} > tests/${file}.out
                else
                        ${2} > ${file}.out
		fi
	fi
done
