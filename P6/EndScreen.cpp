#include "EndScreen.h"

GLuint fontTextID;

void EndScreen::Initialize() {
    state.nextScene = -1;

    state.player = new Entity();
    state.player->isActive = false;

    fontTextID = Util::LoadTexture("font.png");
}

void EndScreen::Update(float deltaTime) {
    return;
}

void EndScreen::Render(ShaderProgram* program) {
    if (state.player->lives >= 1)
    {
        Util::DrawText(program, fontTextID, "You Reached the Exit!", 0.95f, -0.5f, glm::vec3(0.5, -0.5f, 0));
        Util::DrawText(program, fontTextID, "Good Job!", 0.95f, -0.5f, glm::vec3(3.25, -1.5f, 0));
    }
    else 
    {
        Util::DrawText(program, fontTextID, "You Ran Out of Lives!", 0.95f, -0.5f, glm::vec3(0.5, -0.5f, 0));
        Util::DrawText(program, fontTextID, "Your Life has Ended!", 0.95f, -0.5f, glm::vec3(0.75, -1.5f, 0));
    }

    Util::DrawText(program, fontTextID, "Score: " + std::to_string(state.player->score), 0.7, -0.45f, glm::vec3(4, -3, 0));
    Util::DrawText(program, fontTextID, "Press R to Play Again", 0.7, -0.45f, glm::vec3(2.5, -5, 0));
    Util::DrawText(program, fontTextID, "Press ESC to Exit", 0.7, -0.45f, glm::vec3(3, -6, 0));
}

void EndScreen::FreeMusic()
{
    return;
}