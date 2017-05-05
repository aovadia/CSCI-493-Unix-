//Adam Ovadia
//CSCI 493
//Project 2

#include <stdio.h>  /* for printf() */
#include <signal.h> /* for signal() */
#include <string.h> /* for strlen() */
#include <stdlib.h> /* for calloc() */
#include <unistd.h> /* for sleep() */
#include <fcntl.h>  /* for open() */

/*
 Compilation: make sig
 Execution: ./mySig
 --> The critical code processes for 10 secounds and wont allow a ctrl+c to stop execution
*/

void handler(int signo);
void PrintSignal(sigset_t *set);

int fd;

int main(int argc, char **argv) {
    
    fd=open("file.txt", O_CREAT|O_RDWR|O_APPEND, S_IRUSR|S_IWUSR);

    sigset_t *oldset=malloc(sizeof(sigset_t)); //Hold current signals


    printf("Signals in the oldset: \n");
    PrintSignal(oldset);
    printf("\n");

    // Declare signal handlers
    signal(SIGINT, handler);
    signal(SIGHUP, handler);
    signal(SIGEMT, handler);
    signal(SIGUSR1, handler);
    signal(SIGALRM, handler);
    signal(SIGFPE, handler);
    
    sigset_t *newset=malloc(sizeof(sigset_t));
    
    sigemptyset(newset);
    
    // Add 3 signals to newset
    sigaddset(newset, SIGINT);
    sigaddset(newset, SIGHUP);
    sigaddset(newset, SIGALRM);
    printf("Signals in the newset: \n");
    PrintSignal(newset);
    printf("\n");

    sigset_t *waitset=malloc(sizeof(sigset_t));
    sigemptyset(waitset);
    
    sigaddset(waitset, SIGEMT);
    sigaddset(waitset, SIGFPE);
    sigaddset(waitset, SIGUSR1);
    printf("Signals in the waitset: \n");
    PrintSignal(waitset);
    printf("\n");
    
    printf("Blocking signals:\n");
    PrintSignal(newset);
    sigprocmask(SIG_BLOCK, newset, oldset);
    
//~~~~~~~~~~~~~CRITICAL CODE~~~~~~~~~~~~~~~~~~~~~//
    printf("Processing Critical Code for 10 seconds\n");
    char **name=calloc(3, 20*sizeof(char));
    char **it;
    name[0]="Adam Ovadia\n";
    name[1]="CSCI 493\n";
    name[2]="Project 2\n";
    for (it=&name[0]; *it!=NULL; it++)
    {
        ssize_t num_bytes=write(fd, *it, strlen(*it));
    }
    // wait 10 seconds to allow user to enter ctr+c to test
    sleep(10);
    //raise(SIGALRM); // Can also manually program a signal to occur to signal end
    
//~~~~~~~~~~~END OF CRITICAL CODE~~~~~~~~~~~~~~~~//
    
    printf("Critical Code completed execution!\n");
    sigsuspend(waitset);
  
    sigprocmask(SIG_SETMASK, oldset, NULL);
    
    raise(SIGEMT); // Raise an error
    exit(0);
}

void handler(int signo) {
    char *string=(char *)malloc(20*sizeof(char));
    string="ACTION: Received SIGINT.\n";
    char *hup = "ACTION: Received SIGHUP.\n";
    char *usr = "ACTION: Received SIGUSR1.\n";
    char *alarm = "ACTION: Received SIGALRM.\n";
    char *fpe = "ACTION: Received SIGFPE.\n";
    char *emt = "ACTION: Received SIGEMT.\n";

    switch (signo)
    {
        case SIGINT:
            printf("In handler()...\n");
            printf("%s", string);
            ssize_t num_bytes=write(fd, string, strlen(string));
            break;
        case SIGHUP:
            printf("In handler()...\n");
            printf("%s", hup);
            num_bytes=write(fd, hup, strlen(hup));
            break;
        case SIGUSR1:
            printf("In handler()...\n");
            printf("%s", usr);
            num_bytes=write(fd, usr, strlen(usr));
            break;
        case SIGALRM:
            printf("In handler()...\n");
            printf("%s",alarm);
            num_bytes=write(fd, alarm, strlen(alarm));
            break;
        case SIGFPE:
            printf("In handler()...\n");
            printf("%s",fpe);
            num_bytes=write(fd, fpe, strlen(fpe));
            break;
            
        case SIGEMT:
            printf("In handler()...\n");
            printf("%s",emt);
            num_bytes=write(fd, emt, strlen(emt));
            break;
    }
}

void PrintSignal(sigset_t *set) {
    for (int i=1; i<32; i++)
    {
        if (sigismember(set, i))
            printf("Signal %d: %s\n", i, strsignal(i));
    }
}
