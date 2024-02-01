#pragma once

#include <array>
#include <cstddef>

const int lilyGoT4FullScreenBufferSize = 600*450;        

// MBJ REFACTOR - how to get rid of lilyGoT4FullScreenBufferSize here? c.f. C array []

extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_N;
extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_W;
extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_SW;
extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_S;
extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_SE;
extern const std::array<uint16_t, lilyGoT4FullScreenBufferSize> lily_wraysbury_All;

const int M5FullScreenBufferSize = 135*240;

// MBJ REFACTOR - how to get rid of M5FullScreenBufferSize here? c.f. C array []

extern const std::array<uint16_t, M5FullScreenBufferSize> w1_1;
extern const std::array<uint16_t, M5FullScreenBufferSize> w1_2;
extern const std::array<uint16_t, M5FullScreenBufferSize> w1_3;
extern const std::array<uint16_t, M5FullScreenBufferSize> w1_4;
extern const std::array<uint16_t, M5FullScreenBufferSize> wraysbury_x1;

