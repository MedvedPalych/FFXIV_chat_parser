﻿#
cmake_minimum_required (VERSION 3.8)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

project ("ffxiv_chat_parser")

# Добавьте источник в исполняемый файл этого проекта.
add_executable (ffxiv_chat_parser "main.cpp" "utils.h" "utils.cpp" "message.h" "pipe.h" "pipe.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ffxiv_chat_parser PROPERTY CXX_STANDARD 20)
endif()

# TODO: Добавьте тесты и целевые объекты, если это необходимо.
