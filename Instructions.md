# TP_AWALE
**Basic client/server application for the Awale game**

## Compile & Run:
- To compile the server, go to the server directory and run make to compile the source code. To run the server, execute the prog.out file in the bin/ directory: **bin/prog.out**
- To compile the client, go to the client directory and run make to compile the source code. To run the client, execute the prog.out file in the bin/ directory: **bin/prog.out <server_ipv4_address> <username>**
ex : ./prog.out 127.0.0.1 Saad

## User Commands:
/list_users          - List all active users
/start_duel <user>   - Challenge a user to a duel
/list_games          - List all active games
/make_move <pit>     - Make a move in your game
/watch_player <user> - Watch a player's game
/change_bio <bio>    - Update your biography
/show_bio <user>     - Show the biography of a user
/add_friend <user>   - Send a friend request
/list_friends        - List your friends
/toggle_private      - Toggle private mode
/leave_game          - Leave your current game
/game_history        - View your game history
/replay_game <id>    - Replay a game by ID
/help                - Show this help message
