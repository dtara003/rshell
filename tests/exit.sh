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
test exit
test -f exit
test -d exit
test -e exit
test -d /home/csmajs/gocam001/CS100 || exit
exit test
(exit)
exit []
[ -f /home/csmajs/gocam001/CS100/rshell/src/Command.h ] exit
[ exit ]
exit && (echo A || [ -d /home/csmajs/gocam001/rshell/src])
pwd && (exit || (ls && ls -a));
(exit;)
(exit&&)
echo hello; (date && (mkdir Dir && ls) || exit);
(sadfsad || exit)
(exit && (pwd || date))
(echo Hello && safasfa) && exit && echo This ran
(date && echo Hello) && (exit && ls -a) || echo This ran
e(xit) || exit && echo This ran
pwd || (test -e) exit

