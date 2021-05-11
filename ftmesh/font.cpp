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
#include    "ftmesh/font.h"

#include    "ftmesh/polygon.h"


// snapdev lib
//
#include    <snapdev/not_used.h>


// libutf8 lib
//
#include    <libutf8/libutf8.h>


// snaplogger lib
//
#include    <snaplogger/message.h>


// OpenGL/GLUT libs
//
#include    <GL/glu.h>


// FreeType lib
//
// ft2build.h must come first
#include    <ft2build.h>

#include    FT_FREETYPE_H
#include    FT_GLYPH_H
#include    FT_OUTLINE_H


// C++ lib
//
#include    <iostream>


// last include
//
#include    <snapdev/poison.h>





FT_Library  g_ft_library;


namespace ftmesh
{


namespace
{


constexpr FT_Long const DEFAULT_FACE_INDEX = 0;



class auto_init_freetype_library
{
public:
    auto_init_freetype_library()
    {
        int e(FT_Init_FreeType(&g_ft_library));
        if(e != 0)
        {
            SNAP_LOG_ERROR
                << "An error occurred initializing the FreeType library ("
                << e
                << ")"
                << SNAP_LOG_SEND;
        }
    }
};

auto_init_freetype_library       g_auto_init_freetype_library = auto_init_freetype_library();




} // no name namespace


namespace detail
{


//////////////
// font_impl


// in order to hide all the FreeType headers, we use an internal implementation
//
class font_impl
{
public:
    typedef std::shared_ptr<font_impl>
                            pointer_t;

                            font_impl(std::string const & filename);
                            font_impl(font_impl const &) = delete;
                            ~font_impl();
    font_impl &             operator = (font_impl const &) = delete;

    mesh::pointer_t         get_mesh(char32_t glyph);
    void                    set_precision(int precision);
    bool                    has_kerning_table() const;
    void                    set_size(int point_size, int x_resolution, int y_resolution);
    float                   get_kerning(char32_t current_char, char32_t next_char);

private:
    // WARNING: the callback parameters are not what is defined in the
    //          documentation because the function used to set them up
    //          requires a cast either way and so I just use our types
    //          directly as long as compatible with the docs.
    //
    static void             tess_callback_edge(GLboolean edge, font_impl * impl);
    static void             tess_callback_begin(GLenum type, font_impl * impl);
    static void             tess_callback_vertex(GLdouble const * vertex, font_impl * impl);
    static void             tess_callback_combine(
                                      GLdouble coords[3]
                                    , GLdouble * vertex_data[4]
                                    , GLfloat weight[4]
                                    , GLdouble ** out_data
                                    , font_impl * impl);
    static void             tess_callback_end(font_impl * impl);
    static void             tess_callback_error(GLenum errCode, font_impl * impl);

    void                    callback_begin();
    void                    callback_vertex(point const & p);
    point::pointer_t        callback_combine(point const & p);
    void                    callback_end();
    void                    callback_error(GLenum errCode);

