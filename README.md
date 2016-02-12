#RSHELL
Dharti Tarapara, Gian Ocampo

#Overview
RSHELL is a program that simulates a command shell or terminal. It is capable
of running basic commands such as ls, mkdir, rm, etc., and supports commands
that are followed flags as well. These commands can be input from the command
line one by one or as a chain of commands connected by SEMICOLONS ( ; ), AND'S
( && ), and OR'S ( || ).

#To Run

#Limitations

- does not recognize the command cd
- more than one semicolon between two commands is treated as if the user is
  running blank commands in between
    - ls -a;;; mkdir direct
        - runs the command ls -a, two blank commands in between the three
          semicolons, and then mkdir direct
- AND and OR connectors are detected in groups of two only
    - ls -a && mkdir direct
        - runs mkdir direct if ls -a executes and does no return an EXECUTED
          status
    - ls -a & mkdir direct
        - does not recognize a single & as a connector and will try to execute
          the entirety of ls -a & mkdir direct as one command, outputting an
          returning a 
