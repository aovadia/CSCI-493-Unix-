// Adam Ovadia

#include <unistd.h>
#include <stdio.h>
#include <pwd.h>
#include <grp.h>

char * getRealOwner(int uid) {
    struct passwd *ptr;
    ptr = getpwuid(uid);
    
    return ptr->pw_name;
}

char * getRealGroup(int gid) {

    struct group *Ptr;
    gid_t GID;
    GID=getgid();
    Ptr = getgrgid((gid_t)gid);
    return Ptr->gr_name;

}
