import chessbot
from discord.ext import commands
import os
import discord
import random
import config


chess_engine_depth = 2
file_path = "bot.data"
mention_template = "<@{}>"
description = "A bot that can manage chess games and play in them."

servers = {}


def prefix(bot, message):
    """Returns the prefix of the server a message was sent in."""
    # If no instance exists for this server for data to be stored, initialize
    # one.
    if message.server.id in servers:
        return servers[message.server.id].prefix
    else:
        servers[message.server.id] = Server()
        return servers[message.server.id].prefix

    
bot = commands.Bot(command_prefix=prefix, description=description)


class InvalidMove(Exception):
    """Thrown when an invalid move is made."""
    pass

    
class Game():
    """Represents a game between two players or a player and the bot.
    
    Member variables:
    game - the instance of the game
    white - the user user who is playing as White
    black - the user who is playing as Black
    
    Functions:
    ___init___() - initializes a game between two players and picks White and
    Black randomly
    
    make_best_move() - makes the best move chosen by the chess engine for the
    player to move this turn
    
    get_board_url() - returns a URL of an image showing the chessboard
    """
    def __init__(self, player_id_1, player_id_2):
        """Initializes a game between two players. White and Black are chosen
        randomly.
        """
        # Initialize an instance of the class from the chess library.
        self.game = chessbot.Game()
        
        # Randomly choose who is White and who is Black.
        if random.randint(0, 1) == 1:
            self.white_id = player_id_1
            self.black_id = player_id_2
        else:
            self.white_id = player_id_2
            self.black_id = player_id_1

            
    async def make_best_move(self):
        """Makes the best move for the player to move this turn and returns
        the string representation of that move.
        """
        move = self.game.best_move(chess_engine_depth)
        self.game.make_move(move)
        return self.game.move_to_string(move)
    
    
    def get_board_url(self):
        """Gets the URL to an image of the board in its current state."""
        template = "http://www.fen-to-image.com/image/36/double/coords/{}"
        # Append the FEN of the board to the URL.
        return template.format(self.game.fen())
        
        
    def get_turn(self):
        """Gets the color of the player who is to play this turn."""
        return self.game.get_turn()
        
        
    def get_player_id_to_move(self):
        """Gets the user ID of the player who is to play this turn."""
        if self.get_turn() == chessbot.Color_white:
            return self.white_id
        else:
            return self.black_id
            
            
    def get_game_state(self):
        """Gets the current state of the game."""
        return self.game.game_state()
    
    
    def ended(self):
        """Checks if the game has ended."""
        return self.get_game_state() != chessbot.Game_state_in_progress
    
    
    def end_game_message(self):
        """Returns a message to be outputted when the game has ended."""
        game_state = self.get_game_state()
        
        white_mention = mention(self.white_id)
        black_mention = mention(self.black_id)
        
        winner_mention = None
        loser_mention = None
        
        # Checkmate
        if (game_state == chessbot.Game_state_checkmate_by_white or
                game_state == chessbot.Game_state_checkmate_by_black):
            # Checkmate by White
            if game_state == chessbot.Game_state_checkmate_by_white:
                winner_mention = white_mention
                loser_mention = black_mention
            # Checkmate by Black
            else:
                winner_mention = black_mention
                loser_mention = white_mention
            return winner_mention + " has checkmated " + loser_mention
        # Stalemate
        elif game_state == chessbot.Game_state_stalemate:
            return ("The game between " + white_mention + " and " +
                    black_mention + " has ended due to stalemate.")
        # Threefold repetition
        elif game_state == chessbot.Game_state_threefold_repetition:
            return ("The game between " + white_mention + " and " +
                    black_mention + " has ended due to threefold repetition.")
        # Fifty-move rule
        elif game_state == chessbot.Game_state_fifty_move:
            return ("The game between " + white_mention + " and " +
                    black_mention + " has ended due to the fifty move "
                    "rule.")
        # Insufficient material
        elif game_state == chessbot.Game_state_insufficient_material:
            return ("The game between " + white_mention + " and " +
                    black_mention + " has ended due to insufficient "
                    "material for a checkmate to be possible.")
        
 
    async def make_move(self, move_str):
        """Uses the string representation of a move to make that move. Throws
        InvalidMove if that move is invalid.
        """
        move = self.game.string_to_move(move_str)
        
        invalid_move_str = move == chessbot.Move_none
        pseudo_illegal_move = not self.game.is_pseudo_legal(move)
        
        # Throw an exception if this move is invalid.
        if (invalid_move_str or pseudo_illegal_move or
                not self.game.make_move(move)):
            raise InvalidMove()

            
