# Lab 2

# Note
Please don't use an input file that is made on Windows. There are issues when reading the file, something to do with file encoding 🤔🤔.

# Usage
``` 
$ make
$ ./myshell 
/SOFE3950-labs/Lab2/myshell $ cd <directory>
/SOFE3950-labs/Lab2/myshell $ clr
/SOFE3950-labs/Lab2/myshell $ dir
/SOFE3950-labs/Lab2/myshell $ environ
/SOFE3950-labs/Lab2/myshell $ echo <msg_here>
/SOFE3950-labs/Lab2/myshell $ help
/SOFE3950-labs/Lab2/myshell $ quit
```
## I/O Redirection
```
$ make
$ ./myshell < commands.txt 
$ ./myshell > out.txt 
$ ./myshell < commands.txt > out.txt 
$ cat commands.txt | ./myshell > out.txt
```