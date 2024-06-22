#include "snorlax.hpp"
#include <iostream>

ALLEGRO_BITMAP* snorlax_left;
ALLEGRO_BITMAP* snorlax_right;
ALLEGRO_BITMAP* snorlax_up;
ALLEGRO_BITMAP* snorlax_down;
ALLEGRO_BITMAP* snorlax_attk_down;
ALLEGRO_BITMAP* snorlax_attk_left;
ALLEGRO_BITMAP* snorlax_attk_right;
ALLEGRO_BITMAP* snorlax_attk_up;

snorlax::snorlax(int x, int y)
    : Enemy("Animations/snorlax_down.png", x, y, 10, 50, 5 * extraHp, 5, 5, 1 * extraDmg, 10), frameCount(0) {
    // Add the paths of your frame images to the frames vector.
    snorlax_right = al_load_bitmap("Resource/images/Animations/snorlax_right.png");
    snorlax_left = al_load_bitmap("Resource/images/Animations/snorlax_left.png");
    snorlax_down = al_load_bitmap("Resource/images/Animations/snorlax_down.png");
    snorlax_up = al_load_bitmap("Resource/images/Animations/snorlax_up.png");
}

void snorlax::Update(float deltaTime) {
    static float frameTimer = 0;
    frameTimer += deltaTime;
    if (frameTimer >= 0.3f) { // 1 second delay
        frameTimer = 0;
        if (++frameCount >= 2) { // Assuming 2 frames
            frameCount = 0;
        }
    }
    if (Velocity.x > 0) direction = 0; // Moving right
    else if (Velocity.x < 0) direction = 1; // Moving left
    else if (Velocity.y < 0) direction = 2; // Moving up
    else if (Velocity.y > 0) direction = 3; // Moving down


    Enemy::Update(deltaTime);
}

void snorlax::Draw() const {
    // Calculate the source position of the current frame
    int frameWidth = 32;
    int frameHeight = 32;
    int sourceX = frameCount * frameWidth;
    int sourceY = 0; // Assuming all frames are on the same row

    // Define the scale factor
    float scaleFactor = 3.0f; // Change this value to scale the boss

    // Calculate the new position based on the scale factor
    float scaledWidth = frameWidth * scaleFactor;
    float scaledHeight = frameHeight * scaleFactor;
    float newPositionX = Position.x - (scaledWidth - frameWidth) / 2 - 18;
    float newPositionY = Position.y - (scaledHeight - frameHeight) / 2 - 20;

    // Draw the current frame
    if(direction == 0)
    {
        
        al_draw_scaled_bitmap(snorlax_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    if(direction == 1)
    {
       
        al_draw_scaled_bitmap(snorlax_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 2)
    {
        al_draw_scaled_bitmap(snorlax_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 3)
    {
       al_draw_scaled_bitmap(snorlax_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    

    if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
        al_draw_circle(Position.x, Position.y, hitbox, al_map_rgb(0, 0, 100), 2);
	}
}