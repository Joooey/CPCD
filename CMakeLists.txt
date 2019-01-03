cmake_minimum_required(VERSION 3.12)
project(CompilationPrincipleCourseDesgin)

set(CMAKE_CXX_STANDARD 14)

add_executable(CompilationPrincipleCourseDesgin main.cpp Enums.h Structs.h defines.h GlobalVar.h Interpret.h TargetUtils.h Algorithm.h GetUtils.h Block.h SysUtils.h)