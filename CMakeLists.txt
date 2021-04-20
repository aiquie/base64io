cmake_minimum_required(VERSION 2.6)
project(base64io)

add_executable(base64io base64io.c base64.h base64.c unifile.h unifile.c convbuf.h convbuf.c)

add_custom_command(TARGET base64io POST_BUILD COMMAND "${PROJECT_SOURCE_DIR}/test.sh")
