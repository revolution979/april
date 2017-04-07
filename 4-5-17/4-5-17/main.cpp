#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"

int main() {

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP *potatur = NULL;
	al_init();
	al_init_image_addon();
	potatur = al_load_bitmap("potato.png");
	display = al_create_display(640, 480);
	al_set_window_title(display, "turtles are magical ponies!!");
	al_set_display_icon(display, potatur);
	al_flip_display();
	al_rest(6);
	al_destroy_display(display);

}