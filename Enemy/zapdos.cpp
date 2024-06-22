#include "zapdos.hpp"
#include <iostream>

ALLEGRO_BITMAP* zapdos_left;
ALLEGRO_BITMAP* zapdos_right;
ALLEGRO_BITMAP* zapdos_up;
ALLEGRO_BITMAP* zapdos_down;
ALLEGRO_BITMAP* zapdos_attk_down;
ALLEGRO_BITMAP* zapdos_attk_left;
ALLEGRO_BITMAP* zapdos_attk_right;
ALLEGRO_BITMAP* zapdos_attk_up;

zapdos::zapdos(int x, int y)
    : Enemy("Animations/zapdos_down.png", x, y, 20, 20, 200, 10, 20, 200, 40), frameCount(0) {
    // Add the paths of your frame images to the frames vector.
    zapdos_right = al_load_bitmap("Resource/images/Animations/zapdos_right.png");
    zapdos_left = al_load_bitmap("Resource/images/Animations/zapdos_left.png");
    zapdos_down = al_load_bitmap("Resource/images/Animations/zapdos_down.png");
    zapdos_up = al_load_bitmap("Resource/images/Animations/zapdos_up.png");
    // zapdos_attk_down = al_load_bitmap("Resource/images/Animations/zapdos_attack_down.png");
    // zapdos_attk_right = al_load_bitmap("Resource/images/Animations/zapdos_attack_right.png");
    // zapdos_attk_left = al_load_bitmap("Resource/images/Animations/zapdos_attack_left.png");
    // zapdos_attk_up = al_load_bitmap("Resource/images/Animations/zapdos_attack_up.png");
}

void zapdos::Update(float deltaTime) {
    static float frameTimer = 0;
    frameTimer += deltaTime;
    if (frameTimer >= 0.2f) { // 1 second delay
        frameTimer = 0;
        if (++frameCount >= 3) { // Assuming 2 frames
            frameCount = 0;
        }
    }
    if (Velocity.x > 0) direction = 0; // Moving right
    else if (Velocity.x < 0) direction = 1; // Moving left
    else if (Velocity.y < 0) direction = 2; // Moving up
    else if (Velocity.y > 0) direction = 3; // Moving down

    // // Check if the boss is being knockbacked.
    // if (boss_Knockback) {
    //     knockbackTimer -= deltaTime;
    //     if (knockbackTimer <= 0) {
    //         // Reset the knockback state after the timer runs out.
    //         boss_Knockback = false;
    //     }
    // }

    Enemy::Update(deltaTime);
}

void zapdos::Draw() const {
    // Calculate the source position of the current frame
    int frameWidth = 45;
    int frameHeight = 50;
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
        // if(boss_Knockback) al_draw_scaled_bitmap(zapdos_attk_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        al_draw_scaled_bitmap(zapdos_right, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    if(direction == 1)
    {
        // if (boss_Knockback) al_draw_scaled_bitmap(zapdos_attk_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        al_draw_scaled_bitmap(zapdos_left, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 2)
    {
        // if(boss_Knockback) al_draw_scaled_bitmap(zapdos_attk_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        al_draw_scaled_bitmap(zapdos_up, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    else if(direction == 3)
    {
        // if(boss_Knockback) al_draw_scaled_bitmap(zapdos_attk_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
        al_draw_scaled_bitmap(zapdos_down, sourceX, sourceY, frameWidth, frameHeight, newPositionX, newPositionY, scaledWidth, scaledHeight, 0);
    }
    

    if (PlayScene::DebugMode) {
		// Draw collision radius.
		al_draw_circle(Position.x, Position.y, CollisionRadius, al_map_rgb(255, 0, 0), 2);
        al_draw_circle(Position.x, Position.y, hitbox, al_map_rgb(0, 0, 100), 2);
	}
}