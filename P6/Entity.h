#pragma once
#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include "Map.h"

enum EntityType { PLAYER, ENEMY, INTERACTABLE, FLAME };

enum AIType { PATROLLER };
enum AIState { IDLE, WALKING, ATTACKING };

enum InteractableType { KEY, DOOR, CHEST, POTION, FOOD, EXIT };

class Entity {
public:
    EntityType entityType;
    AIType aiType;
    AIState aiState;
    InteractableType interactableType;

    glm::vec3 position;
    glm::vec3 movement;
    glm::vec3 acceleration;
    glm::vec3 velocity;
    
    float width = 1;
    float height = 1;

    float speed;

    int keys = 0;
    int lives = 1;
    int score = 0;
    int cooldown = 0;

    bool shotOut;

    bool hasPotion;
    bool hasExited;

    bool hasBeenOpened = false;

    GLuint textureID;

    glm::mat4 modelMatrix;

    int* animIdle = NULL;
    int* animRight = NULL;
    int* animLeft = NULL;
    int* animUp = NULL;
    int* animDown = NULL;

    int* animIndices = NULL;
    int animFrames = 0;
    int animIndex = 0;
    float animTime = 0;
    int animCols = 0;
    int animRows = 0;

    bool isActive = true;

    bool collidedTopMap = false;
    bool collidedBottomMap = false;
    bool collidedLeftMap = false;
    bool collidedRightMap = false;

    Entity* lastCollidedObject = NULL;

    bool collidedTopObj = false;
    bool collidedBottomObj = false;
    bool collidedLeftObj = false;
    bool collidedRightObj = false;

    bool holeTop = false;
    bool holeBottom = false;
    bool holeLeft = false;
    bool holeRight = false;

    Entity();

    bool CheckCollision(Entity* other);
    void CheckCollisionsY(Entity* objects, int objectCount);
    void CheckCollisionsX(Entity* objects, int objectCount);
    void CheckCollisionsX(Map* map);
    void CheckCollisionsY(Map* map);

    void Update(float deltaTime, Entity* player, Entity* enemies, int enemyCount, Entity* items, int itemCount, Map *map);
    void Render(ShaderProgram* program);
    void DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index);

    void AI(Entity* player, Map* map);
    void AIPatroller(Map* map);

    void HandleInteractable (Entity* interactable);

    void ResetPlayer();
};