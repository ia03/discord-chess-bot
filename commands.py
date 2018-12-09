# Start a new game.
# Error if source or target user is already in a game.
# If the request to play with the other player is not mutual, acknowledge it.
# If the target user has already requested to play with the source user, start a game and invalidate the request. White
# and Black are chosen randomly.
# If the source user requests to play with no one or the bot, start a game using the chess engine. White and Black are
# chosen randomly.
# Output the names and the board. Mention both players.
# If it is the bot's turn, use the AI to make the next move.

# End a game.
# Error if the user is not already in a game.
# End the game.
# Output the board.
# Mention both users, telling them the game has ended.

# Change the prefix.
# If the user is not a server manager, error.
# Change the prefix.

# Make a move.
# Error if user is not already in a game.
# Error if it is not the user's turn.
# Error if the move is invalid.
# Run the move.
# If a checkmate or stalemate has occurred, end the game and mention both users, telling them the game has ended. Then
# output the board.
# If it is now the bot's turn, use the AI to make the next move.
# If backup file saved to more than 5 seconds ago, save again.
