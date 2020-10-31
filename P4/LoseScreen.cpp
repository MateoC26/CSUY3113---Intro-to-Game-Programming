#include "LoseScreen.h"

void LoseScreen::Initialize() {
    state.nextScene = -1;
    return;
}

void LoseScreen::Update(float deltaTime) {
    return;
}

void LoseScreen::Render(ShaderProgram* program) {
    Util::DrawText(program, Util::LoadTexture("font.png"), "Game Over", 1.0f, -0.5f, glm::vec3(-2, 3, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "You Lose!", 1.0f, -0.5f, glm::vec3(-2, 2, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press R to Replay", 1.0f, -0.5f, glm::vec3(-4, 0, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press ESC to Exit", 1.0f, -0.5f, glm::vec3(-4, -1, 0));
}