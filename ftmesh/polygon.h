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
 * \brief Definitions of the ftpolygon class.
 *
 * A glyph is defined as a set of \em contours which are actually forming
 * polygons. Here we transform the FreeType vectors and tags in a list of
 * points with a direction.
 *
 * \private
 */


// self
//
#include    <ftmesh/point.h>


// FreeType lib
//
// ft2build.h must come first
#include    <ft2build.h>

#include    FT_FREETYPE_H
//#include    FT_GLYPH_H
//#include    FT_OUTLINE_H


namespace ftmesh
{




class polygon
{
public:
    typedef std::shared_ptr<polygon>        pointer_t;
    typedef std::vector<pointer_t>          vector_t;

                            polygon(
                                      FT_Vector * contour
                                    , char * tags
                                    , unsigned int n);

    std::size_t             size() const;
    point const &           at(int idx) const;
    point const &           leftmost() const;
    void                    apply_parity(int parity);

private:
    void                    add_point(point const & p);
    void                    evaluate_quadratic_curve(
                                  point const & a
                                , point const & b
                                , point const & c);
    void                    evaluate_cubic_curve(
                                  point const & a
                                , point const & b
                                , point const & c
                                , point const & d);

    point::vector_t         f_points = point::vector_t();
    bool                    f_clockwise = false;
    point                   f_leftmost = point(65536.0, 0.0);
};


} // namespace ftmesh
// vim: ts=4 sw=4 et

