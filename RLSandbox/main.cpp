#include <stdlib.h>
#include <raylib.h>
#include <raymath.h>
#include "particle.h"

const char* APP_TITLE = "RayLib Particles toy by DevHrytsan";
const int APP_TARGET_FPS = -1; //Sets target FPS of the app. -1 means no limit to FPS.

const long int PARTICLE_COUNT = 50000;

const int APP_SCREEN_WIDTH = 1280; //Screen size
const int APP_SCREEN_HEIGHT = 768;

const int DRAW_TEX_WIDTH = APP_SCREEN_WIDTH * 1;  //Size of texture. 
const int DRAW_TEX_HEIGHT = APP_SCREEN_HEIGHT * 1;

const int CAMERA_SPEED = 500;
const int CAMERA_SCROLL_SPEED = 40;

const Color MAIN_BG_COLOR = BLACK;

void HandleCamera(Camera2D& camera, float deltaTime) {

	if (IsKeyDown(KEY_W)) {
		camera.target.y -= CAMERA_SPEED * deltaTime;
	}
	else if (IsKeyDown(KEY_S)) {
		camera.target.y += CAMERA_SPEED * deltaTime;
	}

	if (IsKeyDown(KEY_A)) {
		camera.target.x -= CAMERA_SPEED * deltaTime;
	}
	else if (IsKeyDown(KEY_D)) {
		camera.target.x += CAMERA_SPEED * deltaTime;
	}

	if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON)) {
		camera.zoom = 1;
	}
	else if (fabs(GetMouseWheelMove()) > 0) {
		camera.zoom += GetMouseWheelMove() * CAMERA_SCROLL_SPEED * deltaTime;
	}

	camera.zoom = Clamp(camera.zoom, 0.3, 3);
}

int main(void) {

	InitWindow(APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT, APP_TITLE);
	SetTargetFPS(APP_TARGET_FPS); //Set game to run at specific(APP_TARGET_FPS) FPS

	Camera2D camera = { 0 }; //Create new instance of camera
	camera.offset = { APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT / 2.0f };
	camera.zoom = 1.0f;

	//Element initialization 
	long int particleCount = PARTICLE_COUNT;

	Particle* particles = (Particle*)malloc(particleCount * sizeof(Particle));

	for (long int i = 0; i < particleCount; i++)
	{
		particles[i] = Particle(camera.offset, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT);
	}

	//Texture and Image initialization
	Color* pixelsBytes = (Color*)malloc(DRAW_TEX_WIDTH * DRAW_TEX_HEIGHT * sizeof(Color));
	Image screenImage = { pixelsBytes, DRAW_TEX_WIDTH, DRAW_TEX_HEIGHT, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };
	Texture2D screenTexture = LoadTextureFromImage(screenImage);

	//Main game loop
	while (!WindowShouldClose())
	{
		float deltaTime = GetFrameTime();

		HandleCamera(camera, deltaTime);

		Vector2 cameraTotalOffset = { camera.target.x + camera.offset.x,camera.target.y + camera.offset.y };

		float invZoomMultiplier = 1 / camera.zoom;

		Vector2 realMousePos = { cameraTotalOffset.x + ((float)GetMouseX() * invZoomMultiplier), cameraTotalOffset.y + ((float)GetMouseY() * invZoomMultiplier) };

		ImageDrawRectangle(&screenImage, 0, 0, screenTexture.width, screenTexture.height, BLANK);
		//DevHrytsan: This is most clever method to clean image after particles drawing iteration :)

		for (int i = 0; i < particleCount; i++)
		{
			if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
				particles[i].AttractToPos(realMousePos, 500);
			}
			else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
				particles[i].AttractToPos(realMousePos, -500);
			}

			particles[i].DoFriction(0.99);
			particles[i].Step(deltaTime);

			//It uses technic: Rendering a single textured quad and just updating the texture each frame. 
			int xElemPos = particles[i].GetPos().x;
			int yElemPos = particles[i].GetPos().y;

			int xRelElemPos = (xElemPos - cameraTotalOffset.x) * camera.zoom;
			int yRelElemPos = (yElemPos - cameraTotalOffset.y) * camera.zoom;


			if ((xRelElemPos > 0 && xRelElemPos < APP_SCREEN_WIDTH) && (yRelElemPos > 0 && yRelElemPos < APP_SCREEN_HEIGHT)) {

				//This calculation scales the screen coordinates to the range of the image coordinates. 
				//It maintains the aspect ratio of the image while mapping the screen coordinates to the image coordinates.
				int imageRelPosX = (screenImage.width * xRelElemPos) / APP_SCREEN_WIDTH;
				int imageRelPosY = (screenImage.height * yRelElemPos) / APP_SCREEN_HEIGHT;

				ImageDrawPixel(&screenImage, imageRelPosX, imageRelPosY, particles[i].GetColor());
				//Or other method of this:
				//int index = (imageRelPosY * APP_SCREEN_WIDTH + imageRelPosX);
				//pixelsBytes[index] = elements[i].GetColor();
			}
		}

		//Drawing
		BeginDrawing();

		ClearBackground(MAIN_BG_COLOR); // Sets background color
		UpdateTexture(screenTexture, screenImage.data); //So we update our texture here with our updated screen raw data 

		DrawTexturePro(screenTexture, { 0.0f, 0.0f, static_cast<float>(screenTexture.width), static_cast<float>(screenTexture.height) },
			{ APP_SCREEN_WIDTH / 2.0f, APP_SCREEN_HEIGHT / 2.0f, APP_SCREEN_WIDTH, APP_SCREEN_HEIGHT },
			{ APP_SCREEN_WIDTH / 2, APP_SCREEN_HEIGHT / 2 }, 0.0f, WHITE);

		EndMode2D();

		DrawFPS(10, 10);
		EndDrawing();
	}

	CloseWindow();

	//De-Initialization 
	UnloadImage(screenImage);
	UnloadTexture(screenTexture);
	free(particles);
	free(pixelsBytes);

	return 0;
}
