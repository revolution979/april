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



int main() {

	//create vector
	vector<int>nodes;
	vector<int>::const_iterator iter;
	ALLEGRO_BITMAP *snek = NULL;
	ALLEGRO_BITMAP *wall = NULL;
	ALLEGRO_BITMAP*food = NULL;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;

	int snek_x = 5;
	int snek_y = 5;

	int sneklength = 0;
	bool gotFood = false;

	const int SNEKSIZE = 16;

	int score = 0;

	float CLOCK = .4;

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

	while (true)
	{
		cout << snek_x << " , " << snek_y << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		////prints to console screen
		//for (int i = 0; i < 20; i++) {
		//	for (int j = 0; j < 20; j++)
		//		cout << map[i][j];
		//	cout << endl;
		//}
		if (ev.type == ALLEGRO_EVENT_TIMER) {

			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++) {
					if (map[i][j] == 1)
						map[i][j] = 0;
				}
			//snek tail
			for (int i = 0; i < sneklength; i = i + 2) {
				map[nodes[i]][nodes[i + 1]] = 1;
			}
			if (gotFood == true) {
				food_x = rand() % 20;
				food_y = rand() % 20;
				score++;
				CLOCK = CLOCK = .01;
				map[food_x][food_y] = 2;
				gotFood = false;
			}
			//	cout << "flag1" << endl;
			if (key[0] && snek_y > -1) {
				snek_y -= 1;
			}
			if (key[1] && snek_y > -1) {
				snek_y += 1;
			}
			if (key[2] && snek_x > -1) {
				snek_x -= 1;
			}
			if (key[3] && snek_x > -1) {
				snek_x += 1;
			}
			if (map[food_x] == map[snek_x] && map[food_y] == map[snek_y]) {
				gotFood = true;
				sneklength += 2;
			}
			//cout << "flag1.2" << endl;
			if (snek_x >= 0 &&
				snek_y >= 0 &&
				snek_x < 20 &&
				snek_y < 20)
				map[snek_x][snek_y] = 1;

			nodes.insert(nodes.begin(), snek_y);
			nodes.insert(nodes.begin(), snek_x);
			//cout << "flag1.3" << endl;

			if (snek_x < 0 || snek_y < 0)
				cout << "MATRIX ERROR" << endl;
			cout << "flag 1.4" << endl;
			//kill ekans if it hits the wall
			if (snek_x < 0 || snek_x >= 21 || snek_y < -1 || snek_y >= 21) {
				cout << "snek is dead, hit wall" << endl;
				cout << snek_x << " , " << snek_y << endl;
				al_clear_to_color(al_map_rgb(0, 0, 0));
				//al_draw_textf(font, al_map_rgb(251, 0, 0), 300, 300, NULL, "snek is ded.");
				al_flip_display();
				al_rest(2);
			}
			//	cout << "flag2" << endl;
			if (sneklength > 1) {
				for (iter = nodes.begin() + 2; iter < nodes.end(); iter += 2) {
					cout << "sneklength is " << sneklength << endl;
					cout << "head is" << snek_x << " ' " << snek_y << endl;
					cout << "node values are " << *iter << " , " << *(iter + 1) << endl;

					if ((snek_x == *iter && snek_y == *(iter + 1))) {
						cout << "you bumped into snek's tail, he is ded!" << endl;
						al_clear_to_color(al_map_rgb(0, 0, 0));
						//al_draw_textf(font, al_map_rgb(250, 0, 0), 300, 300, NULL, "snek is ded.");
						al_flip_display();
						al_rest(2);

						return 0;
					}
				}
			}
			nodes.erase(nodes.begin() + sneklength, nodes.end());

			redraw = true;
		}//end timer section
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_UP:
				key[0] = true;
				key[1] = false;
				key[2] = false;
				key[3] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				key[2] = false;
				key[0] = false;
				key[3] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				key[1] = false;
				key[0] = false;
				key[3] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				key[1] = false;
				key[2] = false;
				key[0] = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				//key[0] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				//key[1] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				//key[2] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				//key[3] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			}
		}
		//RENDER SECTION

		if (redraw && al_is_event_queue_empty(event_queue)) {
			cout << "flag3" << endl;
			redraw = false;
			al_clear_to_color(al_map_rgb(0, 0, 0));

			if (gotFood == false)
				map[food_x][food_y] = 2;
			cout << "flag 4" << endl;
			for (int i = 0; i < 20; i++)
				for (int j = 0; j < 20; j++) {

					if (map[i][j] == 1) {
						//al_draw_filled_rectangle(i*SNEKSIZE, j*SNEKSIZE, i*SNEKSIZE + SNEKSIZE, j*SNEKSIZE + SNEKSIZE, al_map_rgb(100, 250, 50));//snek
						al_draw_bitmap(snek, i*SNEKSIZE, j*SNEKSIZE, 0);
					}
					//if (map[i][j] == 0)
					//al_draw_rectangle(i*SNEKSIZE, j*SNEKSIZE, i*SNEKSIZE + SNEKSIZE, j*SNEKSIZE + SNEKSIZE, al_map_rgb(150, 0, 0), 4);//grid lines
					if (map[i][j] == 2) {
						al_draw_bitmap(food, i*SNEKSIZE, j*SNEKSIZE, 0);
						CLOCK = CLOCK - .1;
						//al_draw_filled_rectangle(i*SNEKSIZE, j*SNEKSIZE, i*SNEKSIZE + SNEKSIZE, j*SNEKSIZE + SNEKSIZE, al_map_rgb(150, 50, 150));//food
					}
				}
			cout << "flag 5" << endl;
			//al_draw_textf(font, al_map_rgb(20, 20, 255), 375, 9, ALLEGRO_ALIGN_CENTRE, "score = %i", score);

			if (gotFood == true)
				map[food_x][food_y] = 0;


			//al_draw_bitmap(snek, snek_x, snek_y, 0);

			al_flip_display();
		}
	}
	//al_destroy_sample(sample);
	//al_destroy_sample_instance(instance1);
	al_destroy_bitmap(snek);
	al_destroy_display(display);
	al_destroy_timer(timer);
	al_destroy_event_queue(event_queue);

	return 0;
}
