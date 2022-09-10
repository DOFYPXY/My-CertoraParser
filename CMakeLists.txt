# minimum required CMAKE version
CMAKE_MINIMUM_REQUIRED(VERSION 3.7 FATAL_ERROR)
project(CertoraParser)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# compiler must be 17
set(CMAKE_CXX_STANDARD 17)

# required if linking to static library
add_definitions(-DANTLR4CPP_STATIC)

# using /MD flag for antlr4_runtime (for Visual C++ compilers only)
set(ANTLR4_WITH_STATIC_CRT OFF)

# Specify the version of the antlr4 library needed for this project.
# By default the latest version of antlr4 will be used.  You can specify a
# specific, stable version by setting a repository tag value or a link
# to a zip file containing the libary source.
# set(ANTLR4_TAG 4.10.1)
# set(ANTLR4_ZIP_REPOSITORY https://github.com/antlr/antlr4/archive/refs/tags/4.10.1.zip)

# add external build for antlrcpp
include(ExternalAntlr4Cpp)
# add antrl4cpp artifacts to project environment
include_directories(${ANTLR4_INCLUDE_DIRS})

# set variable pointing to the antlr tool that supports C++
# this is not required if the jar file can be found under PATH environment
if(DEFINED ENV{ANTLR_EXECUTABLE})
    set(ANTLR_EXECUTABLE $ENV{ANTLR_EXECUTABLE})
else()
    set(ANTLR_EXECUTABLE /usr/local/lib/antlr-4.11.1-complete.jar)
endif()
# add macros to generate ANTLR Cpp code from grammar
find_package(ANTLR REQUIRED)

# from https://github.com/antlr/antlr4/issues/3418
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

# Call macro to add lexer and grammar to your build dependencies.
antlr_target(CertoraGrammarParser Certora.g4 VISITOR
             # DEPENDS_ANTLR CertoraGrammarLexer
             OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/libs/)

include_directories(
                    ${PROJECT_SOURCE_DIR}
                    ${PROJECT_SOURCE_DIR}/libs/
   )

# add generated grammar to parser binary target
add_executable(parser
               ${ANTLR_CertoraGrammarParser_CXX_OUTPUTS} 
               main.cpp
               V.cpp
               Visitor.cpp
               AST.cpp )
target_link_libraries(parser antlr4_static)

# from https://github.com/antlr/antlr4/issues/3418
target_link_libraries(parser Threads::Threads)