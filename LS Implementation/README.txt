Adam Ovadia
CSCI 493
LS Project

Compilation: make ls
Execute: ./myLs -<options> <path>

If no path is entered, default path is current working directory


List of implemented commands:

  -help: list the implemented options

  -l format: permission, # links, owner name, group name, number of bytes, date of last modification
 
 -R: Recursively lists all files/contents of dir in a specified path
 
 -a: Include DOT files and hidden files
 
 -A: Include hidden files
 
 -i: For each file, print the file's file serial number (inode number).
 
 -n: Display user and group IDs numerically, rather than converting to a user or group name in a long (-l) output.  This option turns on the -l option.
 
 -T:  When used with the -l (lowercase letter ``ell'') option, display complete time information for the file, including
 month, day, hour, minute, second, and year.
        --> for my implementation if you use T, I will turn on 'l' format
 
 -g: This option is only available for compatibility with POSIX; it is used to display the group name in
 the long (-l) format output (the owner name is suppressed).
 
 
 -h: When used with the -l option, use unit suffixes: Byte, Kilobyte, Megabyte, Gigabyte, Terabyte and Petabyte in
 order to reduce the number of digits to three or less using base 2 for sizes.
        --> Turns on option 'l' by default
 
 -o: List in long format, but omit the group id.
 

