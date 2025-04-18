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

    if (xdist < 0 && ydist < 0)
    {
        return true;
    }

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
                collidedTop = true;
                object->collidedBottom = true;
            }
            else if(velocity.y < 0)
            {
                position.y += penetrationY;
                velocity.y = 0;
                collidedBottom = true;
                object->collidedTop = true;
            }

            lastCollision = object;
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
                collidedRight = true;
                object->collidedLeft = true;
            }
            else if (velocity.x < 0)
            {
                position.x += penetrationX;
                velocity.x = 0;
                collidedLeft = true;
                object->collidedRight = true;
            }
            
            lastCollision = object;
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
        collidedTop = true;
    }
    else if (map->IsSolid(top_left, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }
    else if (map->IsSolid(top_right, &penetration_x, &penetration_y) && velocity.y > 0) {
        position.y -= penetration_y;
        velocity.y = 0;
        collidedTop = true;
    }

    if (map->IsSolid(bottom, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_left, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }
    else if (map->IsSolid(bottom_right, &penetration_x, &penetration_y) && velocity.y < 0) {
        position.y += penetration_y;
        velocity.y = 0;
        collidedBottom = true;
    }

    if (collidedBottom)
    {
        if (!map->IsSolid(bottom_left, &penetration_x, &penetration_y)) {
            holeLeft = true;
        }
        if (!map->IsSolid(bottom_right, &penetration_x, &penetration_y)) {
            holeRight = true;
        }
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
        collidedLeft = true;
    }

    if (map->IsSolid(right, &penetration_x, &penetration_y) && velocity.x > 0) {
        position.x -= penetration_x;
        velocity.x = 0;
        collidedRight = true;
    }
}

void Entity::AIPatroller(Map* map)
{
    switch (aiState)
    {
    case IDLE:
        movement = glm::vec3(-1, 0, 0);
        aiState = WALKING;
        break;

    case WALKING:
        if (holeLeft)
        {
            movement = glm::vec3(1, 0, 0);
            animIndices = animRight;
        }
        else if (collidedRight)
        {
            movement = glm::vec3(-1, 0, 0);
            animIndices = animLeft;
        }
        break;

    case JUMPING:
        break;

    case ATTACKING:
            break;

    case RESTING:
        break;
    }
}

void Entity::AICharger(float deltaTime, Entity* player)
{
    switch (aiState)
    {
    case IDLE:
        if (glm::distance(position, player->position) < 3.0f)
        {
            attackPosition = player->position;
            aiState = ATTACKING;
        }
        break;

    case WALKING:
        break;

    case JUMPING:
        break;

    case ATTACKING:
        if (fabs(position.x - attackPosition.x) < 0.5f)
        {
            movement = glm::vec3(0);
            aiState = RESTING;
        }
        else
        {
            if (attackPosition.x <= position.x)
            {
                movement = glm::vec3(-1, 0, 0);
                animIndices = animLeft;
            }
            else if (attackPosition.x > position.x)
            {
                movement = glm::vec3(1, 0, 0);
                animIndices = animRight;
            }
        }
        break;

    case RESTING:
        restTimeRemaining -= deltaTime;
        
        if (restTimeRemaining <= 0)
        {
            restTimeRemaining = 2;
            aiState = IDLE;
        }
        break;
    }
}

void Entity::AIJumper(Map* map)
{
    switch (aiState)
    {
    case IDLE:
        if (movement.x == 0)
            movement = glm::vec3(1, 0, 0);
        
        jumpPower = 4.0f;
        aiState = JUMPING;
        break;

    case WALKING:
        break;

    case JUMPING:
        if(collidedBottom)
            jump = true;

        if (holeRight && movement.x == 1)
        {
            speed = 1.0f;
            aiState = IDLE;
        }
        else if (holeLeft && movement.x == -1)
        {
            speed = 1.0f;
            jumpPower = 6.5f;
            aiState = IDLE;
        }

        if (collidedLeft)
        {
            movement = glm::vec3(1, 0, 0);
            animIndices = animRight;
        }
        else if (collidedRight)
        {
            movement = glm::vec3(-1, 0, 0);
            animIndices = animLeft;
        }
        break;

    case ATTACKING:
        break;

    case RESTING:
        break;
    }
}

void Entity::AI(float deltaTime, Entity* player, Map* map)
{
    switch (aiType)
    {
    case PATROLLER:
        AIPatroller(map);
        break;

    case CHARGER:
        AICharger(deltaTime, player);
        break;

    case JUMPER:
        AIJumper(map);
        break;
    }
}

void Entity::Update(float deltaTime, Entity* player, Entity* objects, int objectCount, Map* map)
{
    if (isActive == false) return;

    if (entityType == ENEMY)
    {
        AI(deltaTime, player, map);
    }

    collidedTop = false;
    collidedBottom = false;
    collidedLeft = false;
    collidedRight = false;
    holeLeft = false;
    holeRight = false;

    if (animIndices != NULL) {
        if (glm::length(movement) != 0) {
            animTime += deltaTime;

            if (animTime >= 0.25f)
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
            animIndex = 0;
        }
    }

    if (jump)
    {
        jump = false;
        velocity.y += jumpPower;
    }

    velocity.x = movement.x * speed;
    velocity += acceleration * deltaTime;

    position.y += velocity.y * deltaTime;  // Move on Y
    CheckCollisionsY(map);
    CheckCollisionsY(objects, objectCount); // Fix if needed

    position.x += velocity.x * deltaTime;  // Move on X
    CheckCollisionsX(map);
    CheckCollisionsX(objects, objectCount); // Fix if needed

    if (entityType == PLAYER && lastCollision)
    {
        if (collidedBottom && lastCollision->entityType == ENEMY && lastCollision->collidedTop)
        {
            lastCollision->isActive = false;
            lastCollision = NULL;
            velocity.y += 4.0f;
            position.y += velocity.y * deltaTime;
        }
        else if ((collidedLeft || collidedRight) && lastCollision->entityType == ENEMY)
        {
            isActive = false;
        }
    }
    else if (entityType == ENEMY && lastCollision)
    {
         if ((collidedLeft || collidedRight) && lastCollision->entityType == PLAYER)
         {
            lastCollision->isActive = false;
            lastCollision = NULL;
         }
    }

    if (position.y < -8)
        isActive = false;

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