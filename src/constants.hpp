#pragma once

#include <cstdint>

inline constexpr int LEVEL_SIZE_X = 80;
inline constexpr int LEVEL_SIZE_Y = 30;

inline constexpr char PLAYER_HEAD_SPRITE_X = 'c';
inline constexpr char PLAYER_HEAD_SPRITE_Y = 'U';
inline constexpr char PLAYER_BODY_SPRITE = 'o';
inline constexpr char APPLE_SPRITE = '6';

inline constexpr uint32_t LAYER_DEFUALT = 0;
inline constexpr uint32_t LAYER_PLAYER = 1;
inline constexpr uint32_t LAYER_APPLE = 2;