#include "Level3.h"

#define LEVEL3_WIDTH 21
#define LEVEL3_HEIGHT 13

#define LEVEL3_ENEMY_COUNT 10
#define LEVEL3_INTERACTABLE_COUNT 10

unsigned int LEVEL3_data[] =
{
   77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,
   77,  15,  61,  61,  61,  61,  61,  61,  61, 121, 121, 121,  61,  61,  61,  61,  83, 116, 116, 116,  77,
   77,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60, 116,  15, 116,  77,
   77,  61,  61,  61,  61,  61,  61,  61,  61,  61,  60,  61,  61,  61,  61,  61,  83, 116, 116, 116,  77,
   77,  77,  77,  77,  77,  77,  77,  77,   0,   1,  15,   1,   3,  77,  77,  77,  77,  77,  77,  77,  77,
   77, 106, 106,  61,  61,  61,  61,  61,  14,  36,  15,  36,  17,  96,  96,  96,  96,  96, 110, 111,  77,
   77, 106,  15,  60,  60,  60,  60,  60,  15,  15,  15,  15,  15,  60,  60,  60,  60,  60,  60, 125,  77,
   77, 106, 106,  61,  61,  61,  61,  61,  14,  36,  15,  36,  17,  96,  96,  96,  96,  96, 138, 139,  77,
   77,  77,  77,  77,  77,  77,  77,  77,  28,   1,  15,   1,  31,  77,  77,  77,  77,  77,  77,  77,  77,
   77,  15,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  77,
   77,  40,  40,  40,  41,  15,  40,  40,  40,  41,  60,  40,  40,  40,  41,  15,  40,  40,  40,  40,  77,
   77,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  60,  15,  77,
   77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77,  77
};

Mix_Chunk* die3;
Mix_Chunk* win;

