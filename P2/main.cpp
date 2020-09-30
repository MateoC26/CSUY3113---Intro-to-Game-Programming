/*
* Project 2 - Pong
* Mateo Castro - mc7212
*/
#define GL_SILENCE_DEPRECATION

/*
* TODO
* Rename Collision variables in update()
* Add score tracker on screen
* Add instructions to start game
* Add game over/winner text
* 
* Optional:
* Randomize ball movement
*/

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
bool gameOver = false;

ShaderProgram program;
glm::mat4 viewMatrix, leftPaddleMatrix, rightPaddleMatrix, ballMatrix, projectionMatrix;

glm::vec3 leftPaddle_position = glm::vec3(-4.0f, 0, 0);
glm::vec3 leftPaddle_movement = glm::vec3(0, 0, 0);

glm::vec3 rightPaddle_position = glm::vec3(4.0f, 0, 0);
glm::vec3 rightPaddle_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float ball_speed = 3.0f;
float paddle_speed = 4.0f;

float ball_radius = 0.15f;
float paddle_height = 1.0f;
float paddle_width = 0.36f;

int leftPaddle_score = 0;
int rightPaddle_score = 0;

GLuint paddleTextureID;
GLuint ballTextureID;

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
	displayWindow = SDL_CreateWindow("Project 2 - Pong", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

#ifdef _WINDOWS
	glewInit();
#endif

	glViewport(0, 0, 640, 480);

	program.Load("shaders/vertex_textured.glsl", "shaders/fragment_textured.glsl");

	viewMatrix = glm::mat4(1.0f);
	leftPaddleMatrix = glm::mat4(1.0f);
	rightPaddleMatrix = glm::mat4(1.0f);
	ballMatrix = glm::mat4(1.0f);
	projectionMatrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

	program.SetProjectionMatrix(projectionMatrix);
	program.SetViewMatrix(viewMatrix);

	glUseProgram(program.programID);

	glClearColor(1.0f, 0.5f, 0.5f, 1.0f);
	glEnable(GL_BLEND);

	// Good setting for transparency
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	paddleTextureID = LoadTexture("paddle.png");
	ballTextureID = LoadTexture("ball.png");
}

void ProcessInput()
{
	leftPaddle_movement = glm::vec3(0);
	rightPaddle_movement = glm::vec3(0);

	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
		case SDL_WINDOWEVENT_CLOSE:
			gameIsRunning = false;
			break;

		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_SPACE:
				if (ball_movement.x == 0 && ball_movement.y == 0 && !gameOver)
				{
					ball_movement.x = 1.0f;
					ball_movement.y = 1.0f;
				}
				break;
			}
			break; // SDL_KEYDOWN
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (!gameOver)
	{
		if (keys[SDL_SCANCODE_W] && leftPaddle_position.y + paddle_height / 2 < 3.75f)
		{
			leftPaddle_movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_S] && leftPaddle_position.y - paddle_height / 2 > -3.75f)
		{
			leftPaddle_movement.y = -1.0f;
		}

		if (keys[SDL_SCANCODE_UP] && rightPaddle_position.y + paddle_height / 2 < 3.75f)
		{
			rightPaddle_movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_DOWN] && rightPaddle_position.y - paddle_height / 2 > -3.75f)
		{
			rightPaddle_movement.y = -1.0f;
		}
	}

	// Normalize vector if it goes over speed
	if (glm::length(leftPaddle_movement) > 1.0f)
	{
		leftPaddle_movement = glm::normalize(leftPaddle_movement);
	}

	if (glm::length(rightPaddle_movement) > 1.0f)
	{
		rightPaddle_movement = glm::normalize(rightPaddle_movement);
	}
}

void resetBall()
{
	ball_position.x = 0;
	ball_position.y = 0;

	ball_movement.x = 0;
	ball_movement.y = 0;
}

void reset()
{
	resetBall();

	leftPaddle_position.x = -4.0f;
	leftPaddle_position.y = 0;

	rightPaddle_position.x = 4.0f;
	rightPaddle_position.y = 0;
}

float lastTicks = 0.0f;

void Update()
{
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float deltaTime = ticks - lastTicks;
	lastTicks = ticks;

	//Ball
	ball_position += ball_movement * ball_speed * deltaTime;

	ballMatrix = glm::mat4(1.0f);
	ballMatrix = glm::translate(ballMatrix, ball_position);
	if (ball_position.x + ball_radius > 5.0f)
	{
		resetBall();
		leftPaddle_score++;
	}
	else if (ball_position.x - ball_radius < -5.0f)
	{
		resetBall();
		rightPaddle_score++;
	}

	if (ball_position.y + ball_radius > 3.75f)
	{
		ball_movement.y = -1.0f;
	}
	else if (ball_position.y - ball_radius < -3.75f)
	{ 
		ball_movement.y = 1.0f;
	}

	//Left Paddle
	leftPaddle_position += leftPaddle_movement * paddle_speed * deltaTime;

	leftPaddleMatrix = glm::mat4(1.0f);
	leftPaddleMatrix = glm::translate(leftPaddleMatrix, leftPaddle_position);

	//Right Paddle
	rightPaddle_position += rightPaddle_movement * paddle_speed * deltaTime;

	rightPaddleMatrix = glm::mat4(1.0f);
	rightPaddleMatrix = glm::translate(rightPaddleMatrix, rightPaddle_position);

	//Collision
	float xdist = fabs(ball_position.x - leftPaddle_position.x) - ((paddle_width +	ball_radius) / 2.0f);
	float ydist = fabs(ball_position.y - leftPaddle_position.y) - ((paddle_height + ball_radius) / 2.0f);

	float xdist1 = fabs(ball_position.x - rightPaddle_position.x) - ((paddle_width + ball_radius) / 2.0f);
	float ydist1 = fabs(ball_position.y - rightPaddle_position.y) - ((paddle_height + ball_radius) / 2.0f);

	if (xdist < 0 && ydist < 0)
	{
		ball_movement.x = 1.0f;
	}

	if (xdist1 < 0 && ydist1 < 0)
	{
		ball_movement.x = -1.0f;
	}

	//Score
	if (leftPaddle_score == 5 || rightPaddle_score == 5)
	{
		gameOver = true;
	}

	//Game Over
	if (gameOver)
	{
		reset();
	}
}

void drawLeftPaddle()
{
	program.SetModelMatrix(leftPaddleMatrix);
	glBindTexture(GL_TEXTURE_2D, paddleTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawRightPaddle()
{
	program.SetModelMatrix(rightPaddleMatrix);
	glBindTexture(GL_TEXTURE_2D, paddleTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawBall()
{
	program.SetModelMatrix(ballMatrix);
	glBindTexture(GL_TEXTURE_2D, ballTextureID);
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

	drawLeftPaddle();
	drawRightPaddle();
	drawBall();

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