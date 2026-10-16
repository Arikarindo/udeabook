cmake_minimum_required(VERSION 3.16)

project(UdeABook VERSION 0.1 LANGUAGES CXX)

# Estandar de C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Genera compile_commands.json (util para clangd / otros editores)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# Biblioteca con las clases del modelo (se iran agregando: Album, Usuario, ...)
# ---------------------------------------------------------------------------
add_library(udeabook_core STATIC
    UtilidadesCadena.h
    UtilidadesCadena.cpp
    Foto.h
    Foto.cpp
    Album.h
    Album.cpp
    Mensaje.h
    Mensaje.cpp
    SolicitudAmistad.h
    SolicitudAmistad.cpp
    Usuario.h
    Usuario.cpp
    Programa.h
    Programa.cpp
)
target_include_directories(udeabook_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(udeabook_core PUBLIC /W4)
else()
    target_compile_options(udeabook_core PUBLIC -Wall -Wextra -pedantic)
endif()

# ---------------------------------------------------------------------------
# Ejecutable Principal de la Aplicacion (UdeABook)
# ---------------------------------------------------------------------------
add_executable(UdeABook main.cpp)
target_link_libraries(UdeABook PRIVATE udeabook_core)

# ---------------------------------------------------------------------------
# Ejecutables de pruebas unitarias (descomentar si se desea probar una clase individual)
# ---------------------------------------------------------------------------
# add_executable(test_foto test_foto.cpp)
# target_link_libraries(test_foto PRIVATE udeabook_core)
#
# add_executable(test_album test_album.cpp)
# target_link_libraries(test_album PRIVATE udeabook_core)
#
# add_executable(test_notificacion test_notificacion.cpp)
# target_link_libraries(test_notificacion PRIVATE udeabook_core)
#
# add_executable(test_usuario test_usuario.cpp)
# target_link_libraries(test_usuario PRIVATE udeabook_core)
#
# add_executable(test_programa test_programa.cpp)
# target_link_libraries(test_programa PRIVATE udeabook_core)
