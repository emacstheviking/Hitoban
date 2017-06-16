@echo off
REM Creating a folder to contain the makefiles
cd /D F:/redesigned-happiness
cd /D F:/tools
REM Building makefiles for all OS, in the folder makefiles of the project.
REM They will all have the name make and their extension will be the OS
REM they are built for
cbp2make -in F:/redesigned-happiness/project.cbp -unix -out F:/redesigned-happiness/Makefile
cd /D F:/redesigned-happiness
copy /b Makefile+_makefileTests.txt Makefile