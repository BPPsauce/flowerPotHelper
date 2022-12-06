//utils.h
#include <stdbool.h> 

void sleep_for_ms(long long delayInMs); //kernal sleep for given ms

int file_read(char* filename); //read file given by filename - returns value

bool file_write(char *filename, char *value);

void runCommand (char* command); // run command in linux

int getAnalogReading(char* analogFile);