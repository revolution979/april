#include <iostream>
using namespace std;
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_acodec.h"
#include "allegro5\allegro_primitives.h"
#include <time.h>
#include <vector>

const int SNEKSIZE = 16;
double CLOCK = .1;

int main() {

	//create vector
	vector<int>nodes;
	vector<int>::const_iterator iter;
	ALLEGRO_BITMAP *snek = NULL;
	ALLEGRO_BITMAP *wall = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	int snek_x = 5;
	int snek_y = 5;

	int sneklength = 0;
	bool gotFood = false;

	int food_x = 5;
	int food_y = 5;

	int map[20][20];
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			map[20][20] = 0;

	map[food_x][food_y] = 2;

	srand(time(NULL));

	bool key[4] = { false, false, false, false };

	bool redraw = true;

	bool doexit = false;

	al_init();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_install_audio();
	al_init_acodec_addon();
	al_init_primitives_addon();

	al_install_keyboard();

	timer = al_create_timer(CLOCK);
	display = al_create_display(800, 800);

	wall = al_create_bitmap(20, 20);
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(120, 0, 120));

	snek = al_create_bitmap(SNEKSIZE, SNEKSIZE);
	al_set_target_bitmap(snek);
	al_clear_to_color(al_map_rgb(255, 255, 0));

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	while (!doexit)
	{

		cout << snek_x << ", " << snek_y << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			//wipe map
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++) {
					if (map[i][j] == 1)
						map[i][j] = 0;
				}

			//draw trail
			for (int i = 0; i < sneklength; i = i + 2) {
				map[nodes[i]][nodes[i + 1]] = 1;
				cout << "snek trail is at " << nodes[i] << ", " << nodes[i + 1] << endl;
			}
			cout << "snekleght is " << sneklength << endl;

			//check if goal has been reached, set new goal is true
			if (gotFood == true) {

				food_x = rand() % 20;
				food_y = rand() % 20;
				map[food_x][food_y] = 2;          
				//score = score++;
				gotFood = false;

			}
			//flag 1
			if (key[0] && snek_y > -1) {
				snek_y -= 1;
			}
			if (key[1] && snek_y > -1) {
				snek_y += 1;
			}
			if (key[2] && snek_y > -1) {
				snek_y -= 1;
			}
			if (key[3] && snek_y > -1) {
				snek_y += 1;
			}
			//check if player has gotten goal
			if (map[food_x] == map[snek_x] && map[food_y] == map[snek_y]) {
				gotFood = true;
				sneklength += 2;
			}

			if (snek_x > 0 &&
				snek_y > 0 &&
				snek_x < 20 &&
				snek_y < 20)
				map[snek_x][snek_y] = 1;
			
			nodes.insert(nodes.begin(), snek_y);
			nodes.insert(nodes.begin(), snek_x);
			
			if (snek_x < 0 || snek_y < 0)
				cout << "matrix error" << endl;
			//flag2

			
			//kill snek if it hits the wall
			if (snek_x < 0 || snek_x >= 21 || snek_y < -1 || snek_y < 21) {
				cout << "snek is dead, hit wall" << endl;
			}



		}

		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (gotFood == false)
				map[food_x][food_y] = 2;

			for (int i = 0; i < 20; i++) {
				for (int j = 0; j < 20; j++) {
					if (map[i][j] == 1)
						al_draw_bitmap(wall, i , j , NULL);
					
				}
			}

		}

	}



	for (int x = 0; x < 20; x++) {
		for (int y = 0; y < 20; y++) {
			cout << map[x][y];
		}
		cout << endl;
	}

}