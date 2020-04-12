# discord-chess-bot
A Discord chess bot that supports games between players and games between a player and the bot. It implements the minimax algorithm with alpha-beta pruning.

To run this, you need to use SWIG and a C++17 compiler to generate a wrapper for the C++ code in the root directory of this repository. To do this, adjust the python include directory in build.sh and run it.

Then you install Discord.py, set the *token* variable in a new file "config.py" to the token of the bot, and run main.py.

This supports en passant, castling, draws, and more.
