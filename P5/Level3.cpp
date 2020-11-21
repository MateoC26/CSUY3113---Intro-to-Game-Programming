#include "Level3.h"

#define LEVEL3_WIDTH 22
#define LEVEL3_HEIGHT 8

#define LEVEL3_ENEMY_COUNT 3

unsigned int level3_data[] =
{
   0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52,
  84,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52,
  93, 80,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 52,
  93, 80,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0, 58, 52,
  93, 80,  0,  0,  0,  0,  0,  0, 73,  0,  0,  0,  0, 73,  0,  0,  0,  0,  0, 52, 52, 52,
  73,  0,  0,  0,  0,  0, 73,  0, 74, 75, 76, 77, 75, 88,  0,  0,  0,  0, 52, 52, 52, 52,
  74, 75, 76,  0,  0, 73, 74,  0, 88,  0,  0,  0,  0, 74,  0, 52, 52, 52, 52, 52, 52, 52,
  73,  0,  0,  0,  0, 88, 73,  0, 73,  0,  0,  0,  0, 73,  0, 52, 52, 52, 52, 52, 52, 52
};

Mix_Chunk* win;
Mix_Chunk* lose3;

void Level3::Initialize() {
    state.nextScene = -1;

    win = Mix_LoadWAV("Win.wav");
    Mix_VolumeChunk(win, MIX_MAX_VOLUME / 4);

    lose3 = Mix_LoadWAV("Lose.wav");
    Mix_VolumeChunk(lose3, MIX_MAX_VOLUME / 2);

    GLuint mapTextureID = Util::LoadTexture("Snow_Tilesheet.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, level3_data, mapTextureID, 1.0f, 14, 7);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("Robot_Spritesheet.png");

    state.player->animIdle = new int[1]{ 0 };
    state.player->animRight = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.player->animLeft = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.player->animUp = new int[1]{ 1 };

    state.player->animIndices = state.player->animIdle;
    state.player->animFrames = 1;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 9;
    state.player->animRows = 5;

    state.player->height = 1.0f;
    state.player->width = 0.8f;

    state.player->jumpPower = 6.0f;

    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];

    // Enemy 1
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].textureID = Util::LoadTexture("Zombie_Spritesheet.png");
    state.enemies[0].position = glm::vec3(9, -4, 0);
    state.enemies[0].movement = glm::vec3(0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].speed = 1.0f;

    state.enemies[0].animIdle = new int[1]{ 0 };
    state.enemies[0].animRight = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[0].animLeft = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[0].animUp = new int[1]{ 1 };

    state.enemies[0].animIndices = state.enemies[0].animIdle;
    state.enemies[0].animFrames = 1;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 9;
    state.enemies[0].animRows = 5;

    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 0.8f;

    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = IDLE;

    // Enemy 2
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].textureID = Util::LoadTexture("Zombie_Spritesheet.png");
    state.enemies[1].position = glm::vec3(12, -4, 0);
    state.enemies[1].movement = glm::vec3(0);
    state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[1].speed = 1.0f;

    state.enemies[1].animIdle = new int[1]{ 0 };
    state.enemies[1].animRight = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[1].animLeft = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[1].animUp = new int[1]{ 1 };

    state.enemies[1].animIndices = state.enemies[1].animIdle;
    state.enemies[1].animFrames = 1;
    state.enemies[1].animIndex = 0;
    state.enemies[1].animTime = 0;
    state.enemies[1].animCols = 9;
    state.enemies[1].animRows = 5;

    state.enemies[1].height = 1.0f;
    state.enemies[1].width = 0.8f;

    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].aiState = IDLE;

    //Enemy 3
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].textureID = Util::LoadTexture("Zombie_Spritesheet.png");
    state.enemies[2].position = glm::vec3(15, -5, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[2].speed = 1.0f;

    state.enemies[2].animIdle = new int[1]{ 0 };
    state.enemies[2].animRight = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[2].animLeft = new int[8]{ 36, 37, 38, 39, 40, 41, 42, 43 };
    state.enemies[2].animUp = new int[1]{ 1 };

    state.enemies[2].animIndices = state.enemies[2].animIdle;
    state.enemies[2].animFrames = 1;
    state.enemies[2].animIndex = 0;
    state.enemies[2].animTime = 0;
    state.enemies[2].animCols = 9;
    state.enemies[2].animRows = 5;

    state.enemies[2].height = 1.0f;
    state.enemies[2].width = 0.8f;

    state.enemies[2].aiType = PATROLLER;
    state.enemies[2].aiState = IDLE;
}

void Level3::Update(float deltaTime) {
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.map);
    }

    if (state.player->movement.x < 0)
    {

        state.player->modelMatrix = glm::scale(state.player->modelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
    }

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        if (state.enemies[i].movement.x < 0)
        {

            state.enemies[i].modelMatrix = glm::scale(state.enemies[i].modelMatrix, glm::vec3(-1.0f, 1.0f, 1.0f));
        }
    }

    if (state.player->position.x >= 20)
    {
        Mix_PlayChannel(-1, win, 0);
        state.nextScene = 4;
    }
    else if (state.player->lives == 0)
    {
        Mix_PlayChannel(-1, lose3, 0);
        state.nextScene = 4;
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);

    if (state.player->position.x > 5 && state.player->position.x < 16)
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "Lives: " + std::to_string(state.player->lives),
            0.8f, -0.5f, glm::vec3(state.player->position.x - 4.5, -0.5, 0));
    }
    else if (state.player->position.x > 16)
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "Lives: " + std::to_string(state.player->lives),
            0.8f, -0.5f, glm::vec3(11.5, -0.5, 0));
    }
    else
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "Lives: " + std::to_string(state.player->lives),
            0.8f, -0.5f, glm::vec3(0.5, -0.5, 0));
    }

    if (state.player->position.x >= 20)
    {
        Util::DrawText(program, Util::LoadTexture("font.png"), "You Win!", 0.8f, -0.5f, glm::vec3(15, -0.5, 0));
    }
}

void Level3::FreeMusic()
{
    Mix_FreeChunk(win);
    Mix_FreeChunk(lose3);
}