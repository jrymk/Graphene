/*
 * edtaa3()
 *
 * Sweep-and-update Euclidean distance transform of an
 * image. Positive pixels are treated as object pixels,
 * zero or negative pixels are treated as background.
 * An attempt is made to treat antialiased edges correctly.
 * The input image must have pixels in the range [0,1],
 * and the antialiased image should be a box-filter
 * sampling of the ideal, crisp edge.
 * If the antialias region is more than 1 pixel wide,
 * the result from this transform will be inaccurate.
 *
 * By Stefan Gustavson (stefan.gustavson@gmail.com).
 *
 * Originally written in 1994, based on a verbal
 * description of the SSED8 algorithm published in the
 * PhD dissertation of Ingemar Ragnemalm. This is his
 * algorithm, I only implemented it in C.
 *
 * Updated in 2004 to treat border pixels correctly,
 * and cleaned up the code to improve readability.
 *
 * Updated in 2009 to handle anti-aliased edges.
 *
 * Updated in 2011 to avoid a corner case infinite loop.
 *
 * Updated 2012 to change license from LGPL to MIT.
 *
 * Updated 2014 to fix a bug in the gradient computations. Ahem.
 *
 */

/*
 Copyright (C) 2009-2012 Stefan Gustavson (stefan.gustavson@gmail.com)
 The code in this file is distributed under the MIT license:

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 */

// Flix01: Added the EDTAA3FUNC_HAS_IMGUI_SUPPORT definition that can be used
// to provide Dear Imgui (at https://github.com/ocornut/imgui) the method:
// ImGui::PostBuildForSignedDistanceFontEffect(ImFontAtlas* atlas).
// USAGE:
/*
// In imgui_impl_glfw.cpp (or similiar):
#define IMIMPL_BUILD_SDF // Twickable
#ifdef IMIMPL_BUILD_SDF
#define EDTAA3FUNC_HAS_IMGUI_SUPPORT
#include "edtaa3func.h"
#endif //IMIMPL_BUILD_SDF
bool ImGui_ImplGlfw_CreateFontsTexture()
{
    // Build texture atlas
    ImGuiIO& io = ImGui::GetIO();
    unsigned char* pixels;
    int width, height;

    // New code -------------------------------------------------
    if (io.Fonts->ConfigData.empty()) io.Fonts->AddFontDefault();
    io.Fonts->Build(); // (change this line if you use [imgui_freetype](https://github.com/Vuhdo/imgui or https://github.com/ocornut/imgui/tree/master/misc/freetype) appropriately)
#   ifdef IMIMPL_BUILD_SDF
    ImGui::PostBuildForSignedDistanceFontEffect(io.Fonts);
#   endif //IMIMPL_BUILD_SDF
    // End new code ----------------------------------------------
    ...
}


// UPDATE: Now in ImGui::PostBuildForSignedDistanceFontEffect(...):
// if atlas->TexPixelsRGBA32 is not NULL, the whole RGBA32 texture gets processed.
*/

// The methods ImGuiEdtaaHelper::make_distance_mapd(...) and ImGuiEdtaaHelper::make_distance_mapb(...)
// are based on https://github.com/rougier/freetype-gl/blob/master/distance-field.c [Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.]
// freetype-gl license:
/* =========================================================================
 * Freetype GL - A C OpenGL Freetype engine
 * Platform:    Any
 * WWW:         https://github.com/rougier/freetype-gl
 * -------------------------------------------------------------------------
 * Copyright 2011,2012 Nicolas P. Rougier. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY NICOLAS P. ROUGIER ''AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL NICOLAS P. ROUGIER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * The views and conclusions contained in the software and documentation are
 * those of the authors and should not be interpreted as representing official
 * policies, either expressed or implied, of Nicolas P. Rougier.
* =========================================================================
*/

#ifndef EDTAA3FUNC_H_
#define EDTAA3FUNC_H_
#pragma once

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif //__cplusplus

/*
 * Compute the local gradient at edge pixels using convolution filters.
 * The gradient is computed only at edge pixels. At other places in the
 * image, it is never used, and it's mostly zero anyway.
 */
