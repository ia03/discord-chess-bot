# discord-chess-bot
A Discord chess bot.

To run this, you need to compile the magic-bits library using the makefile. You must also use SWIG and a compiler to generate a wrapper for the C++ code in the root directory of this repository.

Then you install Discord.py, set the *token* variable in a new file "config.py" to the token of the bot, and run main.py