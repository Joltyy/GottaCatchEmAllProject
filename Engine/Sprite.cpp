#include <allegro5/allegro.h>
#include <memory>

#include "UI/Component/Image.hpp"
#include "Engine/Point.hpp"
#include "Sprite.hpp"

namespace Engine {
	Sprite::Sprite(std::string img, float x, float y, float w, float h, float anchorX, float anchorY,
		float rotation, float vx, float vy, unsigned char r, unsigned char g, unsigned char b, unsigned char a) :
		Image(img, x, y, w, h, anchorX, anchorY), Rotation(rotation), Velocity(Point(vx, vy)),
		Tint(al_map_rgba(r, g, b, a)), isKnockback(false), isKnockbackturret(false) {
	}

	void Sprite::SetKnockback(bool knockback) {
        isKnockback = knockback;
    }

	void Sprite::SetKnockbackturret(bool knockback) {
        isKnockbackturret = knockback;
    }

	void Sprite::Draw() const {
		ALLEGRO_COLOR drawTint = Tint;
        if (isKnockback) {
			unsigned char r, g, b, a;
			// Extract the RGBA components from the Tint color
			al_unmap_rgba(Tint, &r, &g, &b, &a);
			// Adjust the alpha value to 128 for 50% transparency
			drawTint = al_map_rgba(r, g, b, a * 0.5);
		}

		if (isKnockbackturret) {
			unsigned char r, g, b, a;
			// Extract the RGBA components from the Tint color
			al_unmap_rgba(Tint, &r, &g, &b, &a);
			// Adjust the alpha value to 128 for 50% transparency
			drawTint = al_map_rgba(r, g, b, a * 0.4);
		}
		
		al_draw_tinted_scaled_rotated_bitmap(bmp.get(), Tint, Anchor.x * GetBitmapWidth(), Anchor.y * GetBitmapHeight(),
			Position.x, Position.y, Size.x / GetBitmapWidth(), Size.y / GetBitmapHeight(), Rotation, 0);
	}
	
	void Sprite::Update(float deltaTime) {
		Position.x += Velocity.x * deltaTime;
		Position.y += Velocity.y * deltaTime;
	}
};