void computegradient(double *img, int w, int h, double *gx, double *gy)
{
    int i,j,k,p,q;
    double glength, phi, phiscaled, ascaled, errsign, pfrac, qfrac, err0, err1, err;
#define SQRT2 1.4142136
    for(i = 1; i < h-1; i++) { // Avoid edges where the kernels would spill over
        for(j = 1; j < w-1; j++) {
            k = i*w + j;
            if((img[k]>0.0) && (img[k]<1.0)) { // Compute gradient for edge pixels only
                gx[k] = -img[k-w-1] - SQRT2*img[k-1] - img[k+w-1] + img[k-w+1] + SQRT2*img[k+1] + img[k+w+1];
                gy[k] = -img[k-w-1] - SQRT2*img[k-w] - img[k-w+1] + img[k+w-1] + SQRT2*img[k+w] + img[k+w+1];
                glength = gx[k]*gx[k] + gy[k]*gy[k];
                if(glength > 0.0) { // Avoid division by zero
                    glength = sqrt(glength);
                    gx[k]=gx[k]/glength;
                    gy[k]=gy[k]/glength;
                }
            }
        }
    }
    // TODO: Compute reasonable values for gx, gy also around the image edges.
    // (These are zero now, which reduces the accuracy for a 1-pixel wide region
	// around the image edge.) 2x2 kernels would be suitable for this.
}

/*
 * A somewhat tricky function to approximate the distance to an edge in a
 * certain pixel, with consideration to either the local gradient (gx,gy)
 * or the direction to the pixel (dx,dy) and the pixel greyscale value a.
 * The latter alternative, using (dx,dy), is the metric used by edtaa2().
 * Using a local estimate of the edge gradient (gx,gy) yields much better
 * accuracy at and near edges, and reduces the error even at distant pixels
 * provided that the gradient direction is accurately estimated.
 */
double edgedf(double gx, double gy, double a)
{
    double df, glength, temp, a1;

    if ((gx == 0) || (gy == 0)) { // Either A) gu or gv are zero, or B) both
        df = 0.5-a;  // Linear approximation is A) correct or B) a fair guess
    } else {
        glength = sqrt(gx*gx + gy*gy);
        if(glength>0) {
            gx = gx/glength;
            gy = gy/glength;
        }
        /* Everything is symmetric wrt sign and transposition,
         * so move to first octant (gx>=0, gy>=0, gx>=gy) to
         * avoid handling all possible edge directions.
         */
        gx = fabs(gx);
        gy = fabs(gy);
        if(gx<gy) {
            temp = gx;
            gx = gy;
            gy = temp;
        }
        a1 = 0.5*gy/gx;
        if (a < a1) { // 0 <= a < a1
            df = 0.5*(gx + gy) - sqrt(2.0*gx*gy*a);
        } else if (a < (1.0-a1)) { // a1 <= a <= 1-a1
            df = (0.5-a)*gx;
        } else { // 1-a1 < a <= 1
            df = -0.5*(gx + gy) + sqrt(2.0*gx*gy*(1.0-a));
        }
    }    
    return df;
}

double distaa3(double *img, double *gximg, double *gyimg, int w, int c, int xc, int yc, int xi, int yi)
{
  double di, df, dx, dy, gx, gy, a;
  int closest;
  
  closest = c-xc-yc*w; // Index to the edge pixel pointed to from c
  a = img[closest];    // Grayscale value at the edge pixel
  gx = gximg[closest]; // X gradient component at the edge pixel
  gy = gyimg[closest]; // Y gradient component at the edge pixel
  
  if(a > 1.0) a = 1.0;
  if(a < 0.0) a = 0.0; // Clip grayscale values outside the range [0,1]
  if(a == 0.0) return 1000000.0; // Not an object pixel, return "very far" ("don't know yet")

  dx = (double)xi;
  dy = (double)yi;
  di = sqrt(dx*dx + dy*dy); // Length of integer vector, like a traditional EDT
  if(di==0) { // Use local gradient only at edges
      // Estimate based on local gradient only
      df = edgedf(gx, gy, a);
  } else {
      // Estimate gradient based on direction to edge (accurate for large di)
      df = edgedf(dx, dy, a);
  }
  return di + df; // Same metric as edtaa2, except at edges (where di=0)
}

// Shorthand macro: add ubiquitous parameters dist, gx, gy, img and w and call distaa3()
#define DISTAA(c,xc,yc,xi,yi) (distaa3(img, gx, gy, w, c, xc, yc, xi, yi))

