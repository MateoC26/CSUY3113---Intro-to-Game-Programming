/*
* Project 2 - Pong
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
#include "vector"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

SDL_Window* displayWindow;
bool gameIsRunning = true;
bool gameStarted = false;
bool gameOver = false;

ShaderProgram program;
glm::mat4 viewMatrix, leftPaddleMatrix, rightPaddleMatrix, ballMatrix, projectionMatrix;

glm::vec3 leftPaddle_position = glm::vec3(-4.0f, 0, 0);
glm::vec3 leftPaddle_movement = glm::vec3(0, 0, 0);

glm::vec3 rightPaddle_position = glm::vec3(4.0f, 0, 0);
glm::vec3 rightPaddle_movement = glm::vec3(0, 0, 0);

glm::vec3 ball_position = glm::vec3(0, 0, 0);
glm::vec3 ball_movement = glm::vec3(0, 0, 0);

float ball_speed = 6.0f;
float paddle_speed = 10.0f;

float ball_radius = 0.15f;
float paddle_height = 1.0f;
float paddle_width = 0.36f;

int leftPaddle_score = 0;
int rightPaddle_score = 0;

GLuint paddleTextureID;
GLuint ballTextureID;
GLuint fontTextureID;

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

	glClearColor(1.0f, 0.75f, 0.5f, 1.0f);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	paddleTextureID = LoadTexture("paddle.png");
	ballTextureID = LoadTexture("ball.png");
	fontTextureID = LoadTexture("font.png");
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
		    //Sets ball into movement
			case SDLK_SPACE:
				//Only occurs if the ball is in the middle and the game is not over
				if (ball_movement.x == 0 && ball_movement.y == 0 && !gameOver)
				{
					//Sets gameStarted to true and randomizes ball movement
					gameStarted = true;
					int rand_x = rand() % 100;
					int rand_y = rand() % 100;

					if (rand_x < 50)
					{
						ball_movement.x = 1.0f;
					}
					else
					{
						ball_movement.x = -1.0f;
					}

					if (rand_y < 50)
					{
						ball_movement.y = 1.0f;
					}
					else
					{
						ball_movement.y = -1.0f;
					}
				}
				break;
			//Restarts game
			case SDLK_r:
				if (gameOver)
				{
					gameOver = 0;
					gameStarted = 0;
					leftPaddle_score = 0;
					rightPaddle_score = 0;
				}
				break;
			//Closes game
			case SDLK_ESCAPE:
				if (gameOver)
				{
					gameIsRunning = false;
				}
				break;
			}
			break; // SDL_KEYDOWN
		}
	}

	const Uint8* keys = SDL_GetKeyboardState(NULL);

	if (!gameOver && gameStarted)
	{
		//Controls left paddle, W is up and S is down
		if (keys[SDL_SCANCODE_W] && leftPaddle_position.y + paddle_height / 2 < 3.75f)
		{
			leftPaddle_movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_S] && leftPaddle_position.y - paddle_height / 2 > -3.75f)
		{
			leftPaddle_movement.y = -1.0f;
		}

		//Controls right paddle, ARROW UP is up and ARROW DOWN is down
		if (keys[SDL_SCANCODE_UP] && rightPaddle_position.y + paddle_height / 2 < 3.75f)
		{
			rightPaddle_movement.y = 1.0f;
		}
		else if (keys[SDL_SCANCODE_DOWN] && rightPaddle_position.y - paddle_height / 2 > -3.75f)
		{
			rightPaddle_movement.y = -1.0f;
		}
	}

	// Normalize vector if it goes over speed for both paddles
	if (glm::length(leftPaddle_movement) > 1.0f)
	{
		leftPaddle_movement = glm::normalize(leftPaddle_movement);
	}
	if (glm::length(rightPaddle_movement) > 1.0f)
	{
		rightPaddle_movement = glm::normalize(rightPaddle_movement);
	}
}

//Resets ball to center of the screen and stops movement
void resetBall()
{
	ball_position.x = 0;
	ball_position.y = 0;

	ball_movement.x = 0;
	ball_movement.y = 0;
}

//Resets paddles and ball
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

	//Incrementing score if the ball hits side wall
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

	//Bouncing off top and bottom walls
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
	float xdist_left = fabs(ball_position.x - leftPaddle_position.x) - ((paddle_width +	ball_radius) / 2.0f);
	float ydist_left = fabs(ball_position.y - leftPaddle_position.y) - ((paddle_height + ball_radius) / 2.0f);

	float xdist_right = fabs(ball_position.x - rightPaddle_position.x) - ((paddle_width + ball_radius) / 2.0f);
	float ydist_right = fabs(ball_position.y - rightPaddle_position.y) - ((paddle_height + ball_radius) / 2.0f);

	if (xdist_left < 0 && ydist_left < 0)
	{
		ball_movement.x = 1.0f;
	}

	if (xdist_right < 0 && ydist_right < 0)
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

	//Checks if game has started and displays respective text
	if (!gameStarted)
	{
		DrawText(&program, fontTextureID, "First Player To 5 Points Wins", 0.5f, -0.25f,
			glm::vec3(-3.5f, 3.0f, 0));

		DrawText(&program, fontTextureID, "Press SPACE To Start", 0.5f, -0.25f, 
			glm::vec3(-2.3f, 2.5f, 0));
	}
	else
	{
		DrawText(&program, fontTextureID, std::to_string(leftPaddle_score),
			1.0f, -0.25f, glm::vec3(-2.0f, 3.0f, 0));

		DrawText(&program, fontTextureID, std::to_string(rightPaddle_score),
			1.0f, -0.25f, glm::vec3(2.0f, 3.0f, 0));
	}

	//Displays winner and options to close or play again
	if (gameOver)
	{
		if (leftPaddle_score == 5)
		{
			DrawText(&program, fontTextureID, "Player 1 Wins!", 0.5f, -0.25f, 
				glm::vec3(-1.6f, 1.0f, 0));
		}
		else
		{
			DrawText(&program, fontTextureID, "Player 2 Wins!", 0.5f, -0.25f,
				glm::vec3(-1.6f, 1.0f, 0));
		}

		DrawText(&program, fontTextureID, "Press R To Play Again", 0.5f, -0.25f,
			glm::vec3(-2.5f, -1.0f, 0));

		DrawText(&program, fontTextureID, "Press ESC To Close", 0.5f, -0.25f,
			glm::vec3(-2.1f, -2.0f, 0));
	}

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