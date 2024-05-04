cmake_minimum_required(VERSION 3.23)
project(mp3parser)

set(CMAKE_CXX_STANDARD 14)

add_executable(mp3parser main.cpp lib/parser/parser.cpp lib/parser/parser.h lib/parser/frames/frames.cpp lib/parser/frames/frames.h)
