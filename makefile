all: soilDriver

soilDriver: 
	arm-linux-gnueabihf-gcc -Wall -g -pthread -std=c99 -D _POSIX_C_SOURCE=200809L soilSensor.c main.c -o soilDriver
	cp soilDriver ~/cmpt433/public/myApps/
	
clean: 
	rm -f $(OBJ)