void edtaa3(double *img, double *gx, double *gy, int w, int h, short *distx, short *disty, double *dist)
{
  int x, y, i, c;
  int offset_u, offset_ur, offset_r, offset_rd,
  offset_d, offset_dl, offset_l, offset_lu;
  double olddist, newdist;
  int cdistx, cdisty, newdistx, newdisty;
  int changed;
  double epsilon = 1e-3;

  /* Initialize index offsets for the current image width */
  offset_u = -w;
  offset_ur = -w+1;
  offset_r = 1;
  offset_rd = w+1;
  offset_d = w;
  offset_dl = w-1;
  offset_l = -1;
  offset_lu = -w-1;

  /* Initialize the distance images */
  for(i=0; i<w*h; i++) {
    distx[i] = 0; // At first, all pixels point to
    disty[i] = 0; // themselves as the closest known.
    if(img[i] <= 0.0)
      {
	dist[i]= 1000000.0; // Big value, means "not set yet"
      }
    else if (img[i]<1.0) {
      dist[i] = edgedf(gx[i], gy[i], img[i]); // Gradient-assisted estimate
    }
    else {
      dist[i]= 0.0; // Inside the object
    }
  }

  /* Perform the transformation */
  do
    {
      changed = 0;

      /* Scan rows, except first row */
      for(y=1; y<h; y++)
        {

          /* move index to leftmost pixel of current row */
          i = y*w;

          /* scan right, propagate distances from above & left */

          /* Leftmost pixel is special, has no left neighbors */
          olddist = dist[i];
          if(olddist > 0) // If non-zero distance or not set yet
            {
	      c = i + offset_u; // Index of candidate for testing
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_ur;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  changed = 1;
                }
            }
          i++;

          /* Middle pixels have all neighbors */
          for(x=1; x<w-1; x++, i++)
            {
              olddist = dist[i];
              if(olddist <= 0) continue; // No need to update further

	      c = i+offset_l;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_lu;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_u;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_ur;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  changed = 1;
                }
            }

          /* Rightmost pixel of row is special, has no right neighbors */
          olddist = dist[i];
          if(olddist > 0) // If not already zero distance
            {
	      c = i+offset_l;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_lu;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_u;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty+1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  changed = 1;
                }
            }

          /* Move index to second rightmost pixel of current row. */
          /* Rightmost pixel is skipped, it has no right neighbor. */
          i = y*w + w-2;

          /* scan left, propagate distance from right */
          for(x=w-2; x>=0; x--, i--)
            {
              olddist = dist[i];
              if(olddist <= 0) continue; // Already zero distance

	      c = i+offset_r;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  changed = 1;
                }
            }
        }
      
      /* Scan rows in reverse order, except last row */
      for(y=h-2; y>=0; y--)
        {
          /* move index to rightmost pixel of current row */
          i = y*w + w-1;

          /* Scan left, propagate distances from below & right */

          /* Rightmost pixel is special, has no right neighbors */
          olddist = dist[i];
          if(olddist > 0) // If not already zero distance
            {
	      c = i+offset_d;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_dl;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  changed = 1;
                }
            }
          i--;

          /* Middle pixels have all neighbors */
          for(x=w-2; x>0; x--, i--)
            {
              olddist = dist[i];
              if(olddist <= 0) continue; // Already zero distance

	      c = i+offset_r;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_rd;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_d;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
                  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_dl;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
                  dist[i]=newdist;
                  changed = 1;
                }
            }
          /* Leftmost pixel is special, has no left neighbors */
          olddist = dist[i];
          if(olddist > 0) // If not already zero distance
            {
	      c = i+offset_r;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
                  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_rd;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx-1;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
		  dist[i]=newdist;
                  olddist=newdist;
                  changed = 1;
                }

	      c = i+offset_d;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx;
              newdisty = cdisty-1;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
                  dist[i]=newdist;
                  changed = 1;
                }
            }

          /* Move index to second leftmost pixel of current row. */
          /* Leftmost pixel is skipped, it has no left neighbor. */
          i = y*w + 1;
          for(x=1; x<w; x++, i++)
            {
              /* scan right, propagate distance from left */
              olddist = dist[i];
              if(olddist <= 0) continue; // Already zero distance

	      c = i+offset_l;
	      cdistx = distx[c];
	      cdisty = disty[c];
              newdistx = cdistx+1;
              newdisty = cdisty;
              newdist = DISTAA(c, cdistx, cdisty, newdistx, newdisty);
              if(newdist < olddist-epsilon)
                {
                  distx[i]=newdistx;
                  disty[i]=newdisty;
                  dist[i]=newdist;
                  changed = 1;
                }
            }
        }
    }
  while(changed); // Sweep until no more updates are made

  /* The transformation is completed. */

}
#ifdef __cplusplus
} // extern "C"
#endif //__cplusplus

