#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void sigusr1_handler(int signum) {
    printf("Received SIGUSR1 signal in process with PID %d\n", getpid());
}

int main() {
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        // This is the first child process
        printf("I am the first child process with PID %d\n", getpid());
        sleep(1);
        kill(getpid() + 1, SIGUSR1);         // getpid() + 1 is used to obtain the PID of the second child process
        _exit(0);
        
    } else if (pid1 > 0) {
        // This is the parent process
        printf("I am the parent process with PID %d\n", getpid());
        pid2 = fork();
        if (pid2 == 0) {
            // This is the second child process
            printf("I am the second child process with PID %d\n", getpid());
            signal(SIGUSR1, sigusr1_handler);  //
            pause();
            printf("Exiting second child process with PID %d\n", getpid());
        } else if (pid2 > 0) {
            // This is the parent process
            printf("I am the parent process with PID %d, waiting for child processes to complete...\n", getpid());
            waitpid(pid1, NULL, 0);
            waitpid(pid2, NULL, 0);
            printf("All child processes have completed,\n exiting.\n");
        } else {
            // Failed to fork second child
            printf("Failed to fork second child process\n");
        }
    } else {
        // Failed to fork first child
        printf("Failed to fork first child process\n");
    }

    return 0;
}
