/*
* Project 6 - Students Choice
* Dungeon Delver
* Mateo Castro - mc7212
*/

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

#include "Util.h"
#include "Effects.h"
#include "Entity.h"
#include "Map.h"
#include "Scene.h"
#include "MainMenu.h"
#include "Level1.h"
#include "Level2.h"
#include "Level3.h"
#include "EndScreen.h"

#include <SDL_mixer.h>

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, modelMatrix, projectionMatrix;

glm::mat4 uiViewMatrix, uiProjectionMatrix;
GLuint fontTextureID;
GLuint heartTextureID;
GLuint heartEmptyTextureID;

Scene* currentScene;
Scene* sceneList[5];

Effects* effects;

Mix_Music* music;
Mix_Chunk* fireball;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Dungeon Delver", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
    SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(0, 0, 640, 480);

    program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

    uiViewMatrix = glm::mat4(1.0);
    uiProjectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);
    fontTextureID = Util::LoadTexture("font.png");
    heartTextureID = Util::LoadTexture("Heart.png");
    heartEmptyTextureID = Util::LoadTexture("HeartEmpty.png");

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
    music = Mix_LoadMUS("Our-Mountain_v003_Looping.mp3");
    Mix_PlayMusic(music, -1);
    Mix_VolumeMusic(MIX_MAX_VOLUME / 2.5);

    fireball = Mix_LoadWAV("Fireball.wav");
    Mix_VolumeChunk(fireball, MIX_MAX_VOLUME / 2);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0, 0, 0, 1.0f);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    sceneList[0] = new MainMenu();
    sceneList[1] = new Level1();
    sceneList[2] = new Level2();
    sceneList[3] = new Level3();
    sceneList[4] = new EndScreen();
    SwitchToScene(sceneList[0]);

    effects = new Effects(projectionMatrix, viewMatrix);
}

void ProcessInput() {

    if (currentScene->state.player) currentScene->state.player->movement = glm::vec3(0);

    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            gameIsRunning = false;
            break;

        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                break;

            case SDLK_RETURN:
                if (!currentScene->state.player)
                    currentScene->state.nextScene = 1;
                break;

            case SDLK_r:
                if (currentScene == sceneList[4])
                    currentScene->state.nextScene = 1;
                break;

            case SDLK_ESCAPE:
                if (currentScene == sceneList[4])
                    gameIsRunning = false;
                break;
            }
            break;
        }
    }

    const Uint8* keys = SDL_GetKeyboardState(NULL);

    if (currentScene->state.player && currentScene->state.flame)
    {
        if (keys[SDL_SCANCODE_A]) {
            currentScene->state.player->movement.x = -1.0f;
        }
        else if (keys[SDL_SCANCODE_D]) {
            currentScene->state.player->movement.x = 1.0f;
        }
        else if (keys[SDL_SCANCODE_W]) {
            currentScene->state.player->movement.y = 1.0f;
        }
        else if (keys[SDL_SCANCODE_S]) {
            currentScene->state.player->movement.y = -1.0f;
        }

        if (keys[SDL_SCANCODE_LEFT] && !currentScene->state.flame->shotOut && currentScene->state.player->lives > 0) {
            Mix_PlayChannel(-1, fireball, 0);

            currentScene->state.flame->shotOut = true;
            currentScene->state.flame->position = glm::vec3(currentScene->state.player->position.x - 1, currentScene->state.player->position.y, 0);
            currentScene->state.flame->movement.x = -1.0f;
            currentScene->state.flame->isActive = true;
        }
        else if (keys[SDL_SCANCODE_RIGHT] && !currentScene->state.flame->shotOut && currentScene->state.player->lives > 0) {
            Mix_PlayChannel(-1, fireball, 0);

            currentScene->state.flame->shotOut = true;
            currentScene->state.flame->position = glm::vec3(currentScene->state.player->position.x + 1, currentScene->state.player->position.y, 0);
            currentScene->state.flame->movement.x = 1.0f;
            currentScene->state.flame->isActive = true;
        }
        else if (keys[SDL_SCANCODE_UP] && !currentScene->state.flame->shotOut && currentScene->state.player->lives > 0) {
            Mix_PlayChannel(-1, fireball, 0);
            
            currentScene->state.flame->shotOut = true;
            currentScene->state.flame->position = glm::vec3(currentScene->state.player->position.x, currentScene->state.player->position.y + 1, 0);
            currentScene->state.flame->movement.y = 1.0f;
            currentScene->state.flame->isActive = true;
        }
        else if (keys[SDL_SCANCODE_DOWN] && !currentScene->state.flame->shotOut && currentScene->state.player->lives > 0) {
            Mix_PlayChannel(-1, fireball, 0);
            
            currentScene->state.flame->shotOut = true;
            currentScene->state.flame->position = glm::vec3(currentScene->state.player->position.x, currentScene->state.player->position.y - 1, 0);
            currentScene->state.flame->movement.y = -1.0f;
            currentScene->state.flame->isActive = true;
        }

        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
        }

        if (glm::length(currentScene->state.flame->movement) > 1.0f) {
            currentScene->state.flame->movement = glm::normalize(currentScene->state.flame->movement);
        }
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
        if(currentScene != sceneList[0])
            currentScene->Update(FIXED_TIMESTEP);

        effects->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player)
    {
        if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 15)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 0, 0));
        }
        else if (currentScene->state.player->position.x >= 15)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-15, 0, 0));
        }
        else
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 0, 0));
        }

        if (currentScene == sceneList[3])
        {
            if (currentScene->state.player->position.y > -8.25 && currentScene->state.player->position.y < -3.75)
            {
                viewMatrix = glm::translate(viewMatrix, glm::vec3(0, -currentScene->state.player->position.y, 0));
            }
            else if (currentScene->state.player->position.y >= -3.75)
            {
                viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 3.75, 0));
            }
            else
            {
                viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 8.25, 0));
            }
        }
        else
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(0, 3.75, 0));
        }
    }
    else
    {
        viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
    }

    viewMatrix = glm::translate(viewMatrix, effects->viewOffset);
}

