#pragma once
#include "frames/frames.h"


struct Header {
    std::string identifier;
    uint8_t version{};
    uint8_t revision{};
    uint8_t flags{}; //abcd0000
    uint32_t size{};
};

void fun(std::string& path);