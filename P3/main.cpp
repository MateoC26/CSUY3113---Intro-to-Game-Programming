/*
* Project 3 - Lunar Lander
* Mateo Castro - mc7212
* Textures Credit: 1-Bit Platformer Pack from Kenney (www.kenney.nl)
*/

#define GL_SILENCE_DEPRECATION

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_mixer.h>
#include "glm/mat4x4.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Entity.h"
#include "vector"

#define PLATFORM_COUNT 28
#define GOAL_COUNT 4

enum class GameMode { HOW_TO_PLAY, GAME_LEVEL, GAME_OVER };

struct GameState {
    Entity* player;
    Entity* platforms;
    Entity* goals;
};

GameMode mode;
GameState state;

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameOver = false;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

GLuint fontTextureID;

Mix_Music* music;
Mix_Chunk* win;
Mix_Chunk* lose;

GLuint LoadTexture(const char* filePath) {
    int w, h, n;
    unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

    if (image == NULL) {
        std::cout << "Unable to load image. Make sure the path is correct\n";
        assert(false);
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);
    return textureID;
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Project 3 - Lunar Lander", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Music.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2);

    win = Mix_LoadWAV("Win.wav");
    Mix_VolumeChunk(win, MIX_MAX_VOLUME / 4);

    lose = Mix_LoadWAV("Lose.wav");
    Mix_VolumeChunk(win, MIX_MAX_VOLUME / 2);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    mode = GameMode::HOW_TO_PLAY;

    // Initialize Game Objects

    // Initialize Player
    state.player = new Entity();
    state.player->position = glm::vec3(0, 4.0f, 0);
    state.player->acceleration = glm::vec3(0, -.015f, 0);
    state.player->speed = 0.0025f;
    state.player->fuel = 50;
    state.player->textureID = LoadTexture("tile_0381.png");
    state.player->entityType = EntityType::PLAYER;

    state.player->height = 0.9f;
    state.player->width = 0.55f;

    // Platforms
    state.platforms = new Entity[PLATFORM_COUNT];
    
    GLuint platformTextureID = LoadTexture("tile_0070.png");

    // Outer Walls
    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        state.platforms[i].textureID = platformTextureID;
        state.platforms[i].entityType = EntityType::PLATFORM;

        if (i < 7)
        {
            state.platforms[i].position = glm::vec3(-4.5f, 3.75f - i, 0);
        }
        else if (i < 14)
        {
            state.platforms[i].position = glm::vec3(4.5f, 3.75f - (i % 7), 0);
        }
        else if (i < 18)
        {
            state.platforms[i].position = glm::vec3(-4.5f + (i % 4), -3.25f, 0);
        }
        else if (i < 22)
        {
            state.platforms[i].position = glm::vec3(1.5f + (i % 4), -3.25f, 0);
        }
    }

    // Inner Platforms
    state.platforms[22].position = glm::vec3(-2.5f, 0.75f, 0);

    state.platforms[23].position = glm::vec3(2.5f, 0.75f, 0);

    state.platforms[24].position = glm::vec3(-3.5f, -0.25f, 0);

    state.platforms[25].position = glm::vec3(3.5f, -0.25f, 0);

    state.platforms[26].position = glm::vec3(-0.5f, -0.25f, 0);

    state.platforms[27].position = glm::vec3(0.5f, -0.25f, 0);

    for (int i = 0; i < PLATFORM_COUNT; i++)
    {
        state.platforms[i].Update(0, NULL, 0);
    }

    // Goals
    state.goals = new Entity[GOAL_COUNT];

    GLuint goalTextureID = LoadTexture("tile_0165.png");

    state.goals[0].textureID = goalTextureID;
    state.goals[0].entityType = EntityType::GOAL;
    state.goals[0].position = glm::vec3(-0.5f, -3.25, 0);

    state.goals[1].textureID = goalTextureID;
    state.goals[1].entityType = EntityType::GOAL;
    state.goals[1].position = glm::vec3(0.5f, -3.25, 0);

    state.goals[2].textureID = goalTextureID;
    state.goals[2].entityType = EntityType::GOAL;
    state.goals[2].position = glm::vec3(-3.5f, 0.75f, 0);

    state.goals[3].textureID = goalTextureID;
    state.goals[3].entityType = EntityType::GOAL;
    state.goals[3].position = glm::vec3(3.5f, 0.75f, 0);

    for (int i = 0; i < GOAL_COUNT; i++)
    {
        state.goals[i].Update(0, NULL, 0);
    }

    // Font
    fontTextureID = LoadTexture("font.png");
}

