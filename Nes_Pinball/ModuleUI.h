#ifndef __MODULEUUI_H__
#define __MODULEUUI_H__

#include "Module.h"
#include "Globals.h"

struct SDL_Texture;
#include "SDL\include\SDL_pixels.h"

#define MAX_FONTS 9
#define MAX_FONT_CHARS 256

struct Font
{
	char table[MAX_FONT_CHARS];
	SDL_Texture* graphic = nullptr;
	uint rows, len, char_w, char_h, row_chars;
};
class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status Update();
	bool CleanUp();

	// Load Font
	int Load(const char* texture_path, const char* characters, uint rows = 1);
	void UnLoad(int font_id);

	// Create a surface from text
	void Blit(int x, int y, int bmp_font_id, const char* text) const;
	
	unsigned int lastTime = 0;

public:
	int font_score = -1;
	char score_text[10];
	uint score = 0;
	bool firstimeblit = true;
	SDL_Texture* background = nullptr;
	SDL_Texture* stars = nullptr;
	
private:
	Font	 fonts[MAX_FONTS];
};

#endif // __MODULEUI__