#ifdef EDTAA3FUNC_HAS_IMGUI_SUPPORT

#include "../../../imgui.h"

namespace ImGuiEdtaaHelper {
// The purpose of TmpData is to allow releasing the memory all togther
// at the end of a multiple call to make_distance_mapb(...)/make_distance_mapd(...)
// [However we don't use this feature here]
struct TmpData {
    ImVector<double> v_data;

    ImVector<short> v_xdist;
    ImVector<short> v_ydist;
    ImVector<double> v_gx;
    ImVector<double> v_gy;
    ImVector<double> v_outside;
    ImVector<double> v_inside;

    void freeMemory() {
        TmpData tmp;
        v_data.swap(tmp.v_data);
        v_xdist.swap(tmp.v_xdist);
        v_ydist.swap(tmp.v_ydist);
        v_gx.swap(tmp.v_gx);
        v_outside.swap(tmp.v_outside);
        v_inside.swap(tmp.v_inside);
    }
};
static TmpData tmpData;
static void FreeTmpMemory() {tmpData.freeMemory();}

// These two methods are adapted from: https://github.com/rougier/freetype-gl (Copyright 2011,2012 Nicolas P. Rougier),
// so that the memory they use can optionally be freed once, after all the glyphs are processed.
static double * make_distance_mapd( double *data, unsigned int width, unsigned int height, TmpData* pOptionalTmpData=NULL)
{
    if (!pOptionalTmpData) pOptionalTmpData=&tmpData;

    const unsigned int area = width * height;
    IM_ASSERT(area>0);
    pOptionalTmpData->v_xdist.resize(area);         short* xdist =  &pOptionalTmpData->v_xdist[0];
    pOptionalTmpData->v_ydist.resize(area);         short* ydist =  &pOptionalTmpData->v_ydist[0];
    pOptionalTmpData->v_gx.resize(area);            double* gx =    &pOptionalTmpData->v_gx[0];
    pOptionalTmpData->v_gy.resize(area);            double* gy =    &pOptionalTmpData->v_gy[0];
    pOptionalTmpData->v_outside.resize(area);       double* outside=&pOptionalTmpData->v_outside[0];
    pOptionalTmpData->v_inside.resize(area);        double* inside= &pOptionalTmpData->v_inside[0];

    double vmin = DBL_MAX;
    unsigned int i;

    // Compute outside = edtaa3(bitmap); % Transform background (0's)
    computegradient( data, width, height, gx, gy);
    edtaa3(data, gx, gy, width, height, xdist, ydist, outside);
    for( i=0; i<area; ++i)  {
        if( outside[i] < 0.0 )  outside[i] = 0.0;
    }

    // Compute inside = edtaa3(1-bitmap); % Transform foreground (1's)
    memset( gx, 0, sizeof(double)*area );
    memset( gy, 0, sizeof(double)*area );
    for( i=0; i<area; ++i)  data[i] = 1 - data[i];
    computegradient( data, width, height, gx, gy );
    edtaa3( data, gx, gy, width, height, xdist, ydist, inside );
    for( i=0; i<area; ++i ) {
        if( inside[i] < 0 ) inside[i] = 0.0;
    }

    // distmap = outside - inside; % Bipolar distance field
    for( i=0; i<area; ++i)  {
        outside[i] -= inside[i];
        if( outside[i] < vmin ) vmin = outside[i];
    }

    vmin = fabs(vmin);

    for( i=0; i<area; ++i)  {
        double v = outside[i];
        if     ( v < -vmin) outside[i] = -vmin;
        else if( v > +vmin) outside[i] = +vmin;
        data[i] = (outside[i]+vmin)/(2*vmin);
    }

    return data;
}
static unsigned char * make_distance_mapb(const unsigned char *img,unsigned int width, unsigned int height, unsigned char* imgOutSameSize, TmpData* pOptionalTmpData=NULL)
{
    IM_ASSERT(imgOutSameSize);

    if (!pOptionalTmpData) pOptionalTmpData=&tmpData;

    const unsigned int area = width * height;
    IM_ASSERT(area>0);
    pOptionalTmpData->v_data.resize(area);
    double* data =  &pOptionalTmpData->v_data[0];

    unsigned int i;

    // find minimimum and maximum values
    double img_min = DBL_MAX,img_max = DBL_MIN,v;

    for( i=0; i<area; ++i)  {
        data[i] = v = img[i];
        if (v > img_max)    img_max = v;
        if (v < img_min)    img_min = v;
    }

    // Map values from 0 - 255 to 0.0 - 1.0
    for( i=0; i<area; ++i)  data[i] = (img[i]-img_min)/img_max;

    data = make_distance_mapd(data, width, height,pOptionalTmpData);

    // map values from 0.0 - 1.0 to 0 - 255
    for( i=0; i<area; ++i)  imgOutSameSize[i] = (unsigned char)(255*(1-data[i]));

    return imgOutSameSize;
}
} //ImGuiEdtaaHelper


