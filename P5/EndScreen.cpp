#include "EndScreen.h"

void EndScreen::Initialize() {
    state.nextScene = -1;

    state.player = new Entity();
    state.player->isActive = false;
}

void EndScreen::Update(float deltaTime) {
    return;
}

void EndScreen::Render(ShaderProgram* program) {
    if (state.player->lives >= 1)
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "You Reached the Cure!", 0.95f, -0.5f, glm::vec3(.65, -0.5f, 0));
        Util::DrawText(program, Util::LoadTexture("font.png"), "Humanity is Saved!", 0.95f, -0.5f, glm::vec3(1.25, -1.5f, 0));
    }
    else 
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "You Died!", 0.95f, -0.5f, glm::vec3(3, -0.5f, 0));
        Util::DrawText(program, Util::LoadTexture("font.png"), "Humanity is Doomed!", 0.95f, -0.5f, glm::vec3(1, -1.5f, 0));
    }

    Util::DrawText(program, Util::LoadTexture("font.png"), "Press R to Play Again", 0.7, -0.45f, glm::vec3(2.5, -3, 0));
    Util::DrawText(program, Util::LoadTexture("font.png"), "Press ESC to Exit", 0.7, -0.45f, glm::vec3(3, -4, 0));
}

void EndScreen::FreeMusic()
{
    return;
}