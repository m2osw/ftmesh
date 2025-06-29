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
#include    <ftmesh/mesh_string.h>


namespace ftmesh
{


constexpr int const DEFAULT_UPSCALE = 64;
constexpr int const DEFAULT_SIZE = 12;
constexpr int const DEFAULT_RESOLUTION = 72;


namespace detail
{
class font_impl;
} // namespace details


class font
{
public:
    typedef std::shared_ptr<font>         pointer_t;

                            font(std::string const & filename);

    void                    set_precision(int precision);
    void                    set_size(int point, int x_resolution, int y_resolution);

    mesh::pointer_t         get_mesh(char32_t glyph);
    mesh_string::pointer_t  convert_string(std::string const & message);
    float                   string_width(std::string const & message);

private:
    mesh::map_t             f_map = mesh::map_t();
    std::shared_ptr<detail::font_impl>
                            f_impl = std::shared_ptr<detail::font_impl>();
};



} // namespace ftmesh
// vim: ts=4 sw=4 et
