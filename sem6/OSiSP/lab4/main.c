#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

int child_running = 0;

void signal_handler(int signal) {
    pid_t child_pid = fork();
    if (child_pid != 0) {
        exit(0);
    }
}

int main() {
    int counter = 0;
    if (signal(SIGINT, signal_handler) == SIG_ERR) {
        perror("Failed to set signal handler");
        return EXIT_FAILURE;
    }

    FILE *file = fopen("output.txt", "a");
    if (file == NULL) {
        perror("Failed to open file");
        return EXIT_FAILURE;
    }

    while (1) {
        fprintf(file, "%d\n", counter++);
        fflush(file);  
        sleep(1);
    }

    return 0;
}