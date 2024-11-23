
# Awale

## Table of Contents

1. [Project Overview](#project-overview)
2. [Installation](#installation)
3. [Command Reference](#command-reference)
   - [User and Game Interaction](#user-and-game-interaction)
   - [Profile Management](#profile-management)
   - [Friends and Privacy](#friends-and-privacy)
   - [Game Management](#game-management)
   - [Help](#help)

## Project Overview

**Awale** is a classic African strategy game. This project allows players to compete in an exciting and strategic game, manage their profiles, connect with friends, and view past games. Below you’ll find instructions on how to install the game, as well as a full list of commands for interaction.

## Installation

To compile the program for the server-side, first make sure that you're in the project root and execute the command below:

```bash
cd server
make
```

Do the same thing to compile the program for the client-side, except change the directory to `client`:

```bash
cd client
make
```

To execute the program, both executables for the client and the server can be found in the `bin` directory.

For client:

```bash
./bin/client [ip_address] [username]
```

For server:

```bash
./bin/server
```

To clean and recompile the program, simply use the following command in both the `client` and `server` directories:

```bash
make clean
```

## Command Reference

Welcome to the **Awale Game**! Below is a comprehensive list of commands you can use to interact with the game, challenge opponents, manage your profile, and more.

### User and Game Interaction

- **`/list_users`**
  - **Description**: Lists all active users currently online.
  - **Usage**:
    ```bash
    /list_users
    ```

- **`/start_duel <user>`**
  - **Description**: Challenges another user to a duel. Replace `<user>` with the username of the player you want to challenge.
  - **Usage**:
    ```bash
    /start_duel <user>
    ```
  - **Example**:
    ```bash
    /start_duel Player123
    ```

- **`/list_games`**
  - **Description**: Displays all active games you can join or watch.
  - **Usage**:
    ```bash
    /list_games
    ```

- **`/make_move <pit>`**
  - **Description**: Makes a move in your current game by selecting a pit. Replace `<pit>` with the pit number you want to play.
  - **Usage**:
    ```bash
    /make_move <pit>
    ```
  - **Example**:
    ```bash
    /make_move 3
    ```

- **`/watch_player <user>`**
  - **Description**: Watch a specific player's game. Replace `<user>` with the username of the player you want to spectate.
  - **Usage**:
    ```bash
    /watch_player <user>
    ```
  - **Example**:
    ```bash
    /watch_player Player123
    ```

### Profile Management

- **`/change_bio <bio>`**
  - **Description**: Updates your profile biography. Replace `<bio>` with the new biography text you want to set.
  - **Usage**:
    ```bash
    /change_bio <bio>
    ```
  - **Example**:
    ```bash
    /change_bio "Avid Awale player, always up for a challenge!"
    ```

- **`/show_bio <user>`**
  - **Description**: Displays the biography of a specified user. Replace `<user>` with the username you want to check.
  - **Usage**:
    ```bash
    /show_bio <user>
    ```
  - **Example**:
    ```bash
    /show_bio Player123
    ```

### Friends and Privacy

- **`/add_friend <user>`**
  - **Description**: Sends a friend request to another user. Replace `<user>` with the username of the player you want to add as a friend.
  - **Usage**:
    ```bash
    /add_friend <user>
    ```
  - **Example**:
    ```bash
    /add_friend Player123
    ```

- **`/list_friends`**
  - **Description**: Lists all of your current friends.
  - **Usage**:
    ```bash
    /list_friends
    ```

- **`/toggle_private`**
  - **Description**: Toggles your profile between public and private mode. In private mode, only friends can see your profile information.
  - **Usage**:
    ```bash
    /toggle_private
    ```

### Game Management

- **`/leave_game`**
  - **Description**: Leaves your current game. If you are in a game, this command will remove you from it.
  - **Usage**:
    ```bash
    /leave_game
    ```

- **`/game_history`**
  - **Description**: View your past games, including details and outcomes.
  - **Usage**:
    ```bash
    /game_history
    ```

- **`/replay_game <id>`**
  - **Description**: Replays a past game using the game's unique ID. Replace `<id>` with the game ID you want to replay.
  - **Usage**:
    ```bash
    /replay_game <id>
    ```
  - **Example**:
    ```bash
    /replay_game 102
    ```

### Help

- **`/help`**
  - **Description**: Shows a list of all available commands with brief descriptions.
  - **Usage**:
    ```bash
    /help
    ```

---

Feel free to reach out if you have any questions or need more information on any of the commands!
