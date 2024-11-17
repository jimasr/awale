bin/server:bin/server.o bin/server_client_manager.o bin/server_command_handler.o bin/server_command_router.o bin/game_logic.o
	gcc -o bin/server bin/server.o bin/server_client_manager.o bin/server_command_handler.o bin/server_command_router.o bin/game_logic.o

bin/server.o:server.c
	gcc -c server.c -o bin/server.o

bin/server_client_manager.o:server_client_manager.c
	gcc -c server_client_manager.c -o bin/server_client_manager.o

bin/server_command_handler.o:server_command_handler.c
	gcc -c server_command_handler.c -o bin/server_command_handler.o

bin/server_command_router.o:server_command_router.c
	gcc -c server_command_router.c -o bin/server_command_router.o

bin/game_logic.o:game_logic.c
	gcc -c game_logic.c -o bin/game_logic.o

clean:
	rm bin/*