namespace ImGui {
inline void ProcessAlpha8ImageForSignedDistanceFontEffect(unsigned char* pixels,int w,int h)  {
    IM_ASSERT(pixels && w>0 && h>0);
    ImVector<unsigned char> tmpData;tmpData.resize(w*h);
    ImGuiEdtaaHelper::make_distance_mapb(pixels,w,h,&tmpData[0]);
    ImGuiEdtaaHelper::FreeTmpMemory();
    memcpy(pixels,&tmpData[0],w*h);
}

inline void PostBuildForSignedDistanceFontEffect(ImFontAtlas* atlas) {
    IM_ASSERT(!atlas->TexPixelsRGBA32 || atlas->TexPixelsAlpha8);
    /* // For some reasons this does not work when atlas->TexPixelsRGBA32 is present:
    if (!atlas->TexPixelsAlpha8) atlas->GetTexDataAsAlpha8(&atlas->TexPixelsAlpha8,NULL,NULL);
    ProcessAlpha8ImageForSignedDistanceFontEffect(atlas->TexPixelsAlpha8,atlas->TexWidth,atlas->TexHeight);
    if (atlas->TexPixelsRGBA32) {
        IM_ASSERT(atlas->TexPixelsAlpha8);
        // We must paste back atlas->TexPixelsAlpha8 into atlas->TexPixelsRGBA32
        const int w = atlas->TexWidth,h = atlas->TexHeight;
        unsigned char* dst = (unsigned char*) atlas->TexPixelsRGBA32;dst+=3;
        const unsigned char* src = atlas->TexPixelsAlpha8;
        for (int n=w*h;n>0;n--) {*dst = *src++;dst+=4;}
    }
    */
    if (atlas->TexPixelsRGBA32) {
        const int w = atlas->TexWidth,h = atlas->TexHeight;
        // We must extract the alpha channel from atlas->TexPixelsRGBA32
        ImVector<unsigned char> tmpData;tmpData.resize(w*h);
        unsigned char* dst = &tmpData[0];
        const unsigned char* src = (unsigned char*) atlas->TexPixelsRGBA32;src+=3;
        for (int n=w*h;n>0;n--) {*dst++=*src;src+=4;}

        ProcessAlpha8ImageForSignedDistanceFontEffect(&tmpData[0],w,h);

        // We must paste back tmpData into atlas->TexPixelsRGBA32
        dst = (unsigned char*) atlas->TexPixelsRGBA32;dst+=3;
        src = &tmpData[0];
        for (int n=w*h;n>0;n--) {*dst=*src++;dst+=4;}
    }
    else {
        if (!atlas->TexPixelsAlpha8) atlas->GetTexDataAsAlpha8(&atlas->TexPixelsAlpha8,NULL,NULL);
        ProcessAlpha8ImageForSignedDistanceFontEffect(atlas->TexPixelsAlpha8,atlas->TexWidth,atlas->TexHeight);
    }
}
}

#endif //EDTAA3FUNC_HAS_IMGUI_SUPPORT

#endif //EDTAA3FUNC_H_

