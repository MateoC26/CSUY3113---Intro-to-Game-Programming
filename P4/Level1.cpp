#include "Level1.h"

#define LEVEL1_WIDTH 14
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 3

unsigned int level1_data[] =
{
 247, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 247,
 247, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 247,
 247, 104, 105, 105, 106, 000, 000, 000, 000, 000, 000, 000, 000, 247,
 247, 000, 000, 000, 000, 000, 104, 106, 000, 000, 000, 000, 000, 247,
 247, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 000, 247,
 247, 000, 000, 000, 000, 000, 000, 000, 000, 275, 276, 276, 277, 247,
 247, 273, 160, 000, 162, 270, 271, 271, 272, 295, 296, 296, 297, 247,
 247, 293, 000, 000, 000, 290, 291, 291, 292, 295, 296, 296, 297, 247
};

void Level1::Initialize() {
    state.nextScene = -1;

	GLuint mapTextureID = Util::LoadTexture("monochrome_tilemap_packed.png");
	state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 20, 20);

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->position = glm::vec3(1, -5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0, -9.81f, 0);
    state.player->speed = 2.0f;
    state.player->textureID = Util::LoadTexture("george_0.png");

    state.player->animRight = new int[4]{ 3, 7, 11, 15 };
    state.player->animLeft = new int[4]{ 1, 5, 9, 13 };
    state.player->animUp = new int[4]{ 2, 6, 10, 14 };
    state.player->animDown = new int[4]{ 0, 4, 8, 12 };

    state.player->animIndices = state.player->animRight;
    state.player->animFrames = 4;
    state.player->animIndex = 0;
    state.player->animTime = 0;
    state.player->animCols = 4;
    state.player->animRows = 4;

    state.player->height = 0.8f;
    state.player->width = 0.5f;

    state.player->jumpPower = 6.5f;

    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];
    
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].position = glm::vec3(8, -5, 0);
    state.enemies[0].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[0].speed = 1.0f;
    state.enemies[0].textureID = Util::LoadTexture("billy.png");

    state.enemies[0].animRight = new int[4]{ 12, 13, 14, 15 };
    state.enemies[0].animLeft = new int[4]{ 8, 9, 10, 11 };
    state.enemies[0].animUp = new int[4]{ 4, 5, 6, 7 };
    state.enemies[0].animDown = new int[4]{ 0, 1, 2, 3 };

    state.enemies[0].animIndices = state.enemies[0].animLeft;
    state.enemies[0].animFrames = 4;
    state.enemies[0].animIndex = 0;
    state.enemies[0].animTime = 0;
    state.enemies[0].animCols = 4;
    state.enemies[0].animRows = 4;

    state.enemies[0].height = 0.8f;
    state.enemies[0].width = 0.5f;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = IDLE;

    state.enemies[1].entityType = ENEMY;
    state.enemies[1].position = glm::vec3(12, -4, 0);
    state.enemies[1].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[1].speed = 4.0f;
    state.enemies[1].textureID = Util::LoadTexture("billy.png");

    state.enemies[1].animRight = new int[4]{ 12, 13, 14, 15 };
    state.enemies[1].animLeft = new int[4]{ 8, 9, 10, 11 };
    state.enemies[1].animUp = new int[4]{ 4, 5, 6, 7 };
    state.enemies[1].animDown = new int[4]{ 0, 1, 2, 3 };

    state.enemies[1].animIndices = state.enemies[1].animLeft;
    state.enemies[1].animFrames = 4;
    state.enemies[1].animIndex = 0;
    state.enemies[1].animTime = 0;
    state.enemies[1].animCols = 4;
    state.enemies[1].animRows = 4;

    state.enemies[1].height = 0.8f;
    state.enemies[1].width = 0.5f;
    state.enemies[1].aiType = CHARGER;
    state.enemies[1].aiState = IDLE;

    state.enemies[2].entityType = ENEMY;
    state.enemies[2].position = glm::vec3(2, 5, 0);
    state.enemies[2].movement = glm::vec3(0);
    state.enemies[2].acceleration = glm::vec3(0, -9.81f, 0);
    state.enemies[2].speed = 0.5f;
    state.enemies[2].textureID = Util::LoadTexture("billy.png");

    state.enemies[2].animRight = new int[4]{ 12, 13, 14, 15 };
    state.enemies[2].animLeft = new int[4]{ 8, 9, 10, 11 };
    state.enemies[2].animUp = new int[4]{ 4, 5, 6, 7 };
    state.enemies[2].animDown = new int[4]{ 0, 1, 2, 3 };

    state.enemies[2].animIndices = state.enemies[2].animRight;
    state.enemies[2].animFrames = 4;
    state.enemies[2].animIndex = 0;
    state.enemies[2].animTime = 0;
    state.enemies[2].animCols = 4;
    state.enemies[2].animRows = 4;

    state.enemies[2].height = 0.8f;
    state.enemies[2].width = 0.5f;
    state.enemies[2].aiType = JUMPER;
    state.enemies[2].aiState = IDLE;
}

void Level1::Update(float deltaTime) 
{
	state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.map);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.player, 1, state.map);
    }

    state.enemies[0].isActive = false;
    state.enemies[2].isActive = false;

    if (!state.enemies[0].isActive && !state.enemies[1].isActive && !state.enemies[2].isActive)
    {
        state.nextScene = 1;
    }

    if (!state.player->isActive)
    {
        state.nextScene = 2;
    }
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);
    state.player->Render(program);
    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }
}
