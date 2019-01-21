import chessbot
from discord.ext import commands
import os
import threading
import discord
import pickle
import random
import config
import atexit

chess_engine_depth = 2
file_path = "bot.data"
mention_template = "<@{}>"

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
        return self.game.get_turn()
    
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

def prefix(bot, message):
    """Returns the prefix of the server a message was sent in."""
    if message.server.id in servers:
        return servers[message.server.id].prefix
    else:
        servers[message.server.id] = Server()
        return servers[message.server.id].prefix

description = "A bot that can manage chess games and play in them."
bot = commands.Bot(command_prefix=prefix, description=description)

servers = {}

def mention(user_id):
    return mention_template.format(user_id)

@bot.event
async def on_ready():
    """Prints to the console when the bot is ready."""
    print("Successfully logged in.")
    print("Username", bot.user.name)
    print("ID", bot.user.id)
    print("-----------------------")
    
@bot.event
async def on_server_join(server):
    servers[server.id] = Server()

@bot.command(pass_context=True)
async def start(ctx, target_user : discord.Member = None):
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
    
    # Make sure the user is not requesting to play with themselves.
    if source_user_id == target_user_id:
        await bot.say(source_user_mention + ", you can't play with yourself.")
        return
    
    
    # Make sure the source user is not already in a game.
    if any(game.white_id == source_user_id or game.black_id == source_user_id
           for key, game in games.items()):
        await bot.say(source_user_mention + ", you are already in a game.")
        return
    
    # Make sure the target user is not already in a game.
    if any(game.white_id == target_user_id or game.black_id == target_user_id
           for key, game in games.items()):
        await bot.say(target_user_mention + " is already in a game.")
        return
    
    # If the target user is not the bot, make sure the game request is mutual.
    if target_user_id != bot.user.id:
        # If the game request is not mutual, store the object in case the
        # target user does decide to make it so. Inform both users of the
        # request.
        if (target_user_id not in game_requests or
            source_user_id not in game_requests[target_user_id]):
            await bot.say(target_user_mention + ", " + source_user_mention +
                          "has requested that you play with them.")
            if source_user_id not in game_requests:
                game_requests[source_user_id] = []
            game_requests[source_user_id].append(target_user_id)
            return
        # If the game request is mutual, delete it and start a new game.
        else:
            game_requests[target_user_id].remove(source_user_id)
    
    game = Game(source_user_id, target_user_id)
    
    await bot.say("A game has started between " + source_user_mention +
                  " and " + target_user_mention + ".")
    await bot.say(mention(game.white_id) + " is playing as White.")
    await bot.say(mention(game.black_id) + " is playing as Black.")
    
    # If the bot is in the game and has to make a move first.
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
    user_id = ctx.message.author.id
    user_mention = mention(user_id)
    server_id = ctx.message.server.id
    games = servers[server_id].games
    
    user_in_game = False
    
    # Look for the game the user is in.
    for key, game in games.items():
        if game.white_id == user_id or game.black_id == user_id:
            user_in_game = True
            break
    
    # Error out if the user is not in a game.
    if not user_in_game:
        await bot.say(user_mention + ", you are not currently in a game.")
        return
    
    white_mention = mention(game.white_id)
    black_mention = mention(game.black_id)
    
    await bot.say("The game between " + white_mention + " and " +
                  black_mention + "has ended due to resignation by " +
                  user_mention)
    
    await bot.say("Final chessboard position. " + game.get_board_url())
    
    del servers[server_id].games[key]
    
@bot.command(pass_context=True)
async def changeprefix(ctx, new_prefix : str):
    """When executed by a server manager, this command changes the server
    prefix of the server.
    """
    if not ctx.message.author.server_permission.manage_server:
        await bot.say("You must have the \"Manage Server\" permission to"
                      "change the prefix of this server.")
        return
    servers[ctx.message.server.id].prefix = new_prefix
    await bot.say("The prefix has successfully been changed to " +
                  new_prefix)

@bot.command(pass_Context=True)
async def move(ctx, move_str : str):
    """Makes the inputted move in the game the user is in.
    
    
    """
    pass
    
    
def save_data():
    """Saves the bot data to a file."""
    file = open(file_path, "wb")
    pickle.dump(servers, file, protocol=pickle.HIGHEST_PROTOCOL)
    file.close()
    print("saved " + str(servers))
    
def save_loop():
    """Saves the bot data to a file every 15 seconds."""
    threading.Timer(15.0, save_loop).start()
    save_data()

    
def main():
    """Initializes the bot and its data-saving mechanism."""
    
    # If the bot data file exists, load it, deserialize the data, and use it.
    if os.path.exists(file_path):
        global servers
        file = open(file_path, "rb")
        servers = pickle.load(file)
        file.close()
        print(file_path + " successfully read from.")
    else:
        print(file_path + " not found. Using empty data values.")
    
    save_loop()
    atexit.register(save_data)
    bot.run(config.token)
    


    

if __name__ == "__main__":
    main()
