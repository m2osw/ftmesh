// Copyright (c) 2021  Made to Order Software Corp.  All Rights Reserved
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
 * \brief The main class to load fonts and build meshes.
 *
 * This file implements the ftmesh class which handles the loading of
 * fonts and builds meshes for specified glyphs.
 */

// self
//
#include    "ftmesh/mesh.h"


// OpenGL lib
//
#include    <GL/gl.h>


// last include
//
#include    <snapdev/poison.h>



// we assume that GLdouble is "double" and we want to make sure of that
// (if not, we would need to offer an fttype.h file with such types)
//
static_assert(std::is_same<double, GLdouble>::value, "The library expects GLdouble to be a double");



namespace ftmesh
{



mesh::mesh(int advance)
    : f_advance(advance)
{
}


void mesh::begin()
{
    f_indexes.push_back(f_points.size());
    //f_types.push_back(type);
}


void mesh::add_point(point const & point)
{
    f_points.push_back(point);
}


void mesh::end()
{
}


point::vector_t const & mesh::get_points() const
{
    return f_points;
}


mesh::index_vector_t const & mesh::get_indexes() const
{
    return f_indexes;
}


//mesh::type_vector_t const & mesh::get_types() const
//{
//    return f_types;
//}


int mesh::get_advance() const
{
    return f_advance;
}


} // namespace ftmesh
// vim: ts=4 sw=4 et
