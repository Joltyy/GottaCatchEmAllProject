#include "growlithe.hpp"
#include <iostream>

ALLEGRO_BITMAP* growlithe_left;
ALLEGRO_BITMAP* growlithe_right;
ALLEGRO_BITMAP* growlithe_up;
ALLEGRO_BITMAP* growlithe_down;
ALLEGRO_BITMAP* growlithe_attk_down;
ALLEGRO_BITMAP* growlithe_attk_left;
ALLEGRO_BITMAP* growlithe_attk_right;
ALLEGRO_BITMAP* growlithe_attk_up;

growlithe::growlithe(int x, int y)
    : Enemy("Animations/growlithe_right.png", x, y, 20, 20, (50 * extraHp), 10, 20, (5 * extraDmg), 40), frameCount(0) {
    // Add the paths of your frame images to the frames vector.
    growlithe_right = al_load_bitmap("Resource/images/Animations/growlithe_right.png");
    growlithe_left = al_load_bitmap("Resource/images/Animations/growlithe_left.png");
    growlithe_down = al_load_bitmap("Resource/images/Animations/growlithe_down.png");
    growlithe_up = al_load_bitmap("Resource/images/Animations/growlithe_up.png");
}


void growlithe::Update(float deltaTime) {
    if (Velocity.x > 0) direction = 0; // Moving right
    else if (Velocity.x < 0) direction = 1; // Moving left
    else if (Velocity.y < 0) direction = 2; // Moving up
    else if (Velocity.y > 0) direction = 3; // Moving down

    static float frameTimer = 0;
    frameTimer += deltaTime;
    if (frameTimer >= 0.5f) { // 1 second delay
        frameTimer = 0;
        if(direction == 3 || direction == 2) {
            if (++frameCount >= 2) { // Assuming 2 frames
                frameCount = 0;
            }
        }
        else {
            if (++frameCount >= 3) { // Assuming 2 frames
                frameCount = 0;
            }
        }
    }
    

    Enemy::Update(deltaTime);
}

void growlithe::Draw() const {
    // Calculate the source position of the current frame
    int frameWidth = 32;
    int frameHeight = 32;
    int sourceX = frameCount * frameWidth;
    int sourceY = 0; // Assuming all frames are on the same row

    // Define the scale factor
    float scaleFactor = 2.2f; // Change this value to scale the boss

    // Calculate the new position based on the scale factor
    float scaledWidth = frameWidth * scaleFactor;
    float scaledHeight = frameHeight * scaleFactor;
    float newPositionX = Position.x - (scaledWidth - frameWidth) / 2 - 36;
    float newPositionY = Position.y - (scaledHeight - frameHeight) / 2 - 50;

    // Draw the current frame
    if(direction == 0)
    {
        al_draw_scaled_bitmap(growlithe_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    if(direction == 1)
    {
        al_draw_scaled_bitmap(growlithe_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 2)
    {
        al_draw_scaled_bitmap(growlithe_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 3)
    {
        al_draw_scaled_bitmap(growlithe_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    

    if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
        al_draw_circle(Position.x, Position.y, hitbox, al_map_rgb(0, 0, 100), 2);
	}
}