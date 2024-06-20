#include "boss1.hpp"
#include <iostream>

ALLEGRO_BITMAP* boss1_left;
ALLEGRO_BITMAP* boss1_right;
ALLEGRO_BITMAP* boss1_up;
ALLEGRO_BITMAP* boss1_down;
ALLEGRO_BITMAP* boss1_attk_down;
ALLEGRO_BITMAP* boss1_attk_left;
ALLEGRO_BITMAP* boss1_attk_right;
ALLEGRO_BITMAP* boss1_attk_up;

boss1::boss1(int x, int y)
    : Enemy("Animations/giratina_right.png", x, y, 20, 20, 200, 10, 20, 200), frameCount(0) {
    // Add the paths of your frame images to the frames vector.
    boss1_right = al_load_bitmap("Resource/images/Animations/giratina_right.png");
    boss1_left = al_load_bitmap("Resource/images/Animations/giratina_left.png");
    boss1_down = al_load_bitmap("Resource/images/Animations/giratina_down.png");
    boss1_up = al_load_bitmap("Resource/images/Animations/giratina_up.png");
    boss1_attk_down = al_load_bitmap("Resource/images/Animations/giratina_attack_down.png");
    boss1_attk_right = al_load_bitmap("Resource/images/Animations/giratina_attack_right.png");
    boss1_attk_left = al_load_bitmap("Resource/images/Animations/giratina_attack_left.png");
    boss1_attk_up = al_load_bitmap("Resource/images/Animations/giratina_attack_up.png");
}
// boss1::~boss1()
// {
//     al_destroy_bitmap(boss1_attk_down);
//     al_destroy_bitmap(boss1_attk_up);
//     al_destroy_bitmap(boss1_attk_left);
//     al_destroy_bitmap(boss1_attk_right);
//     al_destroy_bitmap(boss1_up);
//     al_destroy_bitmap(boss1_down);
//     al_destroy_bitmap(boss1_left);
//     al_destroy_bitmap(boss1_right);
// }

void boss1::Update(float deltaTime) {
    static float frameTimer = 0;
    frameTimer += deltaTime;
    if (frameTimer >= 0.5f) { // 1 second delay
        frameTimer = 0;
        if (++frameCount >= 2) { // Assuming 2 frames
            frameCount = 0;
        }
    }
    if (Velocity.x > 0) direction = 0; // Moving right
    else if (Velocity.x < 0) direction = 1; // Moving left
    else if (Velocity.y < 0) direction = 2; // Moving up
    else if (Velocity.y > 0) direction = 3; // Moving down

    // Check if the boss is being knockbacked.
    if (boss_Knockback) {
        knockbackTimer -= deltaTime;
        if (knockbackTimer <= 0) {
            // Reset the knockback state after the timer runs out.
            boss_Knockback = false;
        }
    }

    Enemy::Update(deltaTime);
}

void boss1::Draw() const {
    // Calculate the source position of the current frame
    int frameWidth = 64;
    int frameHeight = 64;
    int sourceX = frameCount * frameWidth;
    int sourceY = 0; // Assuming all frames are on the same row

    // Define the scale factor
    float scaleFactor = 3.0f; // Change this value to scale the boss

    // Calculate the new position based on the scale factor
    float scaledWidth = frameWidth * scaleFactor;
    float scaledHeight = frameHeight * scaleFactor;
    float newPositionX = Position.x - (scaledWidth - frameWidth) / 2 - 36;
    float newPositionY = Position.y - (scaledHeight - frameHeight) / 2 - 50;

    // Draw the current frame
    if(direction == 0)
    {
        if(boss_Knockback) al_draw_scaled_bitmap(boss1_attk_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        else al_draw_scaled_bitmap(boss1_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    if(direction == 1)
    {
        if (boss_Knockback) al_draw_scaled_bitmap(boss1_attk_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        else al_draw_scaled_bitmap(boss1_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 2)
    {
        if(boss_Knockback) al_draw_scaled_bitmap(boss1_attk_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        else al_draw_scaled_bitmap(boss1_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 3)
    {
        if(boss_Knockback) al_draw_scaled_bitmap(boss1_attk_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        else al_draw_scaled_bitmap(boss1_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    

    if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);

	}
}