void ProcessInput() {

    state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            // Handles descreasing fuel when pressing LEFT or RIGHT arrows
            case SDLK_LEFT:
                if (mode == GameMode::GAME_LEVEL && !gameOver && state.player->fuel > 0)
                    state.player->fuel--;
                break;
            case SDLK_RIGHT:
                if (mode == GameMode::GAME_LEVEL && !gameOver && state.player->fuel > 0)
                    state.player->fuel--;
                break;
            // Handles moving between Game Modes with SPACE
            case SDLK_SPACE:
                if(mode == GameMode::HOW_TO_PLAY)
                    mode = GameMode::GAME_LEVEL;
                else if (mode == GameMode::GAME_LEVEL && gameOver)
                    mode = GameMode::GAME_OVER;
                break;
            // Handles restarting game with R
            case SDLK_r:
                if (mode == GameMode::GAME_OVER)
                {
                    state.player->textureID = LoadTexture("tile_0381.png");
                    state.player->position = glm::vec3(0, 4.0f, 0);
                    state.player->velocity = glm::vec3(0);
                    state.player->acceleration = glm::vec3(0, -0.015, 0);
                    state.player->fuel = 50;
                    state.player->lastCollision = EntityType::NONE;

                    gameOver = false;
                    
                    mode = GameMode::GAME_LEVEL;
                }
                break;
            // Handles exiting game with ESC
            case SDLK_ESCAPE:
                if (mode == GameMode::GAME_OVER)
                    gameIsRunning = 0;
                break;
            }
            break; // SDL_KEYDOWN
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    // Handles accelerating left and right using LEFT and RIGHT arrows
    if (keys[SDL_SCANCODE_LEFT] && mode == GameMode::GAME_LEVEL && state.player->fuel > 0) {
        state.player->acceleration.x -= state.player->speed;
    }
    else if (keys[SDL_SCANCODE_RIGHT] && mode == GameMode::GAME_LEVEL && state.player->fuel > 0) {
        state.player->acceleration.x += state.player->speed;
    }
}

#define FIXED_TIMESTEP 0.0166666f
float lastTicks = 0;
float accumulator = 0.0f;

