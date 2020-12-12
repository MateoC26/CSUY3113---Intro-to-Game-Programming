#include "Level2.h"

#define LEVEL2_WIDTH 21
#define LEVEL2_HEIGHT 8

#define LEVEL2_ENEMY_COUNT 3
#define LEVEL2_INTERACTABLE_COUNT 6

unsigned int level2_data[] =
{
  126, 127, 127, 127, 129,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76, 126, 127, 127, 127, 129,
  130,  15,  15,  50, 130,  61, 106,  61, 106,  61, 106,  61, 106,  61, 106,  61, 130,  15,  97,  15, 130,
  130,  15,  15,  48, 130, 106,  61, 106,  61, 106,  61, 106,  61, 106,  61, 106, 130,  15,  97,  15, 130,
  130,  15,  15,  50, 130,  61,  61,  61,  83,  61,  61,  61,  83,  61,  61,  61, 130,  15,  15,  15, 130,
  132, 127,  15, 127, 135,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60, 132, 127,  15, 127, 135,
   76,  61,  60,  61,  61,  60,  61,  39,  40,  40,  40,  40,  40,  40,  41,  60,  60,  60,  60,  60,  76,
   76,  61,  60,  60,  60,  60,   0,   2,   3,  15, 116, 116, 116, 116, 116,  60,  60,  60,  87,  60,  76,
   76,  76,  76,  76,  76,  76,  14,  15,  17,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76,  76
};

Mix_Chunk* die2;

void Level2::Initialize()
{
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("TileMap.png");
    state.map = new Map(LEVEL2_WIDTH, LEVEL2_HEIGHT, level2_data, mapTextureID, 1.0f, 14, 10);

    die2 = Mix_LoadWAV("Death.wav");
    Mix_VolumeChunk(die2, MIX_MAX_VOLUME / 2);

    // Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("Player.png");
    state.player->position = glm::vec3(1, -2, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->height = 1.0f;
    state.player->width = 1.0f;

    // Enemies
    state.enemies = new Entity[LEVEL2_ENEMY_COUNT];

    // Enemy 1 - Zombie
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].textureID = Util::LoadTexture("ZombieEnemy.png");
    state.enemies[0].position = glm::vec3(14, -4, 0);
    state.enemies[0].movement = glm::vec3(-1, 0, 0);
    state.enemies[0].acceleration = glm::vec3(0);
    state.enemies[0].speed = 2.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 1.0f;
    state.enemies[0].lives = 2;

    // Enemy 2 - Zombie
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].textureID = Util::LoadTexture("ZombieEnemy.png");
    state.enemies[1].position = glm::vec3(15, -4, 0);
    state.enemies[1].movement = glm::vec3(-1, 0, 0);
    state.enemies[1].acceleration = glm::vec3(0);
    state.enemies[1].speed = 2.0f;
    state.enemies[1].height = 1.0f;
    state.enemies[1].width = 1.0f;
    state.enemies[1].lives = 2;

    // Enemy 3 - Slime
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = PATROLLER;
    state.enemies[2].aiState = WALKING;
    state.enemies[2].textureID = Util::LoadTexture("SlimeEnemy.png");
    state.enemies[2].position = glm::vec3(10, -6, 0);
    state.enemies[2].movement = glm::vec3(1, 0, 0);
    state.enemies[2].acceleration = glm::vec3(0);
    state.enemies[2].speed = 2.0f;
    state.enemies[2].height = 1.0f;
    state.enemies[2].width = 1.0f;
    state.enemies[2].lives = 2;

    // Interactables
    state.interactables = new Entity[LEVEL2_INTERACTABLE_COUNT];

    // Interactable 1 - Key
    state.interactables[0].entityType = INTERACTABLE;
    state.interactables[0].interactableType = KEY;
    state.interactables[0].textureID = Util::LoadTexture("Key.png");
    state.interactables[0].position = glm::vec3(9, -6, 0);

    // Interactable 2 - Key
    state.interactables[1].entityType = INTERACTABLE;
    state.interactables[1].interactableType = KEY;
    state.interactables[1].textureID = Util::LoadTexture("Key.png");
    state.interactables[1].position = glm::vec3(17, -1, 0);

    // Interactable 3 - Door
    state.interactables[2].entityType = INTERACTABLE;
    state.interactables[2].interactableType = DOOR;
    state.interactables[2].textureID = Util::LoadTexture("Gate.png");
    state.interactables[2].position = glm::vec3(18, -4, 0);

    // Interactable 4 - Chest
    state.interactables[3].entityType = INTERACTABLE;
    state.interactables[3].interactableType = CHEST;
    state.interactables[3].textureID = Util::LoadTexture("Chest.png");
    state.interactables[3].position = glm::vec3(10, -2, 0);

    // Interactable 5 - Potion
    state.interactables[4].entityType = INTERACTABLE;
    state.interactables[4].interactableType = POTION;
    state.interactables[4].textureID = Util::LoadTexture("Potion.png");
    state.interactables[4].position = glm::vec3(19, -1, 0);

    // Interactable 6 - Exit
    state.interactables[5].entityType = INTERACTABLE;
    state.interactables[5].interactableType = EXIT;
    state.interactables[5].textureID = Util::LoadTexture("Exit.png");
    state.interactables[5].position = glm::vec3(19, -6, 0);

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

void Level2::Update(float deltaTime)
{
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.interactables, LEVEL2_INTERACTABLE_COUNT, state.map);

    if (state.flame->shotOut)
        state.flame->Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.interactables, LEVEL2_INTERACTABLE_COUNT, state.map);

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL2_ENEMY_COUNT, state.interactables, LEVEL2_INTERACTABLE_COUNT, state.map);
    }

    for (int i = 0; i < LEVEL2_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Update(deltaTime, state.player, NULL, NULL, NULL, NULL, state.map);
    }

    if (state.player->lives <= 0)
    {
        Mix_PlayChannel(-1, die2, 0);
        state.nextScene = 4;
    }

    if (state.player->hasExited)
    {
        state.nextScene = 3;
    }
}

void Level2::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < LEVEL2_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Render(program);
    }

    for (int i = 0; i < LEVEL2_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);

    if (state.flame->shotOut)
        state.flame->Render(program);
}

void Level2::FreeMusic()
{
    Mix_FreeChunk(die2);
}