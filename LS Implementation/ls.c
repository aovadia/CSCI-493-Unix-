// Adam Ovadia
// CSCI 493
// LS Program
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <time.h>

#include "real_user.h"
#include "permissions.h"
#include "stat.h"
#include "options.h"

#include "myDir.c"

void fillUsrOpts(char *input, struct options* myOpts);
void getPermissions(char *path);

int main (int argc, char *argv[]) {
    if (argc == 1) { /*if no arguments */
        char *rel_path[1024];
        getcwd((char *)rel_path, 1024);
        optLOutPut((char *)rel_path, &usr_opts);
        exit(0);
    }
    
    if (strcmp(argv[1], "-help") == 0) {
        printf("List of the 10 available options:\n");
        printf(" -l  format: permission, # links, owner name, group name, number of bytes, date of last modification\n");
        printf(" -R: Recursively lists all files/contents of dir in a specified path\n");
        printf(" -a: Include DOT files and hidden files\n");
        printf(" -A: Include hidden files\n");
        printf(" -i: For each file, print the file's file serial number (inode number)\n");
        printf(" -T: When used with the -l (lowercase letter l) option, display complete time information for the file, \n\tincluding month, day, hour, minute, second, and year. \n\t  --> for my implementation if you use T, I will turn on 'l' format \n");
        printf(" -g: This option is only available for compatibility with POSIX; it is used to display the group name in the \n\t long (-l) format output (the owner name is suppressed).\n");
        printf(" -h: When used with the -l option, use unit suffixes:\n\t Byte, Kilobyte, Megabyte, Gigabyte, Terabyte and Petabyte\n\t in order to reduce the number of digits to three or less using base 2 for sizes. \n\t--> Turns on option 'l' by default\n");
        printf(" -o: List in long format, but omit the group id.\n");
        printf(" -n: Display user and group IDs numerically, rather than converting to a user or group name \n\t in a long (-l) output.  This option turns on the -l option.\n");
        exit(0);
    }
    
    if (argc > 3) {
        printf("Usage: ./myLs <-options> <path> \n");
        exit(0);
    }
  
    char *arg1 = argv[1];
    char *arg2 = argv[2];
    
    if (!arg2) {
        char *rel_path[1024];
        getcwd((char *)rel_path, 1024);
        strcat((char *)rel_path, "/");
        usr_opts.FULL_PATH = (char *)rel_path;
    }
    
    if (argc == 2) {
        if (arg1[0] == '-') {
            fillUsrOpts(arg1, &usr_opts);
        }
        else {
            //this is the path
            usr_opts.FULL_PATH = (char *)malloc(strlen(arg1));
            usr_opts.FULL_PATH = arg1;
        }
    }
    else {
        fillUsrOpts(arg1, &usr_opts);
        usr_opts.FULL_PATH = arg2;
    }
    
    
   if (usr_opts.opt_A && usr_opts.opt_a) {
        printf("Error. You entered conflicting types: Please use -a or -A but not both! \n");
        exit(0);
    }
   
    if (usr_opts.opt_R) {
       Rec_Path(usr_opts.FULL_PATH, &usr_opts);
   }

    else  {
        printf("Entering path: %s\n", usr_opts.FULL_PATH);
        optLOutPut(usr_opts.FULL_PATH, &usr_opts);
    }
}

void fillUsrOpts(char *input, struct options* myOpts) {
    usr_opts.count = strlen(input) - 1;
    printf("total arfs: %d\n", usr_opts.count);
    for (int a = 1; a <strlen(input); a++) {
        if (input[a] == 'a') {
            myOpts->opt_a = true;
        }
        else if (input[a] == 'l') {
            myOpts->opt_l = true;
        }
        else if (input[a] == 'R') {
            myOpts->opt_R = true;
        }
        else if (input[a] == 'A') {
            myOpts->opt_A = true;
        }
        else if (input[a] == 'i') {
            myOpts->opt_i = true;
        }
        else if (input[a] == 'n') {
            myOpts->opt_n = true;
            myOpts->opt_l = true;
        }
        else if (input[a] == 'T') {
            myOpts->opt_T = true;
            myOpts->opt_l = true;
        }
        else if (input[a] == 'g') {
            myOpts->opt_g = true;
            myOpts->opt_l = true;
        }
        else if (input[a] == 'h') {
            myOpts->opt_h = true;
            myOpts->opt_l = true;
        }
        else if (input[a] == 'o') {
            myOpts->opt_o = true;
            myOpts->opt_l = true;
        }
        else {
            printf("Invalid option!\n");
            exit(0);
        }
    }
}
