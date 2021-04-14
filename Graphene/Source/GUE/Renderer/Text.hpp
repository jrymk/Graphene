#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include "../../ExceptionHandler.hpp"

///FreeType 2.10.4
///============== =
///
///Homepage: https://www.freetype.org
///
///FreeType is a freely available software library to render fonts.


namespace gue {
	class Face {
	private:
		FT_Library* m_library;
		FT_Face* m_face = new FT_Face();

	public:
		Face(FT_Library library) {
			this->m_library = &library;
		}

		~Face() {
			delete m_face;
		}
		
		bool loadFont(const std::string& fontFile, unsigned int size) {
			FT_Error error = FT_New_Face(*m_library, fontFile.c_str(), 0, m_face);

			if (!error && error == FT_Err_Unknown_File_Format) {
				DBG("FreeType: the font file could be opened and read, but it appears that its font format is unsupported");
				return false;
			}

			if (error) {
				DBG("FreeType: another error code means that the font file could not be opened or read, or that it is broken...");
			}

			FT_Set_Pixel_Sizes(*m_face, 0, size * 64);
		}

		FT_Face* getFace() {
			return m_face;
		}
		
	};


	
}