class Server():
    """Manages the data for a server.
    
    Member variables:
    prefix - the prefix for commands
    games - a dictionary of Game instances. The key for each element is a
    frozen set of the 2 user IDs of the players.
    game_requests - a dictionary used to represent game requests. The key is
    the ID of the person requesting a game while the value is a list of user
    IDs of users whom the user has requested to play with.
    """
    def __init__(self):
        """Initializes a server data instance."""
        self.prefix = ","
        self.games = {}
        self.game_requests = {}


def mention(user_id):
    """Returns a mention of the user ID passed.
    
    user_id - the ID of the user to mention
    """
    # Use the template to generate a string.
    return mention_template.format(user_id)

    
def find_game_key(server_id, user_id):
    """Returns the key of the game a user is in on a specified server.
    
    Returns None if the game was not found.
    
    server_id - the ID of the server in which to look for the game
    user_id - the ID of the user for which to look for a game
    """
    # Check every game for the user.
    for key, game in servers[server_id].games.items():
        if game.white_id == user_id or game.black_id == user_id:
            return key
    return None

    
def is_to_play(game, user_id):
    """Checks if the specified user is to play this turn in the specified
    game.
    
    game - the game to check if the user is in
    user_id - the ID of the user for which to check if playing
    """
    return game.get_player_id_to_move() == user_id

    
@bot.event
async def on_ready():
    """Prints to the console when the bot is ready."""
    print("Successfully logged in.")
    print("Username", bot.user.name)
    print("ID", bot.user.id)
    print("-----------------------")
    
    
@bot.event
async def on_server_join(server):
    """Initializes a server object when joining a new server."""
    servers[server.id] = Server()

    
@bot.command(pass_context=True)
async def start(ctx, target_user: discord.Member = None):
    """Used to start a game with another user.
    
    Errors out if the user requests to play with themselves, is already in a
    game, or is requesting to play with someone who is in a game.
    
    Two users must use this command on each other in order for the game to
    start. This is not required for games against the bot.
    """
    # If no target user is specified, assume this is a request to play with
    # the bot.
    if not target_user:
        target_user = ctx.message.server.get_member(bot.user.id)

    source_user_id = ctx.message.author.id
    target_user_id = target_user.id
    
    server_id = ctx.message.server.id
    server_data = servers[server_id]
    games = server_data.games
    game_requests = server_data.game_requests
    
    source_user_mention = mention(source_user_id)
    target_user_mention = mention(target_user_id)
    
    is_bot_game = target_user_id == bot.user.id
    
    # Make sure the source user is not requesting to play with themselves.
    if source_user_id == target_user_id:
        await bot.say(source_user_mention + ", you can't play with yourself.")
        return
    
    
    # Make sure the source user is not already in a game.
    if find_game_key(server_id, source_user_id) is not None:
        await bot.say(source_user_mention + ", you are already in a game.")
        return
    
    # Make sure the target user is not already in a game.
    if (find_game_key(server_id, target_user_id) is not None and
            not is_bot_game):
        await bot.say(target_user_mention + " is already in a game.")
        return
    
    # If the target user is not the bot, make sure the game request is mutual.
    if not is_bot_game:
        # If the game request is not mutual, store the object in case the
        # target user does decide to make it so. Inform both users of the
        # request.
        if (target_user_id not in game_requests or
            source_user_id not in game_requests[target_user_id]):
            await bot.say(target_user_mention + ", " + source_user_mention +
                          " has requested that you play with them.")
            if source_user_id not in game_requests:
                game_requests[source_user_id] = []
            game_requests[source_user_id].append(target_user_id)
            return
        # If the game request is mutual, delete it and start a new game.
        else:
            game_requests[target_user_id].remove(source_user_id)
    
    # Initialize a game.
    game = Game(source_user_id, target_user_id)
    
    # State that the game has started and who is playing for which side.
    await bot.say("A game has started between " + source_user_mention +
                  " and " + target_user_mention + ".")
    await bot.say(mention(game.white_id) + " is playing as White.")
    await bot.say(mention(game.black_id) + " is playing as Black.")
    
    # If the bot is in the game and has to make a move first, do so.
    if game.white_id == bot.user.id:
        move_str = await game.make_best_move()
        await bot.say(game.get_board_url())
        await bot.say("The bot made move " + move_str + ".")
    # Otherwise, just output an image of the board.
    else:
        await bot.say(game.get_board_url())

    game_key = frozenset((source_user_id, target_user_id))
    
    servers[server_id].games[game_key] = game


