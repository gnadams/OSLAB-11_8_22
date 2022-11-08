To compile my source code:
1st -> add CMakeLists.txt and  Lab1.c to a directory titled "OSLabs".

2nd -> inside OSLabs directory use "mkdir build" to create a build folder

3rd -> use "cd build" to navigate into the build directory.

4th -> use "cmake .." to start building the files

5th -> use "make" to create the executables.

6th -> use "./Lab1" to open the shell!

NOTE: the minimum version required for this program to run is with CMake Version 3.10.2.
	If you are using an older version you may have to edit the CMakeLists.txt file.


linux~gna5 - man page


*Description:

linux~gna5 is a simple stripped down linux shell to execute a few basic commands. 
Some of these commands could be called where as others required a little bit more work.

*Commands: 

C file1 file2		-Copies file, file1 is the file to be copied; file2 is the destination.

D file			-Deletes the named file

E comment		-Will display the comment on the screen, followed by a new line.

H			-Will the display the user manual.

L 			-Lists the current directory

M file			-will create the named text file via nano.

P file			-will print the contents of a text file to the screen.

Q			-will quit out of the shell.

W			-wipes the screen.

X program		-will execute the named program.

-----------------------------------------------------------------------------

I utilized resources from "Brian Fraser" for learning the basics of the fork and exec system calls.
2 Of the videos that I watched are linked below:

https://www.youtube.com/watch?v=mj2VjcOXXs4&ab_channel=BrianFraser

https://www.youtube.com/watch?v=9seb8hddeK4&ab_channel=BrianFraser
