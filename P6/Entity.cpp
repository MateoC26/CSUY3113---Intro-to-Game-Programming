#include "Entity.h"

Entity::Entity()
{
    position = glm::vec3(0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);
    speed = 0;

    modelMatrix = glm::mat4(1.0f);
}

bool Entity::CheckCollision(Entity *other)
{
    if (other == this) return false;

    if (isActive == false || other->isActive == false) return false;

    float xdist = fabs(position.x - other->position.x) - ((width + other->width) / 2.0f);
    float ydist = fabs(position.y - other->position.y) - ((height + other->height) / 2.0f);

    if (xdist < 0 && ydist < 0) return true;

    return false;
}

void Entity::CheckCollisionsY(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float ydist = fabs(position.y - object->position.y);
            float penetrationY = fabs(ydist - (height / 2.0f) - (object->height / 2.0f));
            if (velocity.y > 0)
            {
                position.y -= penetrationY;
                velocity.y = 0;
                collidedTopObj = true;
            }
            else if(velocity.y < 0)
            {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottomObj = true;
            }

            lastCollidedObject = object;
        }
    }
}

void Entity::CheckCollisionsX(Entity* objects, int objectCount)
{
    for (int i = 0; i < objectCount; i++)
    {
        Entity* object = &objects[i];

        if (CheckCollision(object))
        {
            float xdist = fabs(position.x - object->position.x);
            float penetrationX= fabs(xdist - (width / 2.0f) - (object->width / 2.0f));
            if (velocity.x > 0)
            {
                position.x -= penetrationX;
                velocity.x = 0;
                collidedRightObj = true;
            }
            else if (velocity.x < 0)
            {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeftObj = true;
            }

            lastCollidedObject = object;
        }
    }
}

