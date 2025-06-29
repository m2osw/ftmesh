# - Find FtMesh
#
# FTMESH_FOUND        - System has FtMesh
# FTMESH_INCLUDE_DIRS - The FtMesh include directories
# FTMESH_LIBRARIES    - The libraries needed to use FtMesh
# FTMESH_DEFINITIONS  - Compiler switches required for using FtMesh
#
# License:
#
# Copyright (c) 2011-2025  Made to Order Software Corp.  All Rights Reserved
#
# https://snapwebsites.org/project/ftmesh
# contact@m2osw.com
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

find_path(
    FTMESH_INCLUDE_DIR
        ftmesh/mesh.h

    PATHS
        ENV FTMESH_INCLUDE_DIR
)

find_library(
    FTMESH_LIBRARY
        ftmesh

    PATHS
        ${FTMESH_LIBRARY_DIR}
        ENV FTMESH_LIBRARY
)

mark_as_advanced(
    FTMESH_INCLUDE_DIR
    FTMESH_LIBRARY
)

set(FTMESH_INCLUDE_DIRS ${FTMESH_INCLUDE_DIR})
set(FTMESH_LIBRARIES    ${FTMESH_LIBRARY})


include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
    FtMesh
    REQUIRED_VARS
        FTMESH_INCLUDE_DIR
        FTMESH_LIBRARY
)

# vim: ts=4 sw=4 et
