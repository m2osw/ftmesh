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
 * \brief Definitions of the ftpoint class.
 *
 * Define a (x, y, z) point with some operation.
 *
 * The type is defined such that it can be used with the glut directly.
 * i.e. we have an array of three doubles named f_coordinates.
 *
 * \note
 * We also verify that the double type is what GLdouble is but here we
 * do not want to leak any OpenGL header file.
 */


// C++ lib
//
#include    <cmath>
#include    <cstring>
#include    <memory>
#include    <vector>



namespace ftmesh
{



struct point
{
    typedef std::shared_ptr<point>      pointer_t;
    typedef std::vector<point>          vector_t;
    typedef std::vector<pointer_t>      safe_vector_t;  // "safe" as in the points do not move in memory

    point()
    {
    }

    point(double x, double y)
    {
        f_coordinates[0] = x;
        f_coordinates[1] = y;
    }

    point(double x, double y, double z)
    {
        f_coordinates[0] = x;
        f_coordinates[1] = y;
        f_coordinates[2] = z;
    }

    point(point const & rhs) = default;

    point & operator = (point const & rhs) = default;

    bool operator != (point const & rhs) const
    {
        return std::memcmp(f_coordinates, rhs.f_coordinates, sizeof(f_coordinates)) != 0;
    }

    point operator + (point const & rhs) const
    {
        return point(x() + rhs.x(), y() + rhs.y(), z() + rhs.z());
    }

    point operator - (point const & rhs) const
    {
        return point(x() - rhs.x(), y() - rhs.y(), z() - rhs.z());
    }

    point operator * (double scale) const
    {
        return point(x() * scale, y() * scale, z() * scale);
    }

    double angle() const
    {
        return atan2(y(), x());
    }

    bool is_left_of(point const & rhs) const
    {
        return f_coordinates[0] < rhs.f_coordinates[0];
    }

    double x() const
    {
        return f_coordinates[0];
    }

    double y() const
    {
        return f_coordinates[1];
    }

    double z() const
    {
        return f_coordinates[2];
    }

    // WARNING: we use an array of GLdouble so we are 100% compatible with
    //          the GLU tesselation which requires those are vertices; we
    //          verify that GLdouble is double (see )
    //
    double      f_coordinates[3] = {};
};



} // namespace ftmesh
// vim: ts=4 sw=4 et

