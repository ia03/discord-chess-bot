#ifndef COMMON_H
#define COMMON_H

#include <cstdint>

typedef uint64_t U64;

static const int kSquares = 64;

static const char kRookMagics[] = "lib/rook_magics.magic";
static const char kRookMasks[] = "lib/rook_masks.magic";
static const char kRookShifts[] = "lib/rook_shifts.magic";
static const char kRookOffsets[] = "lib/rook_offsets.magic";
static const char kRookAttackTable[] = "lib/rook_attack_table.magic";

static const char kBishopMagics[] = "lib/bishop_magics.magic";
static const char kBishopMasks[] = "lib/bishop_masks.magic";
static const char kBishopShifts[] = "lib/bishop_shifts.magic";
static const char kBishopOffsets[] = "lib/bishop_offsets.magic";
static const char kBishopAttackTable[] = "lib/bishop_attack_table.magic";

#endif