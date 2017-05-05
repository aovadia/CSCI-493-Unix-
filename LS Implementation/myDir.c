// Adam Ovadia
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <dirent.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "mk_space.h" //used to allocate space for new path name

#include "options.h"

/* function type that is called for each filename */
typedef int Myfunc(const char *, const struct stat *, int);
void Rec_Path (char *path, struct options* myOpts);

static Myfunc   myfunc;
static int      myftw(char *, Myfunc *, struct options* myOpts);
static int      dopath(Myfunc *, struct options* myOpts);

static long nreg, ndir, nblk, nchr, nfifo, nslink, nsock, ntot;

void Rec_Path (char *path, struct options* myOpts) {
      struct stat buf;
    
    int ret;
  
    if (lstat(path, &buf) < 0) {
        printf("Stat error! \n");
    }
    else if (!S_ISDIR(buf.st_mode)) {
          printf("Your entry: %s is not a valid directory! \n", path);
    }
    else {
        ret = myftw(path, myfunc, myOpts);       /* does it all */
        exit(ret);
    }
}

/*
 * Descend through the hierarchy, starting at "pathname".
 * The caller’s func() is called for every file.
 */
#define FTW_F   1       /* file other than directory */
#define FTW_D   2       /* directory */

#define FTW_DNR 3   /* directory that can’t be read */
#define FTW_NS  4   /* file that we can’t stat */

static char *fullpath;      /* contains full pathname for every file */
static size_t pathlen;

static int myftw(char *pathname, Myfunc *func, struct options* myOpts) {
     fullpath = path_alloc(&pathlen);
    if (pathlen <= strlen(pathname)) {
        pathlen = strlen(pathname) * 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            printf("realloc failed\n");
    }
    strcpy(fullpath, pathname);
    return(dopath(func, myOpts));
}

/*
 * Descend through the hierarchy, starting at "fullpath".
 * If "fullpath" is anything other than a directory, we lstat() it,
 * call func(), and return.  For a directory, we call ourself
 * recursively for each name in the directory.
 */

static int dopath(Myfunc* func, struct options* myOpts) {
    struct stat statbuf;
    struct dirent *dirp;
    DIR *dp;
    int ret, n;
    
    if (lstat(fullpath, &statbuf) < 0)  /* stat error */
        return(func(fullpath, &statbuf, FTW_NS));
    if (S_ISDIR(statbuf.st_mode) == 0)  /* not a directory */
        return(func(fullpath, &statbuf, FTW_F));
    
    /*
     * It’s a directory.  First call func() for the directory,
     * then process each filename in the directory.
     */
    if ((ret = func(fullpath, &statbuf, FTW_D)) != 0)
        return(ret);
    printf("Path: %s\n", fullpath);

    n = strlen(fullpath);
    if (n + NAME_MAX + 2 > pathlen) {   /* expand path buffer */
        pathlen *= 2;
        if ((fullpath = realloc(fullpath, pathlen)) == NULL)
            printf("realloc failed\n");
    }
    fullpath[n++] = '/';
    fullpath[n] = 0;
    
    if ((dp = opendir(fullpath)) == NULL)   /* can’t read directory */
        return(func(fullpath, &statbuf, FTW_DNR));

    while ((dirp = readdir(dp)) != NULL) {
        bool is_dot = (strcmp(dirp->d_name, ".") == 0  ||
                       strcmp(dirp->d_name, "..") == 0);
        if ( is_dot || dirp->d_name[0] == '.') {
                if (myOpts->opt_a) {
                    if (myOpts->opt_i) {
                        printf("%lld ", getInode(fullpath));

                    }
                    if (myOpts->opt_l) {
                        if (myOpts->opt_i) {
                            printf("%lld ", getInode(fullpath));
                        }
                        strcpy(&fullpath[n], dirp->d_name);
                        printSingle(fullpath, myOpts);
                        printf(" %s \n", dirp->d_name);
                        
                    }
                    else {
                        printf("%s\n",dirp->d_name);
                    }
                }
                else if (myOpts->opt_A && !is_dot) {
                    if (myOpts->opt_i) {
                        printf("%lld ", getInode(fullpath));

                    }
                    if (myOpts->opt_l) {
                        if (myOpts->opt_i) {
                            printf("%lld ", getInode(fullpath));
                        }
                        strcpy(&fullpath[n], dirp->d_name);
                        printSingle(fullpath, myOpts);
                        printf(" %s \n", dirp->d_name);
                        
                    }
                    else {
                        printf("%s\n",dirp->d_name);
                    }

                }
            continue;       /* ignore dot and dot-dot */
        }
        
        if (myOpts->opt_l) {
            if (myOpts->opt_i) {
                printf("%lld ", getInode(fullpath));
            }
            strcpy(&fullpath[n], dirp->d_name);
            printSingle(fullpath, myOpts);
            printf(" %s \n", dirp->d_name);
            
        }
        else {
            if (myOpts->opt_i) {
                printf("%lld ", getInode(fullpath));
            }
            printf(" %s \n", dirp->d_name);
        }

        strcpy(&fullpath[n], dirp->d_name); /* append name after "/" */
        if ((ret = dopath(func,myOpts)) != 0)      /* recursive */
            break;  /* time to leave */
    }
    printf("\n");
    fullpath[n-1] = 0; /* erase everything from slash onward */
    if (closedir(dp) < 0)
        printf("can’t close directory %s \n", fullpath);
    return(ret);

}


static int myfunc(const char *pathname, const struct stat *statptr, int type) {
    switch (type) {
        case FTW_F:
            // Count each type
            switch (statptr->st_mode & S_IFMT) {
                case S_IFREG:
                    nreg++;
                    break;
                case S_IFBLK:
                    nblk++;
                    break;
                case S_IFCHR:
                    nchr++;
                    break;
                case S_IFIFO:
                    nfifo++;
                    break;
                case S_IFLNK:
                    nslink++;
                    break;
                case S_IFSOCK:
                    nsock++;
                    break;
                case S_IFDIR:
                    break;
            }
            break;
        case FTW_D:
            ndir++;
            break;
        case FTW_DNR:
            break;
        case FTW_NS:
            break;
        default:
            break;
    }
    return(0);
}