void Render() {
    glClear(GL_COLOR_BUFFER_BIT);

    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);
    currentScene->Render(&program);

    if (currentScene->state.player && currentScene != sceneList[4])
    {
        program.SetProjectionMatrix(uiProjectionMatrix);
        program.SetViewMatrix(uiViewMatrix);

        int maxLives = 3;

        if (currentScene->state.player->hasPotion)
            maxLives = 4;

        Util::DrawText(&program, fontTextureID, "Score: " + std::to_string(currentScene->state.player->score), 0.5, -0.3f, glm::vec3(2.75, 3.5, 0));

        for (int i = 0; i < currentScene->state.player->lives; i++)
        {
            Util::DrawIcon(&program, heartTextureID, glm::vec3(-4.35 + (i * 1.15), 3.2, 0));
        }

        for (int i = 0; i < maxLives - currentScene->state.player->lives; i++)
        {
            if(maxLives == 3)
                Util::DrawIcon(&program, heartEmptyTextureID, glm::vec3(-2.05 - (i * 1.15), 3.2, 0));
            else if(maxLives == 4)
                Util::DrawIcon(&program, heartEmptyTextureID, glm::vec3(-0.9 - (i * 1.15), 3.2, 0));
        }
    }

    effects->Render();

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();

    for (int i = 0; i < 5; i++) sceneList[i]->FreeMusic();
    Mix_FreeMusic(music);
    Mix_FreeChunk(fireball);
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0)
        {
            int currLives;
            int currScore;
            bool potion;

            effects->Start(FADEOUT, 0.5f);
            if (currentScene->state.player && currentScene != sceneList[4])
            {
                currLives = currentScene->state.player->lives;
                currScore = currentScene->state.player->score;
                potion = currentScene->state.player->hasPotion;
            }

            SwitchToScene(sceneList[currentScene->state.nextScene]);

            effects->Start(FADEIN, 0.5f);
            if (currentScene->state.player && currentScene != sceneList[1])
            {
                currentScene->state.player->lives = currLives;
                currentScene->state.player->score = currScore;
                currentScene->state.player->hasPotion = potion;
            }
        }

        Render();
    }

    Shutdown();
    return 0;
}