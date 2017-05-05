// Adam Ovadia

#ifndef options_h
#define options_h
#include <stdio.h>

struct options {
    bool opt_a;
    bool opt_l;
    bool opt_R;
    bool opt_A;
    bool opt_i;
    bool opt_n;
    bool opt_T;
    bool opt_g;
    bool opt_h;
    bool opt_o;
    char *FULL_PATH;
    int count;
    
};

// Initialize options to false
struct options usr_opts  = (struct options) {
    .count = 0, .opt_a =  false , .opt_l =  false, .opt_R = false ,
    .opt_A = false, .opt_i = false, .opt_n = false, .opt_T = false,
    .opt_g = false, .opt_h = false, .opt_o = false
};
#endif /* options_h */
