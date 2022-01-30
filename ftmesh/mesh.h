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
 * \brief Definitions of the ftmesh class.
 *
 * This header defines the ftmesh class which represents one glyphs.
 *
 * When a glyph is loaded, it gets transformed in a list of vertices which
 * represent triangles that can be used to render the character.
 */

// self
//
#include    "point.h"


// C++ lib
//
#include    <deque>
#include    <map>


namespace ftmesh
{


class mesh
{
public:
    typedef std::shared_ptr<mesh>           pointer_t;
    typedef std::deque<pointer_t>           deque_t;
    typedef std::map<char32_t, pointer_t>   map_t;
    typedef std::vector<int>                index_vector_t;
    //typedef std::vector<GLenum>             type_vector_t;

                                mesh(float advance);

    void                        begin();
    void                        add_point(point const & point);
    void                        end();

    point::vector_t const &     get_points() const;
    index_vector_t const &      get_indexes() const;
    //type_vector_t const &       get_types() const;
    float                       get_advance() const;

private:
    point::vector_t             f_points = point::vector_t();
    index_vector_t              f_indexes = index_vector_t();
    //type_vector_t               f_type = type_vector_t();
    float                       f_advance = 0;
};



} // namespace ftmesh
// vim: ts=4 sw=4 et
