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
#include    <ftmesh/font.h>


// snapdev lib
//
#include    <snapdev/not_reached.h>


// C lib
//
#include    <unistd.h>


// we're testing many of those here so ignore warnings
//
#pragma GCC diagnostic ignored "-Wfloat-equal"


CATCH_TEST_CASE("font", "[font]")
{
    CATCH_START_SECTION("Make sure font works")
    {
        ftmesh::font f("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf");
        f.set_size(78, 72, 72);

        ftmesh::mesh_string::pointer_t s(f.convert_string("FtMesh ij"));
        for(auto c : *s)
        {
            ftmesh::mesh::pointer_t m(c->get_mesh());
            ftmesh::point::vector_t const & p(m->get_points());
            ftmesh::mesh::index_vector_t const & i(m->get_indexes());
//std::cerr << "Mesh advance = " << c->get_advance() << " (" << i.size() << " indexes)\n";
            for(std::size_t j(0); j < i.size(); ++j)
            {
                std::size_t const start(i[j]);
                std::size_t const end(j + 1 >= i.size() ? p.size() : i[j + 1]);
                std::size_t const size(end - start);

//std::cerr << "  " << size << " points: [" << start << ".." << end << "]\n ";

                for(std::size_t k(0); k < size; ++k)
                {
std::cerr << " (" << p[start + k].x() << ", " << p[start + k].y() << ")";
                }
std::cerr << "\n";
            }
            int advance(c->get_advance());
std::cerr << "advance by " << advance << " to next glyph\n";
        }
    }
    CATCH_END_SECTION()
}


// vim: ts=4 sw=4 et
