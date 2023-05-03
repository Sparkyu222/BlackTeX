#add_compile_options(-Wall -Wextra -pedantic -Werror)

# Nous voulons un cmake "récent" pour utiliser les dernières fonctionnalités

cmake_minimum_required(VERSION 3.18.4)

# Nom du projet
project(BlackTeX)

# Dossier qui contient les builds
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/lib)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})

# Inclure pgnp
add_subdirectory(pgnp)

# Création de l'exécutable
add_executable(blacktex main.cpp)

# Liens de la cible avec les bibliothèques externes
target_include_directories(blacktex PRIVATE pgnp/src pgnp/libs/chess-move-interface/src)
target_link_libraries(blacktex PRIVATE pgnp)