#include "Globals.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "ModuleRender.h"
#include "ModuleUI.h"
#include "ModuleSceneIntro.h"
#include <string.h>

// Reference at https://www.youtube.com/watch?v=OEhmUuehGOA

ModuleUI::ModuleUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
}

ModuleUI::~ModuleUI()
{}

// Load new texture from file path
int ModuleUI::Load(const char* texture_path, const char* characters, uint rows)
{
	int id = -1;

	if (texture_path == nullptr || characters == nullptr || rows == 0)
	{
		LOG("Could not load font");
		return id;
	}

	SDL_Texture* tex = App->textures->Load(texture_path);

	if (tex == nullptr || strlen(characters) >= MAX_FONT_CHARS)
	{
		LOG("Could not load font at %s with characters '%s'", texture_path, characters);
		return id;
	}

	id = 0;
	for (; id < MAX_FONTS; ++id)
	if (fonts[id].graphic == nullptr)
		break;

	if (id == MAX_FONTS)
	{
		LOG("Cannot load font %s. Array is full (max %d).", texture_path, MAX_FONTS);
		return id;
	}

	// if we reach this point we can safely create a new bmp font
	Font* f = &fonts[id];
	f->graphic = tex;
	strcpy_s(fonts[id].table, MAX_FONT_CHARS, characters);
	f->rows = rows;
	f->len = strlen(characters);
	f->row_chars = fonts[id].len / rows;

	uint w, h;
	App->textures->GetSize(tex, w, h);
	f->row_chars = fonts[id].len / rows;
	f->char_w = w / fonts[id].row_chars;
	f->char_h = h / rows;
	
	LOG("Successfully loaded BMP font from %s", texture_path);

	return id;
}

void ModuleUI::UnLoad(int font_id)
{
	if (font_id >= 0 && font_id < MAX_FONTS && fonts[font_id].graphic != nullptr)
	{
		App->textures->Unload(fonts[font_id].graphic);
		fonts[font_id].graphic = nullptr;
		LOG("Successfully Unloaded BMP font_id %d", font_id);
	}
}

void ModuleUI::Blit(int x, int y, int font_id, const char* text) const
{
	if (text == nullptr || font_id < 0 || font_id >= MAX_FONTS || fonts[font_id].graphic == nullptr)
	{
		LOG("Unable to render text with bmp font id %d", font_id);
		return;
	}

	const Font* font = &fonts[font_id];
	SDL_Rect rect;
	uint len = strlen(text);

	rect.w = font->char_w;
	rect.h = font->char_h;

	for (uint i = 0; i < len; ++i)
	{
		for (uint j = 0; j < font->len; ++j)
		{
			if (font->table[j] == text[i])
			{
				rect.x = rect.w * (j % font->row_chars);
				rect.y = rect.h * (j / font->row_chars);
				App->renderer->Blit(font->graphic, x, y, &rect, 0.0f);
				x += rect.w;
				break;
			}
		}
	}
}

// Load assets
bool ModuleUI::Start()
{
	LOG("Loading space scene");
	font_score = App->ui->Load("pinball/NUMBERS2.png", "0123456789", 1);
	return true;
}

// UnLoad assets
bool ModuleUI::CleanUp()
{
	LOG("Unloading ui");

	for (int i = 0; i < MAX_FONTS; i++) {
		this->UnLoad(i);
	}

	return true;
}

// Update: draw background
update_status ModuleUI::Update()
{
	sprintf_s(score_text, 10, "%06i", App->scene_intro->points);
	App->ui->Blit(18, 48, font_score, score_text);
	sprintf_s(score_text, 10, "%06i", App->scene_intro->maxpoints);
	App->ui->Blit(18, 96, font_score, score_text);
	sprintf_s(score_text, 10, "%02i", App->scene_intro->numballs);
	App->ui->Blit(49, 192, font_score, score_text);

	return UPDATE_CONTINUE;
}