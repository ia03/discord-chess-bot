%module chessbot
%include "std_vector.i"
%include "std_array.i"
%include "std_string.i"
%include "types.h"
%import "utils.h"

%{
#include "game.h"
#include "lib/magicmoves.h"
%}

namespace std {
    %template(MoveVector) vector<Move>;
};

%include "game.h"

extern void initmagicmoves(void);