void Entity::CheckCollisionsY(Map* map)
{
    // Probes for tiles
    glm::vec3 top = glm::vec3(position.x, position.y + (height / 2), position.z);
    glm::vec3 top_left = glm::vec3(position.x - (width / 2), position.y + (height / 2), position.z);
    glm::vec3 top_right = glm::vec3(position.x + (width / 2), position.y + (height / 2), position.z);

    glm::vec3 bottom = glm::vec3(position.x, position.y - (height / 2), position.z);
    glm::vec3 bottom_left = glm::vec3(position.x - (width / 2), position.y - (height / 2), position.z);
    glm::vec3 bottom_right = glm::vec3(position.x + (width / 2), position.y - (height / 2), position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    if (map->IsSolid(top, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTopMap = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottomMap = true;
    }
}

void Entity::CheckCollisionsX(Map* map)
{
    // Probes for tiles
    glm::vec3 left = glm::vec3(position.x - (width / 2), position.y, position.z);
    glm::vec3 right = glm::vec3(position.x + (width / 2), position.y, position.z);

    float penetration_x = 0;
    float penetration_y = 0;

    
    if (map->IsSolid(left, &penetration_x, &penetration_y) && velocity.x < 0) {
        position.x += penetration_x;
        velocity.x = 0;
        collidedLeftMap = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRightMap = true;
    }
    
}

void Entity::AIPatroller(Map* map)
{
    switch (aiState)
    {
    case IDLE:
        break;

    case WALKING:
        if (holeLeft || collidedLeftMap || collidedLeftObj)
        {
            movement = glm::vec3(1, 0, 0);
        }
        else if (holeRight || collidedRightMap || collidedRightObj)
        {
            movement = glm::vec3(-1, 0, 0);
        }
        else if (holeTop || collidedTopMap || collidedTopObj)
        {
            movement = glm::vec3(0, -1, 0);
        }
        else if (holeBottom || collidedBottomMap || collidedBottomObj)
        {
            movement = glm::vec3(0, 1, 0);
        }
        break;

    case ATTACKING:
        break;
    }
}

void Entity::AI(Entity* player, Map* map)
{
    switch (aiType)
    {
    case PATROLLER:
        AIPatroller(map);
        break;
    }
}

void Entity::HandleInteractable(Entity* interactable)
{
    switch (interactable->interactableType)
    {
    case KEY:
        interactable->isActive = false;
        keys++;
        break;

    case DOOR:
        if (keys > 0)
        {
            interactable->isActive = false;
            keys--;
        }
        break;

    case CHEST:
        if (keys > 0 && !interactable->hasBeenOpened)
        {
            interactable->hasBeenOpened = true;
            score += 100;
            keys--;
        }
        break;

    case POTION:
        interactable->isActive = false;
        score += 50;
        hasPotion = true;
        lives = 4;
        break;

    case FOOD:
        int maxLives;
        interactable->isActive = false;

        if (hasPotion)
            maxLives = 4;
        else
            maxLives = 3;

        if (lives < maxLives)
            lives++;
        else
            score += 50;

        break;

    case EXIT:
        if (collidedRightObj)
        {
            hasExited = true;
        }
        break;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* enemies, int enemyCount, Entity* items, int itemCount, Map* map)
{
    if (isActive == false) return;

    if (entityType == PLAYER && cooldown > 0)
        cooldown -= deltaTime;

    if (entityType == INTERACTABLE)
    {
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, position);
        return;
    }

    collidedTopMap = false;
    collidedBottomMap = false;
    collidedLeftMap = false;
    collidedRightMap = false;

    collidedTopObj = false;
    collidedBottomObj = false;
    collidedLeftObj = false;
    collidedRightObj = false;

    holeTop = false;
    holeBottom = false;
    holeLeft = false;
    holeRight = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.10f)
            {
                animTime = 0.0f;
                animIndex++;
                if (animIndex >= animFrames)
                {
                    animIndex = 0;
                }
            }
        }
        else {
            animIndices = animIdle;
            animFrames = 1;
            animIndex = 0;
        }
    }

    velocity.x = movement.x * speed;
    velocity.y = movement.y * speed;
    velocity += acceleration * deltaTime;

    // Y
    position.y += velocity.y * deltaTime;  // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(items, itemCount);
    if (lastCollidedObject && lastCollidedObject->entityType == INTERACTABLE && entityType == PLAYER)
    {
        HandleInteractable(lastCollidedObject);
        lastCollidedObject = NULL;
        collidedTopObj = false;
        collidedBottomObj = false;
        collidedLeftObj = false;
        collidedRightObj = false;
    }
    CheckCollisionsY(enemies, enemyCount);
    if (entityType == ENEMY) CheckCollisionsY(player, 1);

    // X
    position.x += velocity.x * deltaTime;  // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(items, itemCount);
    if (lastCollidedObject && lastCollidedObject->entityType == INTERACTABLE && entityType == PLAYER)
    {
        HandleInteractable(lastCollidedObject);
        lastCollidedObject = NULL;
        collidedTopObj = false;
        collidedBottomObj = false;
        collidedLeftObj = false;
        collidedRightObj = false;
    }
    CheckCollisionsX(enemies, enemyCount);
    if (entityType == ENEMY) CheckCollisionsX(player, 1);

    if (lastCollidedObject && lastCollidedObject->entityType == PLAYER)
    {
        if (collidedLeftObj)
        {
            player->collidedRightObj = true;
            lastCollidedObject = NULL;
        }
        else if (collidedRightObj)
        {
            player->collidedLeftObj = true;
            lastCollidedObject = NULL;
        }
        else if (collidedTopObj)
        {
            player->collidedTopObj = true;
            lastCollidedObject = NULL;
        }
        else if (collidedBottomObj)
        {
            player->collidedBottomObj = true;
            lastCollidedObject = NULL;
        }
    }

    if ((player->collidedLeftObj || player->collidedRightObj || player->collidedTopObj || player->collidedBottomObj) && player->cooldown == 0)
    {
        player->lives--;
        player->collidedTopObj = false;
        player->collidedBottomObj = false;
        player->collidedLeftObj = false;
        player->collidedRightObj = false;
        player->cooldown = 5;
    }

    if (entityType == FLAME)
    {
        if (collidedLeftMap || collidedRightMap || collidedTopMap || collidedBottomMap)
        {
            movement = glm::vec3(0);
            position = glm::vec3(0);
            isActive = false;
            shotOut = false;
        }
        else if (lastCollidedObject)
        {
            if (lastCollidedObject->entityType == ENEMY)
            {
                lastCollidedObject->lives--;
                player->score += 50;
            }

            lastCollidedObject = NULL;
            movement = glm::vec3(0);
            position = glm::vec3(0);
            isActive = false;
            shotOut = false;
        }
    }

    if (entityType == ENEMY)
    {
        AI(player, map);
    }

    if (lives <= 0)
    {
        isActive = false;
    }

    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
}

void Entity::DrawSpriteFromTextureAtlas(ShaderProgram* program, GLuint textureID, int index)
{
    float u = (float)(index % animCols) / (float)animCols;
    float v = (float)(index / animCols) / (float)animRows;

    float width = 1.0f / (float)animCols;
    float height = 1.0f / (float)animRows;

    float texCoords[] = { u, v + height, u + width, v + height, u + width, v,
        u, v + height, u + width, v, u, v };

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::Render(ShaderProgram* program) 
{
    if (isActive == false) return;

    program->SetModelMatrix(modelMatrix);

    if (animIndices != NULL) {
        DrawSpriteFromTextureAtlas(program, textureID, animIndices[animIndex]);
        return;
    }

    float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
    float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

    glBindTexture(GL_TEXTURE_2D, textureID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
    glEnableVertexAttribArray(program->texCoordAttribute);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Entity::ResetPlayer()
{
    position = glm::vec3(1, -5, 0);
    movement = glm::vec3(0);
    acceleration = glm::vec3(0);
    velocity = glm::vec3(0);

    collidedTopObj = false;
    collidedBottomObj = false;
    collidedLeftObj = false;
    collidedRightObj = false;

    animIndices = animIdle;
    animFrames = 1;
    animIndex = 0;
}