@bot.command(pass_context=True)
async def resign(ctx):
    """Used to resign from a game.
    
    Errors out if the user executing this command is not in a game.
    """
    user_id = ctx.message.author.id
    user_mention = mention(user_id)
    server_id = ctx.message.server.id
    games = servers[server_id].games
    
    user_in_game = False
    
    # Look for the game the user is in.
    game_key = find_game_key(server_id, user_id)
    
    # Error out if the user is not in a game.
    if not game_key:
        await bot.say(user_mention + ", you are not currently in a game.")
        return
    
    white_mention = mention(servers[server_id].games[game_key].white_id)
    black_mention = mention(servers[server_id].games[game_key].black_id)
    
    # Announce that the game has ended and send an image of the chessboard.
    
    await bot.say("The game between " + white_mention + " and " +
                  black_mention + " has ended due to resignation by " +
                  user_mention)
    
    await bot.say("Final chessboard position. " +
                  servers[server_id].games[game_key].get_board_url())
    
    # Delete the game instance.
    del servers[server_id].games[game_key]
    
    
@bot.command(pass_context=True)
async def changeprefix(ctx, new_prefix: str):
    """When executed by a server manager, this command changes the server
    prefix of the server.
    """
    # Make sure the user has the Manage Server permission.
    if not ctx.message.author.server_permissions.manage_server:
        await bot.say("You must have the \"Manage Server\" permission to "
                      "change the prefix of this server.")
        return
    # Change the prefix and acknowledge that change.
    servers[ctx.message.server.id].prefix = new_prefix
    await bot.say("The prefix has successfully been changed to " +
                  new_prefix)

                  
@bot.command(pass_context=True)
async def move(ctx, move_str: str):
    """Makes the inputted move in the game the user is in.
    
    An error is outputted if the user is not in a game, the move is
    invalid/illegal, or it is not the user’s turn.
    """
    user_id = ctx.message.author.id
    user_mention = mention(user_id)
    
    server_id = ctx.message.server.id
    
    user_in_game = False
    
    # Look for the game the user is in.
    game_key = find_game_key(server_id, user_id)
    
    # Make sure the user is in a game.
    if not game_key:
        await bot.say("You must be in a game to make a move, " + user_mention)
        return
    
    # Make sure it is the user's turn to play.
    if not is_to_play(servers[server_id].games[game_key], user_id):
        await bot.say("It is not your turn to play, " + user_mention)
        return
    
    # Make the move and make sure it is valid.
    try:
        await servers[server_id].games[game_key].make_move(move_str)
    except InvalidMove:
        await bot.say("That move is invalid, " + user_mention)
        return
    
    opponent_id = servers[server_id].games[game_key] \
                                    .get_player_id_to_move()
    
    opponent_mention = mention(opponent_id)
    
    
    await bot.say(opponent_mention + ", " + user_mention + " has made move " +
                  move_str)
    await bot.say(servers[server_id].games[game_key].get_board_url())

    # Check if the game has ended. Send a message and delete the game instance
    # if that is the case.
    if servers[server_id].games[game_key].ended():
        await bot.say(servers[server_id].games[game_key].end_game_message())
        del servers[server_id].games[game_key]
        return
    
    # If the game has not ended and it is the bot's turn to play, make a move.
    if opponent_id == bot.user.id:
        bot_move_str = await servers[server_id].games[game_key] \
                                               .make_best_move()
        await bot.say(servers[server_id].games[game_key].get_board_url())
        await bot.say(user_mention + ", the bot has made move " +
                      bot_move_str)
        
        # If the game ends, inform the user and delete the game instance.
        if servers[server_id].games[game_key].ended():
            await bot.say(servers[server_id].games[game_key] \
                                            .end_game_message())
            del servers[server_id].games[game_key]


    
def main():
    """Initializes the bot and its data-saving mechanism."""
    # Initialize the magic moves library.
    chessbot.initmagicmoves()
    
    # Run the bot.
    bot.run(config.token)
    

if __name__ == "__main__":
    main()
