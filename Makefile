all: sensor_app

sensor_app: main.o temp_functions.o temp_functions.h
	gcc -o sensor_app main.o temp_functions.o

main.o: main.c 
	gcc -c -o main.o main.c

temp_functions.o: temp_functions.c
	gcc -c -o temp_functions.o temp_functions.c

clean:
	del *.o 
	del sensor_app.exe
