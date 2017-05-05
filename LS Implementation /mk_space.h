// Adam Ovadia

#ifndef mk_space_h
#define mk_space_h

void err_sys(const char *fmt, ...);

char *path_alloc(size_t *sizep); /* also return allocated size, if nonnull */

#endif /* mk_space_h */
#include "mk_space.c"
