# The following variables are defined with the C++ Thread library parameters:
#
# FTMESH_FOUND        - System has CppThread
# FTMESH_INCLUDE_DIRS - The CppThread include directories
# FTMESH_LIBRARIES    - The libraries needed to use CppThread (none)
# FTMESH_DEFINITIONS  - Compiler switches required for using CppThread (none)
#
# License:
#   Copyright (c) 2021  Made to Order Software Corp.  All Rights Reserved
#
#   https://snapwebsites.org/
#   contact@m2osw.com
# 
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License along
#   with this program; if not, write to the Free Software Foundation, Inc.,
#   51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
#

find_path(
    CPPTHREAD_INCLUDE_DIR
        ftmesh/mesh.h

    PATHS
        $ENV{FTMESH_INCLUDE_DIR}
)

find_library(
    CPPTHREAD_LIBRARY
        ftmesh

    PATHS
        $ENV{FTMESH_LIBRARY}
)

mark_as_advanced(
    FTMESH_INCLUDE_DIR
    FTMESH_LIBRARY
)

set(FTMESH_INCLUDE_DIRS ${FTMESH_INCLUDE_DIR})
set(FTMESH_LIBRARIES    ${FTMESH_LIBRARY}    )


# handle the QUIETLY and REQUIRED arguments and set CPPTHREAD_FOUND to TRUE
# if all listed variables are TRUE
#
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    FtMesh
    DEFAULT_MSG
    FTMESH_INCLUDE_DIR
    FTMESH_LIBRARY
)

# vim: ts=4 sw=4 et
