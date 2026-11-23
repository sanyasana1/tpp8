cmake_minimum_required(VERSION 3.21)
project(pr_tpp8 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(SqliteOrm CONFIG REQUIRED)
find_package(unofficial-sqlite3 CONFIG REQUIRED)

add_executable(pr_tpp8
    src/main.cpp
    src/SQLiteServerRepository.cpp
    src/TestData.cpp
    src/Demos.cpp
    src/TransactionDemos.cpp
    src/RawSqliteDemo.cpp
)

target_include_directories(pr_tpp8 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_link_libraries(pr_tpp8 PRIVATE
    sqlite_orm::sqlite_orm
    unofficial::sqlite3::sqlite3
)

if(MSVC)
    target_compile_options(pr_tpp8 PRIVATE /utf-8)
endif()
