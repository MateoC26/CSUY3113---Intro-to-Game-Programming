#include "Level1.h"

#define LEVEL1_WIDTH 21
#define LEVEL1_HEIGHT 8

#define LEVEL1_ENEMY_COUNT 2
#define LEVEL1_INTERACTABLE_COUNT 5

unsigned int level1_data[] =
{
   74,  74,  74,  74,   0,   1,   1,   1,   1,   1,   1,   1,   1,   1,   1,   0,   1,   1,   3,  74,  74,
   74,  61,  15,  61,  14,  15,  15,  50,  49,  50,  15,  15,  15,  15,  15,  14,  87,  15,  17,  15,  74,
   74,  61,  60,  61,  14,  15,  15,  15,  15,  15,  15,  15,  15,  15,  15,  14,  15,  15,  17,  60,  74,
   74,  61,  60,  61,  28,  15,   1,  14,  15,  15,  15,  15,  15,  15,  15,  14,  15,  15,  17,  60,  74,
   74,  61,  60,  60,  60,  60,  61,  56,   1,  15,   1,   3,  15,  15,  15,  14,  15,  15,  17,  60,  74,
   74, 109, 110, 111,  61,  61,  61,  14,  15,  15,  15,  17,   1,  15,   1,   1,   1,  15,  31,  60,  74,
   74, 123, 124, 125,  61,  61,  61,  14,  36,  15,  36,  17,  83,  60,  60,  60,  60,  60,  60,  60,  74,
   74, 137, 138, 139,  74,  74,  74,  28,   1,   1,   1,  31,  74,  74,  74,  74,  74,  74,  74,  74,  74
};

Mix_Chunk* die1;

void Level1::Initialize() 
{
    state.nextScene = -1;

    die1 = Mix_LoadWAV("Death.wav");
    Mix_VolumeChunk(die1, MIX_MAX_VOLUME);

    GLuint mapTextureID = Util::LoadTexture("TileMap.png");
    state.map = new Map(LEVEL1_WIDTH, LEVEL1_HEIGHT, level1_data, mapTextureID, 1.0f, 14, 10);

    // Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("Player.png");
    state.player->position = glm::vec3(9, -5.5, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->height = 1.0f;
    state.player->width = 1.0f;
    state.player->hasPotion = false;
    state.player->lives = 3;

    // Enemies
    state.enemies = new Entity[LEVEL1_ENEMY_COUNT];

    // Enemy 1 - Hooded Figure
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].textureID = Util::LoadTexture("HoodedEnemy.png");
    state.enemies[0].position = glm::vec3(19, -2, 0);
    state.enemies[0].movement = glm::vec3(0, -1, 0);
    state.enemies[0].acceleration = glm::vec3(0);
    state.enemies[0].speed = 2.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 1.0f;

    // Enemy 2 - Flying Squid
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].textureID = Util::LoadTexture("SquidEnemy.png");
    state.enemies[1].position = glm::vec3(2, -2, 0);
    state.enemies[1].movement = glm::vec3(1, 0, 0);
    state.enemies[1].acceleration = glm::vec3(0);
    state.enemies[1].speed = 2.0f;
    state.enemies[1].height = 1.0f;
    state.enemies[1].width = 1.0f;

    // Interactables
    state.interactables = new Entity[LEVEL1_INTERACTABLE_COUNT];

    // Interactable 1 - Key
    state.interactables[0].entityType = INTERACTABLE;
    state.interactables[0].interactableType = KEY;
    state.interactables[0].textureID = Util::LoadTexture("Key.png");
    state.interactables[0].position = glm::vec3(19, -1, 0);

    // Interactable 2 - Key
    state.interactables[1].entityType = INTERACTABLE;
    state.interactables[1].interactableType = KEY;
    state.interactables[1].textureID = Util::LoadTexture("Key.png");
    state.interactables[1].position = glm::vec3(2, -1, 0);

    // Interactable 3 - Door
    state.interactables[2].entityType = INTERACTABLE;
    state.interactables[2].interactableType = DOOR;
    state.interactables[2].textureID = Util::LoadTexture("Door.png");
    state.interactables[2].position = glm::vec3(17, -5, 0);

    // Interactable 4 - Chest
    state.interactables[3].entityType = INTERACTABLE;
    state.interactables[3].interactableType = CHEST;
    state.interactables[3].textureID = Util::LoadTexture("Chest.png");
    state.interactables[3].position = glm::vec3(13, -1, 0);

    // Interactable 5 - Exit
    state.interactables[4].entityType = INTERACTABLE;
    state.interactables[4].interactableType = EXIT;
    state.interactables[4].textureID = Util::LoadTexture("Exit.png");
    state.interactables[4].position = glm::vec3(17, -1, 0);

   // state.flame
    state.flame = new Entity();
    state.flame->entityType = FLAME;
    state.flame->textureID = Util::LoadTexture("Flame.png");
    state.flame->position = glm::vec3(0);
    state.flame->movement = glm::vec3(0);
    state.flame->acceleration = glm::vec3(0);
    state.flame->speed = 3.0f;
    state.flame->height = 0.75f;
    state.flame->width = 0.75f; 
    state.flame->shotOut = false;
    state.flame->isActive = false;
}

void Level1::Update(float deltaTime)
{
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.interactables, LEVEL1_INTERACTABLE_COUNT, state.map);

    if(state.flame->shotOut)
        state.flame->Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.interactables, LEVEL1_INTERACTABLE_COUNT, state.map);

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL1_ENEMY_COUNT, state.interactables, LEVEL1_INTERACTABLE_COUNT, state.map);
    }

    for (int i = 0; i < LEVEL1_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Update(deltaTime, state.player, NULL, NULL, NULL, NULL, state.map);
    }

    if (state.player->lives <= 0)
    {
        Mix_PlayChannel(-1, die1, 0);
        state.nextScene = 4;
    }

    if (state.player->hasExited)
    {
        state.nextScene = 2;
    }
}

void Level1::Render(ShaderProgram* program) {
	state.map->Render(program);

    for (int i = 0; i < LEVEL1_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Render(program);
    }

    for (int i = 0; i < LEVEL1_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);

    if (state.flame->shotOut)
        state.flame->Render(program);
}

void Level1::FreeMusic()
{
    Mix_FreeChunk(die1);
}