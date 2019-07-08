#include "Font.hpp"

Glyph::Glyph(){
	adv_x = adv_y = bitmap_w = bitmap_h = bitmap_l = bitmap_t = texture_offset = 0.0f;
};

Glyph::~Glyph(){
}


FT_Library Font::ft;

Font::Font(std::string path, int size) {
	setlocale( LC_ALL, "" );

	this->path = path;
	this->size = size;

	if(FT_Init_FreeType(&ft)) {
		std::cout<<"Could not init freetype library\n"<<std::endl;
	}

	if(FT_New_Face(ft, path.c_str(), 0, &face)) {
		std::cout<<"Could not open font\n"<<std::endl;
	}

	FT_Select_Charmap(face , ft_encoding_unicode);
	FT_Set_Pixel_Sizes(face, 0, size);

	for(int i = 32; i < 128; i++) {
		if(FT_Load_Char(face, i, FT_LOAD_RENDER)) {
			fprintf(stderr, "Loading character %c failed!\n", i);
		    continue;
		 }
		 g = face->glyph;
		 atlas_width += g->bitmap.width+2;
		 atlas_height = std::max(atlas_height, g->bitmap.rows);
	}

	//Create atlas texture
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlas_width, atlas_height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

	//"Load texture"
	int x = 0;
	glyphs_info.reserve(128);
	for(int i = 32; i < 128; i++) {
		unsigned long c = FT_Get_Char_Index(face, i);
		FT_Load_Glyph(face,c,FT_LOAD_RENDER);
		FT_Render_Glyph( face->glyph, FT_RENDER_MODE_NORMAL );
		g = face->glyph;

		Glyph g_temp;
		g_temp.adv_x = g->advance.x >> 6;
		g_temp.adv_y = g->advance.y >> 6;
		g_temp.bitmap_w = g->bitmap.width;
		g_temp.bitmap_h = g->bitmap.rows;
		g_temp.bitmap_l = g->bitmap_left;
		g_temp.bitmap_t = g->bitmap_top;
		g_temp.texture_offset = (float)x / atlas_width;
		glyphs_info[i] = g_temp;

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		x += g->bitmap.width+2;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	tex = textureID;

}


Font::~Font() {
	glDeleteTextures(1, &tex);
	std::cout<<"Font removed"<<std::endl;
}

