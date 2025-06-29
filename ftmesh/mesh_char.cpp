// Copyright (c) 2021-2025  Made to Order Software Corp.  All Rights Reserved
//
// https://snapwebsites.org/project/ftmesh
// contact@m2osw.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License along
// with this program; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

/** \file
 * \brief Implementation of the mesh_char class.
 *
 * When creating a string, you need to handle the advancement between
 * characters which varies depending on the current and next characters
 * (when the font has kerning data). Also, various characters have various
 * width in a fully dynamic font (i.e. variable size).
 *
 * Therefore, just an array of meshes would not be enough. The mesh_string
 * is a queue which adds the advance amount between each character to the
 * meshes of each character.
 */

// self
//
#include    <ftmesh/mesh_char.h>


// last include
//
#include    <snapdev/poison.h>




namespace ftmesh
{


mesh_char::mesh_char(mesh::pointer_t mesh, float advance)
    : f_mesh(mesh)
    , f_advance(advance)
{
}


mesh::pointer_t mesh_char::get_mesh() const
{
    return f_mesh;
}


float mesh_char::get_advance() const
{
    return f_advance;
}


} // namespace ftmesh
// vim: ts=4 sw=4 et
