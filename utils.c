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

int file_read(char* filename)
{
    FILE *file = fopen(filename, "r");
    if(file == NULL)
    {
        printf("Error opening AIN1 file\n");
        exit(1);
    }
    char buff[1024];
    if(!fgets((buff), sizeof(buff), file))
    {
        printf("Error reading from AIN1 file\n");
        exit(1);
    }
    fclose(file);
    return atoi(buff);
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