void Level3::Initialize()
{
    state.nextScene = -1;

    GLuint mapTextureID = Util::LoadTexture("TileMap.png");
    state.map = new Map(LEVEL3_WIDTH, LEVEL3_HEIGHT, LEVEL3_data, mapTextureID, 1.0f, 14, 10);
    
    die3 = Mix_LoadWAV("Death.wav");
    Mix_VolumeChunk(die3, MIX_MAX_VOLUME / 2);

    win = Mix_LoadWAV("Win.wav");
    Mix_VolumeChunk(win, MIX_MAX_VOLUME / 2);

    // Player
    state.player = new Entity();
    state.player->entityType = PLAYER;
    state.player->textureID = Util::LoadTexture("Player.png");
    state.player->position = glm::vec3(10, -6, 0);
    state.player->movement = glm::vec3(0);
    state.player->acceleration = glm::vec3(0);
    state.player->speed = 2.0f;
    state.player->height = 1.0f;
    state.player->width = 1.0f;

    // Enemies
    state.enemies = new Entity[LEVEL3_ENEMY_COUNT];

    // Enemy 1 - Zombie
    state.enemies[0].entityType = ENEMY;
    state.enemies[0].aiType = PATROLLER;
    state.enemies[0].aiState = WALKING;
    state.enemies[0].textureID = Util::LoadTexture("ZombieEnemy.png");
    state.enemies[0].position = glm::vec3(4, -5, 0);
    state.enemies[0].movement = glm::vec3(0, -1, 0);
    state.enemies[0].acceleration = glm::vec3(0);
    state.enemies[0].speed = 3.0f;
    state.enemies[0].height = 1.0f;
    state.enemies[0].width = 1.0f;
    state.enemies[0].lives = 3;

    // Enemy 2 - Zombie
    state.enemies[1].entityType = ENEMY;
    state.enemies[1].aiType = PATROLLER;
    state.enemies[1].aiState = WALKING;
    state.enemies[1].textureID = Util::LoadTexture("ZombieEnemy.png");
    state.enemies[1].position = glm::vec3(5, -5, 0);
    state.enemies[1].movement = glm::vec3(0, -1, 0);
    state.enemies[1].acceleration = glm::vec3(0);
    state.enemies[1].speed = 3.0f;
    state.enemies[1].height = 1.0f;
    state.enemies[1].width = 1.0f;
    state.enemies[1].lives = 3;

    // Enemy 3 - Squid
    state.enemies[2].entityType = ENEMY;
    state.enemies[2].aiType = PATROLLER;
    state.enemies[2].aiState = WALKING;
    state.enemies[2].textureID = Util::LoadTexture("SquidEnemy.png");
    state.enemies[2].position = glm::vec3(15, -5, 0);
    state.enemies[2].movement = glm::vec3(0, -1, 0);
    state.enemies[2].acceleration = glm::vec3(0);
    state.enemies[2].speed = 3.0f;
    state.enemies[2].height = 1.0f;
    state.enemies[2].width = 1.0f;
    state.enemies[2].lives = 3;

    // Enemy 4 - Squid
    state.enemies[3].entityType = ENEMY;
    state.enemies[3].aiType = PATROLLER;
    state.enemies[3].aiState = WALKING;
    state.enemies[3].textureID = Util::LoadTexture("SquidEnemy.png");
    state.enemies[3].position = glm::vec3(16, -5, 0);
    state.enemies[3].movement = glm::vec3(0, -1, 0);
    state.enemies[3].acceleration = glm::vec3(0);
    state.enemies[3].speed = 3.0f;
    state.enemies[3].height = 1.0f;
    state.enemies[3].width = 1.0f;
    state.enemies[3].lives = 3;

    // Enemy 5 - Slime
    state.enemies[4].entityType = ENEMY;
    state.enemies[4].aiType = PATROLLER;
    state.enemies[4].aiState = WALKING;
    state.enemies[4].textureID = Util::LoadTexture("SlimeEnemy.png");
    state.enemies[4].position = glm::vec3(20, -9, 0);
    state.enemies[4].movement = glm::vec3(-1, 0, 0);
    state.enemies[4].acceleration = glm::vec3(0);
    state.enemies[4].speed = 3.0f;
    state.enemies[4].height = 1.0f;
    state.enemies[4].width = 1.0f;
    state.enemies[4].lives = 3;

    // Enemy 6 - Slime
    state.enemies[5].entityType = ENEMY;
    state.enemies[5].aiType = PATROLLER;
    state.enemies[5].aiState = WALKING;
    state.enemies[5].textureID = Util::LoadTexture("SlimeEnemy.png");
    state.enemies[5].position = glm::vec3(1, -11, 0);
    state.enemies[5].movement = glm::vec3(1, 0, 0);
    state.enemies[5].acceleration = glm::vec3(0);
    state.enemies[5].speed = 3.0f;
    state.enemies[5].height = 1.0f;
    state.enemies[5].width = 1.0f;
    state.enemies[5].lives = 3;

    // Enemy 7 - Hooded Figure
    state.enemies[6].entityType = ENEMY;
    state.enemies[6].aiType = PATROLLER;
    state.enemies[6].aiState = WALKING;
    state.enemies[6].textureID = Util::LoadTexture("HoodedEnemy.png");
    state.enemies[6].position = glm::vec3(8, -1, 0);
    state.enemies[6].movement = glm::vec3(0, -1, 0);
    state.enemies[6].acceleration = glm::vec3(0);
    state.enemies[6].speed = 3.0f;
    state.enemies[6].height = 1.0f;
    state.enemies[6].width = 1.0f;
    state.enemies[6].lives = 3;

    // Enemy 8 - Hooded Figure
    state.enemies[7].entityType = ENEMY;
    state.enemies[7].aiType = PATROLLER;
    state.enemies[7].aiState = WALKING;
    state.enemies[7].textureID = Util::LoadTexture("HoodedEnemy.png");
    state.enemies[7].position = glm::vec3(12, -1, 0);
    state.enemies[7].movement = glm::vec3(0, -1, 0);
    state.enemies[7].acceleration = glm::vec3(0);
    state.enemies[7].speed = 3.0f;
    state.enemies[7].height = 1.0f;
    state.enemies[7].width = 1.0f;
    state.enemies[7].lives = 3;

    // Enemy 9 - Golem
    state.enemies[8].entityType = ENEMY;
    state.enemies[8].aiType = PATROLLER;
    state.enemies[8].aiState = WALKING;
    state.enemies[8].textureID = Util::LoadTexture("Golem.png");
    state.enemies[8].position = glm::vec3(1, -2, 0);
    state.enemies[8].movement = glm::vec3(1, 0, 0);
    state.enemies[8].acceleration = glm::vec3(0);
    state.enemies[8].speed = 3.0f;
    state.enemies[8].height = 1.0f;
    state.enemies[8].width = 1.0f;
    state.enemies[8].lives = 3;

    // Enemy 10 - Golem
    state.enemies[9].entityType = ENEMY;
    state.enemies[9].aiType = PATROLLER;
    state.enemies[9].aiState = WALKING;
    state.enemies[9].textureID = Util::LoadTexture("Golem.png");
    state.enemies[9].position = glm::vec3(17, -2, 0);
    state.enemies[9].movement = glm::vec3(-1, 0, 0);
    state.enemies[9].acceleration = glm::vec3(0);
    state.enemies[9].speed = 3.0f;
    state.enemies[9].height = 1.0f;
    state.enemies[9].width = 1.0f;
    state.enemies[9].lives = 3;

    // Interactables
    state.interactables = new Entity[LEVEL3_INTERACTABLE_COUNT];

    // Interactable 1 - Key
    state.interactables[0].entityType = INTERACTABLE;
    state.interactables[0].interactableType = KEY;
    state.interactables[0].textureID = Util::LoadTexture("Key.png");
    state.interactables[0].position = glm::vec3(2, -6, 0);

    // Interactable 2 - Key
    state.interactables[1].entityType = INTERACTABLE;
    state.interactables[1].interactableType = KEY;
    state.interactables[1].textureID = Util::LoadTexture("Key.png");
    state.interactables[1].position = glm::vec3(18, -6, 0);

    // Interactable 3 - Key
    state.interactables[2].entityType = INTERACTABLE;
    state.interactables[2].interactableType = KEY;
    state.interactables[2].textureID = Util::LoadTexture("Key.png");
    state.interactables[2].position = glm::vec3(19, -11, 0);

    // Interactable 4 - Key
    state.interactables[3].entityType = INTERACTABLE;
    state.interactables[3].interactableType = KEY;
    state.interactables[3].textureID = Util::LoadTexture("Key.png");
    state.interactables[3].position = glm::vec3(1, -9, 0);

    // Interactable 5 - Door
    state.interactables[4].entityType = INTERACTABLE;
    state.interactables[4].interactableType = DOOR;
    state.interactables[4].textureID = Util::LoadTexture("Gate.png");
    state.interactables[4].position = glm::vec3(10, -4, 0);

    // Interactable 6 - Door
    state.interactables[5].entityType = INTERACTABLE;
    state.interactables[5].interactableType = DOOR;
    state.interactables[5].textureID = Util::LoadTexture("Gate.png");
    state.interactables[5].position = glm::vec3(10, -8, 0);

    // Interactable 7 - Chest
    state.interactables[6].entityType = INTERACTABLE;
    state.interactables[6].interactableType = CHEST;
    state.interactables[6].textureID = Util::LoadTexture("Chest.png");
    state.interactables[6].position = glm::vec3(1, -1, 0);
    
    // Iteractable 8 - Chest
    state.interactables[7].entityType = INTERACTABLE;
    state.interactables[7].interactableType = CHEST;
    state.interactables[7].textureID = Util::LoadTexture("Chest.png");
    state.interactables[7].position = glm::vec3(5, -10, 0);

    // Interactable 9 - Food
    state.interactables[8].entityType = INTERACTABLE;
    state.interactables[8].interactableType = FOOD;
    state.interactables[8].textureID = Util::LoadTexture("Food.png");
    state.interactables[8].position = glm::vec3(15, -10, 0);

    // Interactable 10 - Exit
    state.interactables[9].entityType = INTERACTABLE;
    state.interactables[9].interactableType = EXIT;
    state.interactables[9].textureID = Util::LoadTexture("Portal.png");
    state.interactables[9].position = glm::vec3(18, -2, 0);

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

void Level3::Update(float deltaTime)
{
    state.player->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.interactables, LEVEL3_INTERACTABLE_COUNT, state.map);

    if (state.flame->shotOut)
        state.flame->Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.interactables, LEVEL3_INTERACTABLE_COUNT, state.map);

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Update(deltaTime, state.player, state.enemies, LEVEL3_ENEMY_COUNT, state.interactables, LEVEL3_INTERACTABLE_COUNT, state.map);
    }

    for (int i = 0; i < LEVEL3_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Update(deltaTime, state.player, NULL, NULL, NULL, NULL, state.map);
    }

    if (state.player->lives <= 0)
    {
        Mix_PlayChannel(-1, die3, 0);
        state.nextScene = 4;
    }

    if (state.player->hasExited)
    {
        Mix_PlayChannel(-1, win, 0);
        state.nextScene = 4;
    }
}

void Level3::Render(ShaderProgram* program) {
    state.map->Render(program);

    for (int i = 0; i < LEVEL3_INTERACTABLE_COUNT; i++)
    {
        state.interactables[i].Render(program);
    }

    for (int i = 0; i < LEVEL3_ENEMY_COUNT; i++)
    {
        state.enemies[i].Render(program);
    }

    state.player->Render(program);

    if (state.flame->shotOut)
        state.flame->Render(program);
}

void Level3::FreeMusic()
{
    Mix_FreeChunk(die3);
    Mix_FreeChunk(win);
}