    std::string const       f_filename = std::string();
    FT_Face                 f_face = FT_Face();
    mesh::pointer_t         f_current_mesh = mesh::pointer_t();
    int                     f_precision = DEFAULT_UPSCALE;
    std::size_t             f_temporary_vertex_pos = 0;
    point::safe_vector_t    f_temporary_vertex = point::safe_vector_t();       
};


font_impl::font_impl(std::string const & font)
    : f_filename(font)
{
    FT_Error e(FT_New_Face(
              g_ft_library
            , f_filename.c_str()
            , DEFAULT_FACE_INDEX
            , &f_face));
    if(e != FT_Err_Ok)
    {
        throw std::runtime_error(
                  "FT_New_Face() could not load \""
                + f_filename
                + "\" (FT_Error: "
                + std::to_string(e)
                + ")");
    }

    e = FT_Select_Charmap(f_face, FT_ENCODING_UNICODE);
    if(e != FT_Err_Ok)
    {
        throw std::runtime_error(
                  "FT_New_Face() could not set Unicode Charmap for \""
                + f_filename
                + "\" (FT_Error: "
                + std::to_string(e)
                + ")");
    }

    // make sure to define a default size which is always the same
    //
    set_size(DEFAULT_SIZE, DEFAULT_RESOLUTION, DEFAULT_RESOLUTION);
}


font_impl::~font_impl()
{
    FT_Done_Face(f_face);
}


mesh::pointer_t font_impl::get_mesh(char32_t glyph)
{
    FT_UInt const index(FT_Get_Char_Index(f_face, glyph));
    int const e(FT_Load_Glyph(f_face, index, FT_LOAD_DEFAULT));
    if(e != FT_Err_Ok
    || f_face->glyph == nullptr)     // the load failed
    {
        return mesh::pointer_t();
    }

    if(f_face->glyph->format != FT_GLYPH_FORMAT_OUTLINE)
    {
        // valid load, not a valid format, we expected vertices (an outline)
        //
        return mesh::pointer_t();
    }

    int start_index(0);
    int end_index(0);

    polygon::vector_t polygons(f_face->glyph->outline.n_contours);

    for(std::size_t i(0); i < polygons.size(); ++i)
    {
        end_index = f_face->glyph->outline.contours[i] + 1;

        polygons[i] = std::make_shared<polygon>(
                                  f_face->glyph->outline.points + start_index
                                , f_face->glyph->outline.tags + start_index
                                , end_index - start_index);

        start_index = end_index;
    }

    // compute the parity of each polygon
    //
    // FIXME: see whether FT_Outline_Get_Orientation can do it for us.
    //
    for(std::size_t i(0); i < polygons.size(); ++i)
    {
        polygon::pointer_t c1(polygons[i]);
        point const leftmost(c1->leftmost());
        int parity(0);
        for(std::size_t j(0); j < polygons.size(); ++j)
        {
            if(j == i)
            {
                continue;
            }

            polygon::pointer_t c2(polygons[j]);
            for(size_t n(0); n < c2->size(); ++n)
            {
                point const p1(c2->at(n));
                point const p2(c2->at(n + 1));

                /* FIXME: combinations of >= > <= and < do not seem stable */
                if((p1.y() <  leftmost.y() && p2.y() <  leftmost.y())
                || (p1.y() >= leftmost.y() && p2.y() >= leftmost.y())
                || (p1.x() >  leftmost.x() && p2.x() >  leftmost.x()))
                {
                    ;
                }
                else if(p1.x() < leftmost.x()
                     && p2.x() < leftmost.x())
                {
                    parity++;
                }
                else
                {
                    point const a(p1 - leftmost);
                    point const b(p2 - leftmost);
                    if(b.x() * a.y() > b.y() * a.x())
                    {
                        parity++;
                    }
                }
            }
        }

        c1->apply_parity(parity);
    }

    f_current_mesh = std::make_shared<mesh>(static_cast<float>(f_face->glyph->advance.x) / static_cast<float>(f_precision));
    f_temporary_vertex_pos = 0;

    GLUtesselator * tobj(gluNewTess());

    gluTessCallback(tobj, GLU_TESS_EDGE_FLAG_DATA, reinterpret_cast<_GLUfuncptr>(&tess_callback_edge));
    gluTessCallback(tobj, GLU_TESS_BEGIN_DATA,     reinterpret_cast<_GLUfuncptr>(&tess_callback_begin));
    gluTessCallback(tobj, GLU_TESS_VERTEX_DATA,    reinterpret_cast<_GLUfuncptr>(&tess_callback_vertex));
    gluTessCallback(tobj, GLU_TESS_COMBINE_DATA,   reinterpret_cast<_GLUfuncptr>(&tess_callback_combine));
    gluTessCallback(tobj, GLU_TESS_END_DATA,       reinterpret_cast<_GLUfuncptr>(&tess_callback_end));
    gluTessCallback(tobj, GLU_TESS_ERROR_DATA,     reinterpret_cast<_GLUfuncptr>(&tess_callback_error));

    if((f_face->glyph->outline.flags & FT_OUTLINE_EVEN_ODD_FILL) != 0) // ft_outline_reverse_fill
    {
        gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD);
    }
    else
    {
        gluTessProperty(tobj, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_NONZERO);
    }

    gluTessProperty(tobj, GLU_TESS_BOUNDARY_ONLY, GLU_FALSE);
    gluTessProperty(tobj, GLU_TESS_TOLERANCE, 0.0);
    gluTessNormal(tobj, 0.0, 0.0, 1.0);

