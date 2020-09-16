// Project 1
// Mateo Castro
// mc7212

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

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;

ShaderProgram program;
glm::mat4 viewMatrix, basketballMatrix, basketball2Matrix, athleteMatrix, projectionMatrix;

float athlete_x = 0;
float athlete_y = 0;

float basketball_x = 0;
float basketball_y = 0;
float basketball_rotate = 0;
float basketball_x_displacement = 2.0f;
float basketball_y_displacement = 2.0f;

float basketball2_x = 0;
float basketball2_y = 3.5f;
float basketball2_rotate = 0;
float basketball2_x_displacement = -2.0f;
float basketball2_y_displacement = -2.0f;

GLuint basketballTextureID;
GLuint athleteTextureID;

GLuint LoadTexture(const char* filePath)
{
	int w, h, n;
	unsigned char* image = stbi_load(filePath, &w, &h, &n, STBI_rgb_alpha);

	if (image == NULL)
	{
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

void Initialize()
{
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Project 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
	basketballMatrix = glm::mat4(1.0f);
	athleteMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_BLEND);

	// Good setting for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	basketballTextureID = LoadTexture("Basketball.png");
	athleteTextureID = LoadTexture("athlete.png");
}

void ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE)
		{
			gameIsRunning = false;
		}
	}
}

float lastTicks = 0.0f;

void Update()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;
    
	// Keeping track of athlete's x and y position
	athlete_x += 2.0f * deltaTime;
	athlete_y += 1.0f * deltaTime;

	/*
	* Translate - ranslate - Moves athlete constantly right, at 2.2 units below the center. Meanwhile
	* the if statement allows the athlete to 'teleport' from the right wall to the left wall
	* after he goes 'into' the right wall.
	*
	* Scale - Makes the athlete bigger
	*/
	athleteMatrix = glm::mat4(1.0f);
	athleteMatrix = glm::translate(athleteMatrix, glm::vec3(athlete_x, -2.2f, 0.0f));
	if (athlete_x > 6.0f)
	{
		athlete_x *= -1.0f;
	}
	athleteMatrix = glm::scale(athleteMatrix, glm::vec3(3.0f, 3.0f, 1.0f));

	// Keeping track of the first basketball's x and y positions, aswell as its rotation
	basketball_x += basketball_x_displacement * deltaTime;
	basketball_y += basketball_y_displacement * deltaTime;
	basketball_rotate += -90.0f * deltaTime;

	/*
	* Translate - Allows the basketball to move in both the x and y directions. Meanwhile
	* the ifs allow the basketball to 'bounce' off the walls (center is bottom wall) by 
	* negating the displacements if the ball goes 'into' any of the walls.
	* 
	* Rotate - Constantly rotates the basketball counter-clockwise
	*/
	basketballMatrix = glm::mat4(1.0f);
	basketballMatrix = glm::translate(basketballMatrix, glm::vec3(basketball_x, basketball_y, 0.0f));
	if (basketball_x > 4.7f || basketball_x < -4.7f)
	{
		basketball_x_displacement *= -1.0f;
	}
	if (basketball_y > 3.5f || basketball_y < 0.0f)
	{
		basketball_y_displacement *= -1.0f;
	}
	basketballMatrix = glm::rotate(basketballMatrix, glm::radians(basketball_rotate), glm::vec3(0.0f, 0.0f, 1.0f));

	// Keeping track of the second basketball's x and y positions, aswell as its rotation
	basketball2_x += basketball2_x_displacement * deltaTime;
	basketball2_y += basketball2_y_displacement * deltaTime;
	basketball2_rotate += 90.0f * deltaTime;

	/*
	* Translate - Allows the basketball to move in both the x and y directions. Meanwhile
	* the ifs allow the basketball to 'bounce' off the walls (center is bottom wall) by
	* negating the displacements if the ball goes 'into' any of the walls.
	*
	* Rotate - Constantly rotates the basketball clockwise
	*/
	basketball2Matrix = glm::mat4(1.0f);
	basketball2Matrix = glm::translate(basketball2Matrix, glm::vec3(basketball2_x, basketball2_y, 0.0f));
	if (basketball2_x > 4.7f || basketball2_x < -4.7f)
	{
		basketball2_x_displacement *= -1.0f;
	}
	if (basketball2_y > 3.5f || basketball2_y < 0.0f)
	{
		basketball2_y_displacement *= -1.0f;
	}
	basketball2Matrix = glm::rotate(basketball2Matrix, glm::radians(basketball2_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
}

void drawBasketball()
{
	program.SetModelMatrix(basketballMatrix);
	glBindTexture(GL_TEXTURE_2D, basketballTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawBasketball2()
{
	program.SetModelMatrix(basketball2Matrix);
	glBindTexture(GL_TEXTURE_2D, basketballTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawathlete()
{
	program.SetModelMatrix(athleteMatrix);
	glBindTexture(GL_TEXTURE_2D, athleteTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	float vertices[] = { -0.5, -0.5, 0.5, -0.5, 0.5, 0.5, -0.5, -0.5, 0.5, 0.5, -0.5, 0.5 };
	float texCoords[] = { 0.0, 1.0, 1.0, 1.0, 1.0, 0.0, 0.0, 1.0, 1.0, 0.0, 0.0, 0.0 };

	glVertexAttribPointer(program.positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program.positionAttribute);
	glVertexAttribPointer(program.texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program.texCoordAttribute);

	drawBasketball();
	drawBasketball2();
	drawathlete();

	glDisableVertexAttribArray(program.positionAttribute);
	glDisableVertexAttribArray(program.texCoordAttribute);

	SDL_GL_SwapWindow(displayWindow);
}

void Shutdown()
{
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	Initialize();

	while (gameIsRunning)
	{
		ProcessInput();
		Update();
		Render();
	}

	Shutdown();
	return 0;
}