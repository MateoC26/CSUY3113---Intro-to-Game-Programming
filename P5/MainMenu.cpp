#include "MainMenu.h"

#define MAINMENU_WIDTH 11
#define MAINMENU_HEIGHT 8

unsigned int mainMenu_data[] =
{
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
   0, 84,  0,  0,  0,  0,  0,  0,  0,  0,  0,
  92, 93, 94,  0,  0,  0,  0,  0,  0,  0,  0,
  92, 93, 94,  0,  0,  0,  0,  0,  0,  0,  0,
  92, 93, 94, 39, 40, 41,  0,  0,  0,  0,  3,
  60, 86, 56, 54, 55, 54,  0, 24,  0,  3, 16,
   3,  3,  3,  3,  3,  3,  3,  3,  3, 16, 16
};

void MainMenu::Initialize() {
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("Snow_Tilesheet.png");
    state.map = new Map(MAINMENU_WIDTH, MAINMENU_HEIGHT, mainMenu_data, mapTextureID, 1.0f, 14, 7);
}

void MainMenu::Update(float deltaTime) {
    return;
}

void MainMenu::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font.png"), "Snowy Mountain Climb", 0.95f, -0.5f, glm::vec3(.65, -0.5f, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press ENTER to play", 0.7, -0.45f, glm::vec3(2.75, -2, 0));

    state.map->Render(program);
}

void MainMenu::FreeMusic()
{
    return;
}