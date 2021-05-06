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
 * \brief Implementation of the ftpolygon class.
 *
 * The FreeType library gives us a list of tagged vertices which we need
 * to transform in a simple polygon. Then we can transform that polygon
 * to a mesh using the glut.
 *
 * \private
 */

// self
//
#include    <ftmesh/polygon.h>


// C++ lib
//
#include    <algorithm>


// last include
//
#include    <snapdev/poison.h>





namespace ftmesh
{


constexpr unsigned int const        BEZIER_STEPS = 5;





polygon::polygon(FT_Vector * contour, char * tags, unsigned int n)
{
    if(n < 3)
    {
        throw std::logic_error("the polygon constructor expects at least 3 vectors in the contour");
    }

    point prev;
    point cur(contour[n - 1].x, contour[n - 1].y);  // we know n >= 3
    point next(contour[0].x, contour[0].y);

    double olddir(0.0);
    double dir((next - cur).angle());
    double angle(0.0);

    // see http://freetype.sourceforge.net/freetype2/docs/glyphs/glyphs-6.html
    // for a full description of FreeType tags
    //
    for(unsigned int i(0); i < n; ++i)
    {
        prev = cur;
        cur = next;
        int const next_pos(i + 1 == n ? 0 : i + 1);
        next = point(contour[next_pos].x, contour[next_pos].y);
        olddir = dir;
        dir = (next - cur).angle();

        // compute our path new direction.
        //
        double t(dir - olddir);
        if(t < -M_PI)
        {
            t += 2 * M_PI;
        }
        if(t > M_PI)
        {
            t -= 2 * M_PI;
        }
        angle += t;

        // only process point tags we know.
        //
        if(n < 2
        || FT_CURVE_TAG(tags[i]) == FT_CURVE_TAG_ON)
        {
            add_point(cur);
        }
        else if(FT_CURVE_TAG(tags[i]) == FT_CURVE_TAG_CONIC)
        {
            point prev2(prev);
            point next2(next);

            // previous point is either the real previous point (an "on"
            // point), or the midpoint between the current one and the
            // previous "conic off" point.
            //
            if(FT_CURVE_TAG(tags[(i - 1 + n) % n]) == FT_CURVE_TAG_CONIC)
            {
                prev2 = (cur + prev) * 0.5;
                add_point(prev2);
            }

            // next point is either the real next point or the midpoint.
            //
            if(FT_CURVE_TAG(tags[(i + 1) % n]) == FT_CURVE_TAG_CONIC)
            {
                next2 = (cur + next) * 0.5;
            }

            evaluate_quadratic_curve(
                      prev2
                    , cur
                    , next2);
        }
        else if(FT_CURVE_TAG(tags[i          ]) == FT_CURVE_TAG_CUBIC
             && FT_CURVE_TAG(tags[(i + 1) % n]) == FT_CURVE_TAG_CUBIC)
        {
            int const after_next_pos((i + 2) % n);
            point const after_next(
                              contour[after_next_pos].x
                            , contour[after_next_pos].y);
            evaluate_cubic_curve(
                      prev
                    , cur
                    , next
                    , after_next);
        }
    }

    // if final angle is positive (+2PI), it's an anti-clockwise polygon,
    // otherwise (-2PI) it's clockwise.
    //
    f_clockwise = angle < 0.0;
}


std::size_t polygon::size() const
{
    return f_points.size();
}


point const & polygon::at(int idx) const
{
    // idx is often used with +1, +2 or -1
    //
    if(idx >= static_cast<int>(size()))
    {
        idx -= size();
    }
    else if(idx < 0)
    {
        idx += size();
    }
    return f_points.at(idx);
}


void polygon::evaluate_quadratic_curve(
          point const & a
        , point const & b
        , point const & c)
{
    for(unsigned int i(1); i < BEZIER_STEPS; i++)
    {
        double const t(static_cast<double>(i) / static_cast<double>(BEZIER_STEPS));
        double const nt(1.0 - t);
        point const u(a * nt + b * t);
        point const v(b * nt + c * t);
        add_point(u * nt + v * t);
    }
}


void polygon::evaluate_cubic_curve(
          point const & a
        , point const & b
        , point const & c
        , point const & d)
{
    for(unsigned int i = 0; i < BEZIER_STEPS; i++)
    {
        double const t(static_cast<double>(i) / static_cast<double>(BEZIER_STEPS));
        double const nt(1.0 - t);
        point const u(a * nt + b * t);
        point const v(b * nt + c * t);
        point const w(c * nt + d * t);
        point const m(u * nt + v * t);
        point const n(v * nt + w * t);
        add_point(m * nt + n * t);
    }
}


void polygon::add_point(point const & p)
{
    // try to avoid duplicates as it doesn't play well with glut tessellation
    //
    // HOWEVER, the current algorithm prevents points from going through
    // the starting point again, which I think is a mistake, that could
    // happen in the middle of the contour (but that's probably very rare)
    //
    if(f_points.empty()
    || (p != f_points.front() && p != f_points.back()))
    {
        f_points.push_back(p);
        if(p.is_left_of(f_leftmost))
        {
            f_leftmost = p;
        }
    }
}


point const & polygon::leftmost() const
{
    return f_leftmost;
}


void polygon::apply_parity(int parity)
{
    // contour orientation reversed?
    //
    // TBD: this would be important if we were to use the outset,
    //      I don't think it is for calculating the mesh
    //
    if((parity & 1) == 0 ^ f_clockwise)
    {
        std::reverse(f_points.begin(), f_points.end());
        f_clockwise = !f_clockwise;
    }
}



} // namespace ftmesh
// vim: ts=4 sw=4 et
