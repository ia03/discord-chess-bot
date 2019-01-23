%module chessbot
%include "std_string.i"
%include "types.h"
%import "utils.h"

%{
#include "game.h"
#include "lib/magicmoves.h"
%}

%include "game.h"

extern void initmagicmoves(void);
