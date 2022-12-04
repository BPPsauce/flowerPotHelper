//util.c : Common functions that are used by various modules
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h> // for struct timeval
#include <time.h> // for nanosleep
#include "utils.h"
#include <unistd.h>

void sleep_for_ms(long long delayInMs) // given by instructor
{
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;

    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;

    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

int getAnalogReading(char* analogFile){
    // Open file
    FILE *f = fopen(analogFile, "r");
    if (!f) {
        printf("ERROR: Unable to open voltage input file. Cape loaded?\n");
        printf(" Check /boot/uEnv.txt for correct options.\n");
        exit(-1);
    }
    // Get reading
    int a2dReading = 0;
    int itemsRead = fscanf(f, "%d", &a2dReading);
    if (itemsRead <= 0) {
        printf("ERROR: Unable to read values from voltage input file.\n");
        exit(-1);
    }
    // Close file
    fclose(f);
    return a2dReading;
}

void runCommand (char* command){
    // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
    // Ignore output of the command; but consume it
    // so we don't get an error when closing the pipe.
    char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {

        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
    // printf("--> %s", buffer); // Uncomment for debugging
        }
    // Get the exit code from the pipe; non-zero is an error:
        int exitCode = WEXITSTATUS(pclose(pipe));

        if (exitCode != 0) {
        perror("Unable to execute command:");
        printf(" command: %s\n", command);
        printf(" exit code: %d\n", exitCode);
        }
}