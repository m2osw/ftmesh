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
 * \brief Definitions of the ftmesh_string class.
 *
 * When creating a string, you need to handle the advancements between
 * characters which varies depending on the current and next characters
 * (when the font has kerning data). Also, various characters have various
 * width in a fully dynamic font.
 *
 * Therefore, just an array of meshes would not be enough. The ftmesh_string
 * is a queue which adds the advance amount between each character to the
 * meshes of each character.
 */

// self
//
#include    <ftmesh/mesh_char.h>



namespace ftmesh
{


class mesh_string
    : public mesh_char::vector_t
{
public:
    typedef std::shared_ptr<mesh_string>  pointer_t;

    void                    add_glyph(mesh::pointer_t mesh, float advance);

private:
};


} // namespace ftmesh
// vim: ts=4 sw=4 et
