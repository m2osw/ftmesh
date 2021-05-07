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

// self
//
#include    "main.h"


// ftmesh lib
//
#include    <ftmesh/point.h>


// snapdev lib
//
#include    <snapdev/not_reached.h>


// C lib
//
#include    <unistd.h>


// we're testing many of those here so ignore warnings
//
#pragma GCC diagnostic ignored "-Wfloat-equal"


CATCH_TEST_CASE("point", "[point]")
{
    CATCH_START_SECTION("Verify constructors and copy")
    {
        ftmesh::point a;

        CATCH_REQUIRE(a.f_coordinates[0] == 0.0);
        CATCH_REQUIRE(a.f_coordinates[1] == 0.0);
        CATCH_REQUIRE(a.f_coordinates[2] == 0.0);

        ftmesh::point b(-33.5, +55.0);

        CATCH_REQUIRE(b.f_coordinates[0] == -33.5);
        CATCH_REQUIRE(b.f_coordinates[1] == 55.0);
        CATCH_REQUIRE(b.f_coordinates[2] == 0.0);

        a = b;

        CATCH_REQUIRE(a.f_coordinates[0] == -33.5);
        CATCH_REQUIRE(a.f_coordinates[1] == 55.0);
        CATCH_REQUIRE(a.f_coordinates[2] == 0.0);
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et

