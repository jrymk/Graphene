#ifndef IMGUIBZ2_H_
#define IMGUIBZ2_H_

/* --------------------------------------------------------------------------

This program, "bzip2", the associated library "libbzip2", and all
documentation, are copyright (C) 1996-2007 Julian R Seward.  All
rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions
are met:

1. Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.

2. The origin of this software must not be misrepresented; you must
   not claim that you wrote the original software.  If you use this
   software in a product, an acknowledgment in the product
   documentation would be appreciated but is not required.

3. Altered source versions must be plainly marked as such, and must
   not be misrepresented as being the original software.

4. The name of the author may not be used to endorse or promote
   products derived from this software without specific prior written
   permission.

THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

Julian Seward, jseward@bzip.org
bzip2/libbzip2 version 1.0.5 of 10 December 2007

--------------------------------------------------------------------------*/

// ROADMAP:
// 1) UPDATE TO NEWER VERSION: TODO
// 2) EXPOSE A SIMPLER AND CLEANER API, MORE IMGUI-FRIENDLY,
//    HIDING ALL INCLUDED HEADERS IN THE CPP FILE IF POSSIBLE: DONE


/*-------------------------------------------------------------*/
/*--- Public header file for the library.                   ---*/
/*---                                               bzlib.h ---*/
/*-------------------------------------------------------------*/


// imguihelper.h has methods to decompress directly from a file path AFAIR

#ifndef IMGUI_API
#include "../../../imgui.h"  // ImVector
#endif //IMGUI_API


namespace ImGui {

#ifndef BZ_DECOMPRESS_ONLY
    IMGUI_API bool Bz2CompressFromMemory(const char* memoryBuffer, int memoryBufferSize, ImVector<char>& rv, bool clearRvBeforeUsage=true);
#endif //BZ_DECOMPRESS_ONLY

    IMGUI_API bool Bz2DecompressFromMemory(const char* memoryBuffer, int memoryBufferSize, ImVector<char>& rv, bool clearRvBeforeUsage=true);

} //namespace ImGui

#endif // IMGUIBZ2_H_

