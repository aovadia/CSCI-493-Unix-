// Adam Ovadia

#include "options.h"

ino_t getInode(char *path);
void printSingle(char *file, struct options* myOpts);
void getFileInfo(char *path, struct options* myOpts);

void optLOutPut(char *path, struct options* myOpts) {
    int n = strlen(path);
    DIR *dp = opendir(path);
    if (dp == NULL) {
        printf("Invalid Path! \n");
        return ;
    }
    struct dirent *dp_dirent = readdir(dp);
    while (dp_dirent) {
        bool is_dot = (strcmp(dp_dirent->d_name, ".") == 0  ||
                       strcmp(dp_dirent->d_name, "..") == 0);
      
        if (is_dot || dp_dirent->d_name[0] == '.')
            
        {
            if (myOpts->opt_a) {
                char *new_path = path;
                strcat(new_path, "/");
                strcat(new_path, dp_dirent->d_name);
                if (myOpts->opt_i) {
                    printf("%-2lld ", getInode(new_path));
                }
                
                if (myOpts->opt_l) {
                    getPermissions(new_path); //List permissions
                
                    getFileInfo(new_path, myOpts);
                }
                // List file name
                printf("  %-8s", dp_dirent->d_name);
                
                printf("\n");
                new_path[n]=0;
                
            }
            else if (myOpts->opt_A && !is_dot) {
                if (dp_dirent->d_name[0] == '.') {
                    char *new_path = path;
                    strcat(new_path, "/");
                    strcat(new_path, dp_dirent->d_name);
                    if (myOpts->opt_i) {
                        printf("%-2lld ", getInode(new_path));
                        
                    }
                    if (myOpts->opt_l) {
                        getPermissions(new_path); //List permissions
                    
                        getFileInfo(new_path, myOpts);
                    }
                    // List file name
                    printf("  %-8s", dp_dirent->d_name);
                    printf("\n");
                    new_path[n]=0;
                }

            }
            
        }

        else {
            char *new_path = path;
            strcat(new_path, "/");
            strcat(new_path, dp_dirent->d_name);
            if (myOpts->opt_i) {
                printf("%-2lld ", getInode(new_path));
                
            }
            if (myOpts->opt_l) {
                getPermissions(new_path); //List permissions
            
                getFileInfo(new_path, myOpts);
            }
            // List file name
            printf("  %-8s", dp_dirent->d_name);
            
            printf("\n");
            new_path[n]=0;
        }
        dp_dirent = readdir(dp);
    }
    closedir(dp);
}

void getPermissions(char *path) {
    struct stat buf;
    int myStat = stat(path, &buf);
    if (myStat < 0) {
        printf("Stat error!: Path is: %s \n", path);
    }
    printf( (S_ISDIR(buf.st_mode)) ? "d" : "-");
    printf( (buf.st_mode & S_IRUSR) ? "r" : "-");
    printf( (buf.st_mode & S_IWUSR) ? "w" : "-");
    printf( (buf.st_mode & S_IXUSR) ? "x" : "-");
    printf( (buf.st_mode & S_IRGRP) ? "r" : "-");
    printf( (buf.st_mode & S_IWGRP) ? "w" : "-");
    printf( (buf.st_mode & S_IXGRP) ? "x" : "-");
    printf( (buf.st_mode & S_IROTH) ? "r" : "-");
    printf( (buf.st_mode & S_IWOTH) ? "w" : "-");
    printf( (buf.st_mode & S_IXOTH) ? "x" : "-");
    printf(" ");
}

void getFileInfo(char *path, struct options* myOpts) {
    // List Owner/group and size
    struct stat file;
    int myStat = stat(path, &file);
    printf("%d\t", file.st_nlink);
    if (myOpts->opt_n) {
        printf("%-8d %-8d %-8d ", file.st_uid, file.st_gid, (int)file.st_size);
    }
    else if (myOpts->opt_h) {
        char byte;
        double size = (double)file.st_size;
        if (size < 1024) {
            byte = 'B';
        }
        else if (size < 1048576) {
            byte = 'K';
            size = size / 1024;
        }
        else if (size < 1073741824) {
            byte = 'M';
            size = size / (1024 * 1024);
        }
        else {
            byte = 'G';
            size = size / (1024 * 1024 * 1024);
        }
        printf("%-8s %-8s %-6.2f %-8c ", getRealOwner(file.st_uid), getRealGroup(file.st_gid), size, byte);
        
    }
    else if (myOpts->opt_g) {
        printf("%-8s %-8d ", getRealGroup(file.st_gid), (int)file.st_size);

    }
    else if (myOpts->opt_o) {
        printf("%-8s %-8d ", getRealOwner(file.st_uid), (int)file.st_size);
    }
    else {
        printf("%-8s %-8s %-8d ", getRealOwner(file.st_uid), getRealGroup(file.st_gid), (int)file.st_size);
    }
    // List time of last modification
    struct tm *info;
    char buffer[80];
    info=localtime(&file.st_mtime);
    if (myOpts->opt_T) {
        strftime(buffer, 180, "%b %d %H:%M:%S %Y", info);
    }
    else {
        strftime(buffer, 180, "%b %d %H:%M", info);

    }
    printf("%-8s", buffer);
    
}
void printSingle(char *file, struct options* myOpts) {
    struct stat *buf;
    if (stat(file, buf) < 0) {
       // printf("Stat Error!   %s\n", file);
    }
        getPermissions(file); //List permissions
        
        getFileInfo(file, myOpts);
}
