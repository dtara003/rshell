#!/bin/sh

echo hello && exit 
echo hello && exit; date && git status 
exit && echo hello; ls < ./a.out
exit || ls || ls -a < ./a.out
date && git status || exit && echo hello 
xyz && exit
exit
echo hello && xyz && exit || ls 
mkdir Dir && exit || rm -r Dir
exit; exit; exit; exit;
touch test.cpp || exit && vim test.cpp
touch test.cpp; g++ test.cpp; exit; ./a.out
invalid || exit;
xyz || exit || echo hello
ls; ls; ls; exit
cat test.cpp; exit;
echo hello; exit && exit
ls || exit && xyz
