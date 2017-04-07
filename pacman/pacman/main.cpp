#include <iostream>
using namespace std;
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *pacman = NULL;

	float pacman_x = 10;
	float pacman_y = 210;

	int score = 0;
	int time = 1000;

	bool key[4] = { false, false, false, false };

	al_init();
	al_init_image_addon();

	al_install_keyboard();

	timer = al_create_timer(.02);

	display = al_create_display(480, 640);

	pacman = al_create_bitmap(32, 32);
	al_set_target_bitmap(pacman);
	al_clear_to_color(al_map_rgb(237, 200, 125));

	al_set_target_bitmap(al_get_backbuffer(display));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

}