    gluTessBeginPolygon(tobj, this);

        for(std::size_t c(0); c < polygons.size(); ++c)
        {
            gluTessBeginContour(tobj);

                polygon::pointer_t polygon(polygons[c]);
                for(std::size_t p(0); p < polygon->size(); ++p)
                {
                    gluTessVertex(
                          tobj
                        , const_cast<double *>(polygon->at(p).f_coordinates)
                        , const_cast<double *>(polygon->at(p).f_coordinates));
                }

            gluTessEndContour(tobj);
        }

    gluTessEndPolygon(tobj);

    gluDeleteTess(tobj);

    mesh::pointer_t result;
    f_current_mesh.swap(result);
    return result;
}


void font_impl::set_precision(int precision)
{
    if(precision <= 0)
    {
        throw std::runtime_error("the precision must be positive");
    }

    f_precision = precision;

    // TBD: should we call the set_size() funciton? if we want to be able to
    // do that, we need to save the point and resolution parameters...
}


bool font_impl::has_kerning_table() const
{
    return FT_HAS_KERNING(f_face) != 0; 
}


/** \brief Set the size of the font.
 *
 * This function sets the size of the font. You must have called the
 * set_precision() function first if you want your new precision to work.
 *
 * The \p point_size is the usual \em point definition. More or less, the
 * height, knowing that it is not going to match the pixel height.
 *
 * \note
 * The constructor calls this function once with:
 *
 * \code
 *     set_size(DEFAULT_SIZE, DEFAULT_RESOLUTION, DEFAULT_RESOLUTION);
 * \endcode
 *
 * \warning
 * It is important to call this function BEFORE you ever call the get_mesh()
 * or convert_string(). Also, calling this function AFTER will not work as
 * expected (i.e. it doesn't dynamically reset all the existing glyph and
 * restart with the new size). In a future version, we may fix this issue.
 *
 * \param[in] point_size  The font height to use.
 * \param[in] x_resolution  The horizontal resolution, 72 by default.
 * \param[in] x_resolution  The vertical resolution, 72 by default.
 */
void font_impl::set_size(int point_size, int x_resolution, int y_resolution)
{
    int const e(FT_Set_Char_Size(
              f_face
            , 0L
            , point_size * f_precision
            , x_resolution
            , y_resolution));
    if(e != FT_Err_Ok)
    {
        SNAP_LOG_ERROR
            << "FT_Set_Char_Size() failed with error #"
            << e
            << " for point: "
            << point_size * f_precision
            << " (including the upscaling), horizontal resolution: "
            << x_resolution
            << ", vertical resolution: "
            << y_resolution
            << SNAP_LOG_SEND;
    }
}


float font_impl::get_kerning(char32_t current_char, char32_t next_char)
{
    FT_Vector kern_advance = FT_Vector();

    //if(has_kerning_table()) -- TBD
    {
        FT_UInt const current_index(FT_Get_Char_Index(f_face, current_char));
        FT_UInt const next_index(FT_Get_Char_Index(f_face, next_char));
        int const e(FT_Get_Kerning(
                  f_face
                , current_index
                , next_index
                , FT_KERNING_UNFITTED
                , &kern_advance));
        if(e != FT_Err_Ok)
        {
            // this is probably common?
        }
    }

    return static_cast<float>(kern_advance.x) / static_cast<float>(f_precision);
}


void font_impl::tess_callback_edge(GLboolean edge, font_impl * impl)
{
    // we have this callback to force the GLU library to only create
    // triangles (GL_TRIANGLES); that way we avoid the GL_TRIANGLE_FAN
    // and GL_TRIANGLE_STRIP and reduce the amount of possible cases
    // outside of this library
    //
    snap::NOTUSED(edge);
    snap::NOTUSED(impl);
}


void font_impl::tess_callback_begin(GLenum type, font_impl * impl)
{
    if(type != GL_TRIANGLES)
    {
        throw std::runtime_error("we expected the type of the tessellation to be GL_TRIANGLES");
    }

    impl->callback_begin();
}


void font_impl::tess_callback_vertex(GLdouble const * vertex, font_impl * impl)
{
    impl->callback_vertex(point(vertex[0], vertex[1]));
}


void font_impl::tess_callback_combine(
          GLdouble vertex[3]
        , GLdouble * vertex_data[4]
        , GLfloat weight[4]
        , GLdouble ** out_data
        , font_impl * impl)
{
    snap::NOTUSED(vertex_data);
    snap::NOTUSED(weight);

    point::pointer_t p(impl->callback_combine(point(vertex[0], vertex[1])));
    *out_data = p->f_coordinates;
    //const FTGL_DOUBLE* vertex = static_cast<const FTGL_DOUBLE*>(coords);
    //*outData = const_cast<FTGL_DOUBLE*>(mesh->Combine(vertex[0], vertex[1], vertex[2]));
}


void font_impl::tess_callback_end(font_impl * impl)
{
    impl->callback_end();
}


void font_impl::tess_callback_error(GLenum error_code, font_impl * impl)
{
    impl->callback_error(error_code);
}


void font_impl::callback_error(GLenum error_code)
{
    snap::NOTUSED(error_code);
}


void font_impl::callback_begin()
{
    f_current_mesh->begin();
}


void font_impl::callback_end()
{
    f_current_mesh->end();
}


void font_impl::callback_vertex(point const & p)
{
    f_current_mesh->add_point(point(p.x() / f_precision, p.y() / f_precision));
}


point::pointer_t font_impl::callback_combine(point const & p)
{
    // the temporary vertex is used to have points that are allocated
    // and do not move in memory while building the mesh, once done
    // they get deleted
    //
    // WARNING: the same pointers are reused between glyphs, in other words
    //          the tessellation is not thread safe at all (but at least we
    //          avoid many alloc/free/re-alloc/re-free/...)
    //
    if(f_temporary_vertex_pos < f_temporary_vertex.size())
    {
        *f_temporary_vertex[f_temporary_vertex_pos] = p;
    }
    else
    {
        f_temporary_vertex.push_back(std::make_shared<point>(p));
    }

    point::pointer_t result(f_temporary_vertex[f_temporary_vertex_pos]);

    ++f_temporary_vertex_pos;

    return result;
}




}




