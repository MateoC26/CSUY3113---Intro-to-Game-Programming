#include "WinScreen.h"

void WinScreen::Initialize() {
    state.nextScene = -1;
}

void WinScreen::Update(float deltaTime) {
    return;
}

void WinScreen::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font.png"), "Game Over", 1.0f, -0.5f, glm::vec3(-2, 3, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "You Win!", 1.0f, -0.5f, glm::vec3(-1.75f, 2, 0));

    Util::DrawText(program, Util::LoadTexture("font.png"), "Press R to Replay", 1.0f, -0.5f, glm::vec3(-4, 0, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press ESC to Exit", 1.0f, -0.5f, glm::vec3(-4, -1, 0));
}