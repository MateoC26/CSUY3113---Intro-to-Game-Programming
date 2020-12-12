/*
* Project 5 - Platformer
* Snowy Mountain Climb
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

Scene* currentScene;
Scene* sceneList[5];

Effects* effects;

Mix_Music* music;
Mix_Chunk* jump;

void SwitchToScene(Scene* scene) {
    currentScene = scene;
    currentScene->Initialize();
}

void Initialize() {
    SDL_Init(SDL_INIT_VIDEO);
    displayWindow = SDL_CreateWindow("Snowy Mountain Climb", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
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

    jump = Mix_LoadWAV("Jump.wav");
    Mix_VolumeChunk(jump, MIX_MAX_VOLUME / 2);

    viewMatrix = glm::mat4(1.0f);
    modelMatrix = glm::mat4(1.0f);
    projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    program.SetProjectionMatrix(projectionMatrix);
    program.SetViewMatrix(viewMatrix);

    glUseProgram(program.programID);

    glClearColor(0.65f, 0.65f, 1.0f, 1.0f);
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

    if(currentScene->state.player && currentScene->state.player->isActive) currentScene->state.player->movement = glm::vec3(0);

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
                if (currentScene->state.player && currentScene->state.player->collidedBottomMap)
                {
                    currentScene->state.player->jump = true;
                    Mix_PlayChannel(-1, jump, 0);
                }
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

    if (currentScene->state.player)
    {
        if (keys[SDL_SCANCODE_LEFT]) {
            currentScene->state.player->movement.x = -1.0f;
            if (!currentScene->state.player->justJumped)
            {
                currentScene->state.player->animFrames = 8;
                currentScene->state.player->animIndices = currentScene->state.player->animLeft;
            }
        }
        else if (keys[SDL_SCANCODE_RIGHT]) {
            currentScene->state.player->movement.x = 1.0f;
            if (!currentScene->state.player->justJumped)
            {
                currentScene->state.player->animFrames = 8;
                currentScene->state.player->animIndices = currentScene->state.player->animRight;
            }
        }

        if (glm::length(currentScene->state.player->movement) > 1.0f) {
            currentScene->state.player->movement = glm::normalize(currentScene->state.player->movement);
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
        currentScene->Update(FIXED_TIMESTEP);

        effects->Update(FIXED_TIMESTEP);

        deltaTime -= FIXED_TIMESTEP;
    }

    accumulator = deltaTime;

    viewMatrix = glm::mat4(1.0f);
    if (currentScene->state.player)
    {
        if (currentScene->state.player->position.x > 5 && currentScene->state.player->position.x < 16)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-currentScene->state.player->position.x, 3.75, 0));
        }
        else if (currentScene->state.player->position.x > 16)
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-16, 3.75, 0));
        }
        else
        {
            viewMatrix = glm::translate(viewMatrix, glm::vec3(-5, 3.75, 0));
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

    effects->Render();

    SDL_GL_SwapWindow(displayWindow);
}


void Shutdown() {
    SDL_Quit();

    for (int i = 0; i < 5; i++) sceneList[i]->FreeMusic();
    Mix_FreeMusic(music);
}

int main(int argc, char* argv[]) {
    Initialize();

    while (gameIsRunning) {
        ProcessInput();
        Update();

        if (currentScene->state.nextScene >= 0)
        {
            int currLives;

            effects->Start(FADEOUT, 0.5f);
            if (currentScene->state.player && currentScene != sceneList[4]) currLives = currentScene->state.player->lives;

            SwitchToScene(sceneList[currentScene->state.nextScene]);

            effects->Start(FADEIN, 0.5f);
            if (currentScene->state.player && currentScene != sceneList[1]) currentScene->state.player->lives = currLives;
        }

        Render();
    }

    Shutdown();
    return 0;
}