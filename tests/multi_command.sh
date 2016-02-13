#!/bin/sh

echo hello || ls && date; #OR and AND with valid commands
ls && xyz && echo hey && date #three ANDS with one invalid command
ls -a ||  sup && git branch < #OR and AND with one invalid command
cd Dir && mkdir Dir # cd and mkdir
mkdir Dir && rm -r Dir  # mdkir then remove it
mkdir Dir && rm -r Dir; mkdir Dir;
touch test.cpp && rm test.cpp # make a file then remove it
echo "hello" && xyz; echo world
git status && git branch # git commands
touch test.cpp; vim test.cpp;
ls; ls -a;
cat test.cpp || rm test.cpp;
touch test.cpp; xyz; vim test.cpp
xyz && echo hello && echo hello; xyz || echo hello && echo world && echo hello
git add; git commit; git push; 
mkdir Dir; cd Dir; touch test.cpp; vim test.cpp
touch test.cpp && g++ test.cpp; ./a.out;
make || g++ test.cpp;

