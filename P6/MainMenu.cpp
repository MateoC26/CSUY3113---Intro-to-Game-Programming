#include "MainMenu.h"

#define MAINMENU_WIDTH 11
#define MAINMENU_HEIGHT 8

unsigned int mainMenu_data[] =
{
   75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75,
   75,  60,  60,  60,  60,  60,  60,  60,  60,  60,  75,
   75,  56,   1,   1,   1,   1,   1,   1,   1,   3,  75,
   75,  14,  15,  15,  15,  15,  15,  15,  15,  17,  75,
   75,  28,   1,   1,   1,   1,   1,   1,   1,  31,  75,
   75,  61,  61,  15,  15,  15,  15,  15,  61,  61,  75,
   75,  61,  61,  61,  61,  61,  61,  61,  61,  61,  75,
   75,  75,  75,  75,  75,  75,  75,  75,  75,  75,  75
};  

GLuint fontTexID;

void MainMenu::Initialize() {
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("TileMap.png");
    state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 14, 10);

    fontTexID = Util::LoadTexture("font.png");
}

void MainMenu::Update(float deltaTime) {
    return;
}

void MainMenu::Render(ShaderProgram* program) {
    state.map->Render(program);

    Util::DrawText(program, fontTexID, "Dungeon Delver", 0.95f, -0.5f, glm::vec3(2.25, -3, 0));
    Util::DrawText(program, fontTexID, "Press ENTER to play", 0.7, -0.45f, glm::vec3(2.75, -5, 0));
}

void MainMenu::FreeMusic()
{
    return;
}