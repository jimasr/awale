bin/client:bin/client.o bin/client_command_parser.o 
	gcc -o bin/client bin/client.o bin/client_command_parser.o

bin/client.o: client.c
	gcc -c client.c -o bin/client.o

bin/client_command_parser.o:client_command_parser.c
	gcc -c client_command_parser.c -o bin/client_command_parser.o

clean:
	rm bin/*
