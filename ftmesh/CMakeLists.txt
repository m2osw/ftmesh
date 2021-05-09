# Copyright (c) 2013-2021 Made to Order Software Corp.
#
# https://snapwebsites.org/project/ftmesh
# contact@m2osw.com
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License along
# with this program; if not, write to the Free Software Foundation, Inc.,
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

##
## ftmesh library
##
project(ftmesh)

# Put the version in the header file
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/version.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/version.h
)

include_directories(
    ${LIBEXCEPT_INCLUDE_DIRS}
    ${FREETYPE_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLUT_INCLUDE_DIR}
    ${SNAPLOGGER_INCLUDE_DIRS}
    ${LIBUTF8_INCLUDE_DIRS}
)

add_library(${PROJECT_NAME} SHARED
    font.cpp
    mesh_char.cpp
    mesh.cpp
    mesh_string.cpp
    polygon.cpp
    version.cpp
)

target_link_libraries(${PROJECT_NAME}
    ${LIBEXCEPT_LIBRARIES}
    ${FREETYPE_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLUT_glut_LIBRARY}
    ${SNAPLOGGER_LIBRARIES}
    ${LIBUTF8_LIBRARIES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION
        ${FTMESH_VERSION_MAJOR}.${FTMESH_VERSION_MINOR}

    SOVERSION
        ${FTMESH_VERSION_MAJOR}
)

install(
    TARGETS
        ${PROJECT_NAME}

    RUNTIME DESTINATION
        bin

    LIBRARY DESTINATION
        lib

    ARCHIVE DESTINATION
        lib
)

install(
    FILES
        font.h
        mesh.h
        mesh_char.h
        mesh_string.h
        point.h
        ${CMAKE_CURRENT_BINARY_DIR}/version.h

    DESTINATION
        include/ftmesh
)


# vim: ts=4 sw=4 et
