#RSHELL
Dharti Tarapara, Gian Ocampo

Overview
--------
RSHELL is a program that simulates a command shell or terminal. It is capable
of running basic commands such as ls, mkdir, rm, etc., and supports commands
that are followed flags as well. These commands can be input from the command
line one by one or as a chain of commands connected by SEMICOLONS ( ; ), AND'S
( && ), and OR'S ( || ).

To Run
------
$ git clone https://github.com/dtara003/rshell.git
$ cd rshell
$ git checkout hw1
$ make
$ bin/rshell

Limitations
-----------
* does not recognize the command cd
* does not handle redirective commands such as echo "hey" > test.cpp
* removes all occurances of extra spaces and &, |, and ; symbols directly before
  the first command and after the last command instead of just outputting an
  error
* user has the ability to run rshell within itself multiple times
* follows STRICT connector format:
    * more than one semicolon between two commands is treated as if the user is
      running blank commands in between
    * ls -a;;; mkdir direct
        * runs the command ls -a, two blank commands in between the three
          semicolons, and then mkdir direct
* AND and OR connectors are detected in groups of two only
    * ls -a & mkdir direct
        * does not recognize a single & as a connector and will try to execute
          the entirety of ls -a & mkdir direct as one command, outputting a
          FAILED status
    * ls -a | mkdir direct
        * does not recognize a single | as a connector and will try to execute
          the entirety of the command chain shown as one command
    * ls -a &&& mkdir direct
        * will execute ls -a, separate the second command from &&, and return a
          FAILED status after attempting to execute & mkdir direct
    * any amount of extra & or | symbols will be processed as if part of the
      second command and will cause the second command to output an error