///////////
// ftfont

font::font(std::string const & filename)
    : f_impl(std::make_shared<detail::font_impl>(filename))
{
}


void font::set_precision(int precision)
{
    f_impl->set_precision(precision);
}


void font::set_size(int point, int x_resolution, int y_resolution)
{
    f_impl->set_size(point, x_resolution, y_resolution);
}



mesh::pointer_t font::get_mesh(char32_t glyph)
{
    auto it(f_map.find(glyph));
    if(it != f_map.end())
    {
        return it->second;
    }

    // not yet cached, build the mesh now
    //
    mesh::pointer_t result(f_impl->get_mesh(glyph));
    f_map[glyph] = result;

    return result;
}


mesh_string::pointer_t font::convert_string(std::string const & message)
{
    mesh_string::pointer_t result(std::make_shared<mesh_string>());

    std::u32string const s(libutf8::to_u32string(message));
    std::size_t const size(s.size());
    if(size > 0)
    {
        std::size_t const max(size - 1);
        for(std::size_t i(0); i < max; ++i)
        {
            mesh::pointer_t m(get_mesh(s[i]));
            if(m != nullptr)
            {
                float advance(m->get_advance());
                advance += f_impl->get_kerning(s[i], s[i + 1]);
                result->add_glyph(m, advance);
            }
        }
        mesh::pointer_t m(get_mesh(s[max]));
        if(m != nullptr)
        {
            result->add_glyph(m, m->get_advance());
        }
    }

    return result;
}


float font::string_width(std::string const & message)
{
    float result(0.0f);

    std::u32string const s(libutf8::to_u32string(message));
    std::size_t const size(s.size());
    if(size > 0)
    {
        std::size_t const max(size - 1);
        for(std::size_t i(0); i < max; ++i)
        {
            mesh::pointer_t m(get_mesh(s[i]));
            if(m != nullptr)
            {
                result += m->get_advance();
                result += f_impl->get_kerning(s[i], s[i + 1]);
            }
        }
        mesh::pointer_t m(get_mesh(s[max]));
        if(m != nullptr)
        {
            result += m->get_advance();
        }
    }

    return result;
}




} // namespace ftmesh
// vim: ts=4 sw=4 et
