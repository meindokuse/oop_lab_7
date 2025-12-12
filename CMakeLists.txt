cmake_minimum_required(VERSION 3.14)
project(DungeonEditor)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

# Основная программа
add_executable(dungeon_editor
    src/main.cpp
    src/NPC.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/Observer.cpp
    src/GameWorld.cpp
    src/Editor.cpp          # ← ДОБАВИТЬ
)

find_package(Threads REQUIRED)
target_link_libraries(dungeon_editor Threads::Threads)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(npc_tests
    tests/test_npc.cpp
    src/NPC.cpp
    src/NPCFactory.cpp
    src/BattleVisitor.cpp
    src/Observer.cpp
    src/GameWorld.cpp
    src/Editor.cpp          # ← ТОЖЕ ДОБАВИТЬ
)

target_link_libraries(npc_tests gtest gtest_main Threads::Threads)

include(GoogleTest)
gtest_discover_tests(npc_tests)
