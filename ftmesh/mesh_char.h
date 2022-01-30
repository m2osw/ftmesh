// Copyright (c) 2021-2022  Made to Order Software Corp.  All Rights Reserved
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
#pragma once

/** \file
 * \brief Definitions of the ftmesh_char class.
 *
 * When creating a string, you need to handle the advancements between
 * characters. A character is a mesh and an advancement. This class holds
 * on both of these parameters.
 */

// self
//
#include    <ftmesh/mesh.h>


namespace ftmesh
{


class mesh_char
{
public:
    typedef std::shared_ptr<mesh_char>      pointer_t;
    typedef std::vector<pointer_t>          vector_t;

                            mesh_char(mesh::pointer_t mesh, float advance);

    mesh::pointer_t         get_mesh() const;
    float                   get_advance() const;

private:
    mesh::pointer_t         f_mesh = mesh::pointer_t();
    int                     f_advance = 0;
};


} // namespace ftmesh
// vim: ts=4 sw=4 et