void Update()
{
    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float deltaTime = ticks - lastTicks;
    lastTicks = ticks;

    deltaTime += accumulator;
    if (deltaTime < FIXED_TIMESTEP)
    {
        accumulator = deltaTime;
        return;
    }

    while (deltaTime >= FIXED_TIMESTEP)
    {
        // Only update player when you're in Game Level and there has been no collision
        if (mode == GameMode::GAME_LEVEL && !gameOver)
        {
            state.player->Update(FIXED_TIMESTEP, state.platforms, PLATFORM_COUNT);
            state.player->Update(FIXED_TIMESTEP, state.goals, GOAL_COUNT);
        }

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    // Updates player model when a collision occurs
    if (state.player->lastCollision == EntityType::GOAL && !gameOver)
    {
        state.player->textureID = LoadTexture("tile_0380.png"); 
        Mix_PlayChannel(-1, win, 0);
        gameOver = true;
    }
    else if (state.player->lastCollision == EntityType::PLATFORM && !gameOver)
    {
        state.player->textureID = LoadTexture("tile_0380.png");
        Mix_PlayChannel(-1, lose, 0);
        gameOver = true;
    }
}

void DrawText(ShaderProgram* program, GLuint fontTextureID, std::string text,
    float size, float spacing, glm::vec3 position)
{
    float width = 1.0f / 16.0f;
    float height = 1.0f / 16.0f;

    std::vector<float> vertices;
    std::vector<float> texCoords;

    for (int i = 0; i < text.size(); i++)
    {
        int index = (int)text[i];
        float offset = (size + spacing) * i;

        float u = (float)(index % 16) / 16.0f;
        float v = (float)(index / 16) / 16.0f;

        vertices.insert(vertices.end(), {
            offset + (-0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (0.5f * size), -0.5f * size,
            offset + (0.5f * size), 0.5f * size,
            offset + (-0.5f * size), -0.5f * size
            });

        texCoords.insert(texCoords.end(), {
            u, v,
            u, v + height,
            u + width, v,
            u + width, v + height,
            u + width, v,
            u, v + height
            });
    }

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, position);
    program->SetModelMatrix(modelMatrix);

    glUseProgram(program->programID);

    glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices.data());
    glEnableVertexAttribArray(program->positionAttribute);

    glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords.data());
    glEnableVertexAttribArray(program->texCoordAttribute);

    glBindTexture(GL_TEXTURE_2D, fontTextureID);
    glDrawArrays(GL_TRIANGLES, 0, (int)(text.size() * 6));

    glDisableVertexAttribArray(program->positionAttribute);
    glDisableVertexAttribArray(program->texCoordAttribute);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Displays how to play
    if (mode == GameMode::HOW_TO_PLAY)
    {
        DrawText(&program, fontTextureID, "How To Play", 0.5f, -0.25f,
            glm::vec3(-1.5f, 3.0f, 0));

        DrawText(&program, fontTextureID, "Use the LEFT and RIGHT arrows to move", 0.5f, -0.25f,
            glm::vec3(-4.5f, 2.0f, 0));

        DrawText(&program, fontTextureID, "You want to reach these goals:", 0.5f, -0.25f,
            glm::vec3(-4.5f, 0.75f, 0));
        state.goals[3].Render(&program);

        DrawText(&program, fontTextureID, "Hitting other blocks is Game Over", 0.5f, -0.25f,
            glm::vec3(-4.0f, -0.5f, 0));

        DrawText(&program, fontTextureID, "Fuel is lost as you move", 0.5f, -0.25f,
            glm::vec3(-3.0f, -1.75f, 0));

        DrawText(&program, fontTextureID, "Press SPACE to start", 0.5f, -0.25f,
            glm::vec3(-2.5f, -3.0f, 0));
    }
    // Displays the level. Includes player, platforms, goals and outcome messages
    else if (mode == GameMode::GAME_LEVEL)
    {
        for (int i = 0; i < PLATFORM_COUNT; i++)
        {
            state.platforms[i].Render(&program);
        }

        for (int i = 0; i < GOAL_COUNT; i++)
        {
            state.goals[i].Render(&program);
        }

        state.player->Render(&program);

        DrawText(&program, fontTextureID, "Fuel: " + std::to_string(state.player->fuel),
            0.5f, -0.25f, glm::vec3(-3.75f, 3.5f, 0));

        if (state.player->lastCollision == EntityType::GOAL)
        {
            DrawText(&program, fontTextureID, "Mission Succesful!", 0.5f, -0.25f,
                glm::vec3(-2.0f, 3.0f, 0));

            DrawText(&program, fontTextureID, "Press SPACE to continue", 0.5f, -0.25f,
                glm::vec3(-2.75f, 2.5f, 0));
        }
        else if (state.player->lastCollision == EntityType::PLATFORM)
        {
            DrawText(&program, fontTextureID, "Mission Failed!", 0.5f, -0.25f,
                glm::vec3(-1.75f, 3.0f, 0));

            DrawText(&program, fontTextureID, "Press SPACE to continue", 0.5f, -0.25f,
                glm::vec3(-2.75f, 2.5f, 0));
        }
    }
    // Display Game Over screen with options to restart or exit. Also displays credits
    else if (mode == GameMode::GAME_OVER)
    {
        DrawText(&program, fontTextureID, "Game Over", 0.5f, -0.25f,
            glm::vec3(-1.0f, 3.25f, 0));
      
        DrawText(&program, fontTextureID, "Press R to play again", 0.5f, -0.25f,
            glm::vec3(-2.5f, 2.75f, 0));
        DrawText(&program, fontTextureID, "Press ESC to exit", 0.5f, -0.25f,
            glm::vec3(-2.0f, 2.25f, 0));

        DrawText(&program, fontTextureID, "CREDITS", 0.5f, -0.25f,
            glm::vec3(-0.75f, 1.5f, 0));

        DrawText(&program, fontTextureID, "1-Bit Platformer Pack by Kenney", 0.5f, -0.25f,
            glm::vec3(-4.0f, 1.0f, 0));
        DrawText(&program, fontTextureID, "(kenney.nl)", 0.5f, -0.25f,
            glm::vec3(-1.25f, 0.5f, 0));

        DrawText(&program, fontTextureID, "Wholesome by Kevin MacLeod", 0.5f, -0.25f,
            glm::vec3(-3.25f, -0.25f, 0));
        DrawText(&program, fontTextureID, "(incompetech.filmmusic.io)", 0.5f, -0.25f,
            glm::vec3(-3.25f, -0.75f, 0));

        DrawText(&program, fontTextureID, "Life Lost Game Over by noirenex", 0.5f, -0.25f,
            glm::vec3(-3.75f, -1.5f, 0));
        DrawText(&program, fontTextureID, "(freesound.org)", 0.5f, -0.25f,
            glm::vec3(-1.75f, -2.0f, 0));

        DrawText(&program, fontTextureID, "PigLevelWin2 by Tuudrut", 0.5f, -0.25f,
            glm::vec3(-2.75f, -2.75f, 0));
        DrawText(&program, fontTextureID, "(freesound.org)", 0.5f, -0.25f,
            glm::vec3(-1.75f, -3.25f, 0));
    }

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();
        Render();
    }

    Shutdown();
    return 0;
}