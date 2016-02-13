#!/bin/sh

#go to rshell
cd ..
#make(if you didnt make already)
make
#go to src
cd src

#echo will return ls which will go into ./a.out's input
echo "ls" | ./a.out

#if you want to echo the echo command echo twice
#for example
echo "echo hello" | ./a.out

#make -C ..
#./../a.out << 'EOF'
#ls
