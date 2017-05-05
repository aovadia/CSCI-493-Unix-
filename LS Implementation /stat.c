// Adam Ovadia

ino_t getInode(char *path) {
    struct stat file;
    int myStat = stat(path, &file);
    return file.st_ino;
}
