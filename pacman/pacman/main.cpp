#include <iostream>
using namespace std;
#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\allegro_font.h"
#include "allegro5\allegro_ttf.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_acodec.h"
#include "allegro5\allegro_primitives.h"


const int RIGHT = 3;
const int LEFT = 1;
const int UP = 2;
const int DOWN = 4;

const int PACSIZE = 16;

int wallCollide(int pacman_x, int pacman_y, int dir, int map[33][28]);

class ghost {

public:
	ghost();
	void initghost(int x, int y, char color, int directon, bool dead, int speed);
	bool isdead();
	void drawghost(char c);
	void printinfo();
	void chase(int x, int y, int map[33][28]);
	bool getpacman(int xpos, int ypos);
	void setposition(int x, int y);

private:
	int hitwall;
	int xpos;
	int ypos;
	int xpos2;
	int ypos2;
	int xpos3;
	int ypos3;
	char color;
	int direction;
	bool dead;
	int speed;
};

int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *pacman = NULL;
	ALLEGRO_BITMAP *wall = NULL;
	ALLEGRO_BITMAP *pellet = NULL;
	ALLEGRO_BITMAP *pp = NULL;
	ALLEGRO_FONT *font = NULL;
	ALLEGRO_FONT *font2 = NULL;

	int pacman_x = 272;
	int pacman_y = 482;

	int score = 0;
	int time = 1000;
	int lives = 3;

	int map[33][28] = {
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
		1,3,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,3,1,
		1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,0,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,1,1,1,2,1,1,2,1,1,1,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,1,1,1,2,2,1,1,1,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,1,2,2,2,2,2,2,1,2,1,1,0,1,1,1,1,1,1,
		2,2,2,2,2,2,0,2,2,2,1,2,2,2,2,2,2,1,2,2,2,0,2,2,2,2,2,2,
		1,1,1,1,1,1,0,1,1,2,1,2,2,2,2,2,2,1,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,2,2,2,2,2,2,2,2,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,
		1,1,1,1,1,1,0,1,1,2,1,1,1,1,1,1,1,1,2,1,1,0,1,1,1,1,1,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
		1,0,1,1,1,1,0,1,1,1,1,1,0,1,1,0,1,1,1,1,1,0,1,1,1,1,0,1,
		1,3,0,0,1,1,0,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,1,1,0,0,3,1,
		1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,
		1,1,1,0,1,1,0,1,1,0,1,1,1,1,1,1,1,1,0,1,1,0,1,1,0,1,1,1,
		1,0,0,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1,
		1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
		1,0,1,1,1,1,1,1,1,1,1,1,0,1,1,0,1,1,1,1,1,1,1,1,1,1,0,1,
		1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
		2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,
	};

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

	font = al_load_ttf_font("cool.ttf", 20, 0);
	font2 = al_load_ttf_font("no.ttf", 20, 0);

	timer = al_create_timer(.04);

	display = al_create_display(560, 660);

	pacman = al_create_bitmap(16, 16);
	al_set_target_bitmap(pacman);
	al_clear_to_color(al_map_rgb(255, 255, 0));


	wall = al_create_bitmap(20, 20);
	al_set_target_bitmap(wall);
	al_clear_to_color(al_map_rgb(120, 0, 120));

	ghost blinky;
	blinky.initghost(224, 320, 'b', 10, false, 10);

	ghost inky;
	inky.initghost(246, 320, 'i', 10, false, 10);

	ghost pinky;
	pinky.initghost(22, 320, 'p', 10, false, 10);

	ghost clyde;
	clyde.initghost(22, 320, 'c', 10, false, 10);

	/*pellet = al_create_bitmap(6, 6);
	al_set_target_bitmap(pellet);
	al_clear_to_color(al_map_rgb(255, 255, 255));*/

	pellet = al_load_bitmap("potato.png");

	pp = al_load_bitmap("Smashball.png");

	al_set_target_bitmap(al_get_backbuffer(display));

	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);

	while (!doexit || score)
	{
		cout << pacman_x << ", " << pacman_y << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//////////////////////////////////////////////////////////////////////////////////////////////////
		//here's the movement algorithm

		if (ev.type == ALLEGRO_EVENT_TIMER) {

			if (lives == 0) {

				//al_draw_textf(font, al_map_rgb(255, 255, 255), 272, 302, ALLEGRO_ALIGN_CENTRE, "you lose");
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				al_draw_textf(font, al_map_rgb(255, 255, 255), 272, 302, ALLEGRO_ALIGN_CENTRE, "you lose");
				al_flip_display();
				al_rest(5.0);
				return 0;
			}
			
			if (score == 2620) {

				//al_draw_textf(font, al_map_rgb(255, 255, 255), 272, 302, ALLEGRO_ALIGN_CENTRE, "you lose");
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				al_draw_textf(font, al_map_rgb(255, 255, 255), 272, 302, ALLEGRO_ALIGN_CENTRE, "you win!!");
				al_flip_display();
				al_rest(5.0);
				return 0;
			}

			

			//if the up button is pressed AND we're still below the top wall,
			//move the box "up" by 4 pixels
			if (key[0] && wallCollide(pacman_x, pacman_y, UP, map) == 0) {
				pacman_y -= 3.0;
			}

			/*if (key2[0] && pacman2_y >= 0) {
				pacman2_y -= 4.0;
			}*/
			//if the down button is pressed AND we're still above the bottom wall
			//move the box "down" by 4 pixels
			if (key[1] && wallCollide(pacman_x, pacman_y, DOWN, map) == 0) {
				pacman_y += 3.0;
			}

			/*if (key2[1] && pacman2_y <= 480 - 75) {
				pacman2_y += 4.0;
			}*/
			//if the left button is pressed AND we're still right of the left wall
			//move the box left by 4 pixels
			if (key[2] && wallCollide(pacman_x, pacman_y, LEFT, map) == 0) {
				pacman_x -= 3.0;
			}

			/*if (key2[2] && pacman2_x >= 0) {
				pacman2_x -= 4.0;
			}*/
			//if the right button is pressed AND we're still left of the right wall
			//move the box right by 4 pixels
			if (key[3] && wallCollide(pacman_x, pacman_y, RIGHT, map) == 0) {
				pacman_x +=  3.0;
			}
			/////warp zones
			//left
			/*if (pacman_x > -2 && pacman_x < 4 == true && pacman_y > 296 && pacman_y < 324 == true) {
				pacman_x = 543;
				pacman_y = 302;

			}*/
			//right
			if (pacman_x > 541 && pacman_x < 547 && pacman_y > 301 && pacman_y < 303) {
				pacman_x = 1;
				pacman_y = 302;
			}
			/////dots eaten
			if(map[(pacman_y + 6) / 20][(pacman_x + 6) / 20] == 0){

				map[(pacman_y + 6) / 20][(pacman_x + 6) / 20] = 2;
				score = score + 10;

			}
			/////dots eaten
			
			blinky.chase(pacman_x, pacman_y, map);
			inky.chase(pacman_x + 45, pacman_y / 2, map);
			pinky.chase(pacman_x + 16, pacman_y + 16, map);
			clyde.chase(pacman_x / 2, pacman_y, map);

			if (blinky.getpacman(pacman_x, pacman_y) == 1 || inky.getpacman(pacman_x, pacman_y) == 1 || pinky.getpacman(pacman_x, pacman_y) == 1 || clyde.getpacman(pacman_x, pacman_y) == 1) {

				pacman_x = 272;
				pacman_y = 482;
				blinky.setposition(224, 320);
				inky.setposition(224, 320);
				lives--;


			}
			if (inky.getpacman(pacman_x, pacman_y) == 1) {

				pacman_x = 272;
				pacman_y = 482;
				inky.setposition(246, 320);
				lives--;


			}
			if (pinky.getpacman(pacman_x, pacman_y) == 1) {

				pacman_x = 272;
				pacman_y = 482;
				pinky.setposition(224, 320);
				lives--;


			}
			if (clyde.getpacman(pacman_x, pacman_y) == 1) {

				pacman_x = 272;
				pacman_y = 482;
				clyde.setposition(224, 320);
				lives--;


			}

			if (map[(pacman_y + 6) / 20][(pacman_x + 6) / 20] == 3) {

				map[(pacman_y + 6) / 20][(pacman_x + 6) / 20] = 2;
				score = score + 50;
				//lives = lives - 1;
			}
			/*if (key2[3] && pacman2_x <= 640 - 32) {
				pacman2_x += 4.0;
			}*/
			if (pacman_x > -40 && pacman_x < 0 && pacman_y > 0 && pacman_y < 800) {
				pacman_x = 543;
				pacman_y = 302;
			}
			redraw = true;
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {

			//"keycode" holds all the different keys on the keyboard
			switch (ev.keyboard.keycode) {

				//if the up key has been pressed
			case ALLEGRO_KEY_UP:
				key[0] = true;
				break;

				//if the down key has been pressed
			case ALLEGRO_KEY_DOWN:
				key[1] = true;
				break;

				//if the left key has been pressed

			case ALLEGRO_KEY_LEFT:
				key[2] = true;
				break;

				//if the right key has been pressed
			case ALLEGRO_KEY_RIGHT:
				key[3] = true;
				break;

				//if the left key has been pressed
			}
		}
		//has something been released on the keyboard?
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				/*if (key[0] && wallCollide(pacman_x, pacman_y, UP, map) == 0) {
					pacman_y -= 3.0;
					*/key[0] = false;
				//}
				
				break;

			case ALLEGRO_KEY_DOWN:
				/*if (key[1] && wallCollide(pacman_x, pacman_y, DOWN, map) == 0) {
					pacman_y += 3.0;
					*/key[1] = false;
				//}
				
				break;

				//kill the program if someone presses escape
			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;

			case ALLEGRO_KEY_LEFT:
				/*if (key[2] && wallCollide(pacman_x, pacman_y, LEFT, map) == 0) {
					pacman_x -= 3.0;
					*/key[2] = false;
				//}
				
				break;

			case ALLEGRO_KEY_RIGHT:
				/*if (key[3] && wallCollide(pacman_x, pacman_y, RIGHT, map) == 0) {
					pacman_x += 3.0;
					*/key[3] = false;
				//}
				
				break;
			}
		}
		//RENDER SECTION
		//if the clock ticked but no other events happened, don't bother redrawing
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			//paint black over the old screen, so the old square dissapears
			//al_clear_to_color(al_map_rgb(0, 0, 0));
			al_clear_to_color(al_map_rgb(0, 0, 0));

			for (int x = 0; x < 33; x++) {
				for (int y = 0; y < 28; y++) {
					if (map[x][y] == 1)
						al_draw_bitmap(wall, y * 20, x * 20, NULL);
					if (map[x][y] == 0)
						al_draw_bitmap(pellet, y * 20 + 6, x * 20 + 6, NULL);
					if (map[x][y] == 3)
						al_draw_bitmap(pp, y * 20, x * 20, NULL);
				}
			}

			blinky.drawghost('b');
			inky.drawghost('i');
			pinky.drawghost('p');
			clyde.drawghost('c');
			//the algorithm above just changes the x and y coordinates
			//here's where the bitmap is actually drawn somewhere else
			//al_draw_bitmap(ball, ball_x, ball_y, 0);
			al_draw_bitmap(pacman, pacman_x, pacman_y, 0);
			
			al_draw_textf(font2, al_map_rgb(255, 255, 255), 80, 0, ALLEGRO_ALIGN_CENTRE, "lives = %i", lives);
			al_draw_textf(font2, al_map_rgb(255, 255, 255), 470, 0, ALLEGRO_ALIGN_CENTRE, "score = %i", score);
			//al_draw_bitmap(pacman2, pacman2_x, pacman2_y, 0);
			al_flip_display();
		}
	}




	for (int x = 0; x < 33; x++) {
		for (int y = 0; y < 28; y++) {
			cout << map[x][y];
		}
		cout << endl;
	}

	al_start_timer(timer);

}
int wallCollide(int pacman_x, int pacman_y, int dir, int map[33][28]) {
	cout << "collision" << endl;
	int new_x1;
	int new_x2;
	int new_x3;
	int new_y1;
	int new_y2;
	int new_y3;
	if (dir == RIGHT) {         // Moving Right
								// Check along the far right side of the sprite, plus 3 (the amount we’re moving)
		new_x1 = pacman_x + 3 + PACSIZE;
		new_x2 = pacman_x + 3 + PACSIZE;
		new_x3 = pacman_x + 3 + PACSIZE;
		//Check at three point along that edge
		new_y1 = pacman_y;
		new_y2 = pacman_y + PACSIZE / 2;
		new_y3 = pacman_y + PACSIZE;
		cout << "collison2" << endl;
		if (map[new_y1 / 20][new_x1 / 20] == 1 || map[new_y2 / 20][new_x2 / 20] == 1 || map[new_y3 / 20][new_x3 / 20] == 1)//don't forget x3
			return 1; //collision!
	}
	if (dir == LEFT) {
		new_x1 = pacman_x - 3;
		new_x2 = pacman_x + 3 / 2;
		new_x3 = pacman_x;

		new_y1 = pacman_y;
		new_y2 = pacman_y + 3 / 2;
		new_y3 = pacman_y + PACSIZE;
		cout << "collision3" << endl;
		if (map[new_y1 / 20][new_x1 / 20] == 1 || map[new_y2 / 20][new_x2 / 20] == 1 || map[new_y3 / 20][new_x3 / 20] == 1)//don't forget x3
			return 1;
	}
	if (dir == UP) {
		new_x1 = pacman_x;
		new_x2 = pacman_x + PACSIZE / 2;
		new_x3 = pacman_x + PACSIZE;

		new_y1 = pacman_y - 3;
		new_y2 = pacman_y - 3;
		new_y3 = pacman_y - 3;

		if (map[new_y1 / 20][new_x1 / 20] == 1 || map[new_y2 / 20][new_x2 / 20] == 1 || map[new_y3 / 20][new_x3 / 20] == 1)//don't forget x3
			return 1;
	}
	if (dir == DOWN) {
		new_x1 = pacman_x;
		new_x2 = pacman_x + PACSIZE;
		new_x3 = pacman_x + PACSIZE / 2;

		new_y1 = pacman_y + PACSIZE + 3;
		new_y2 = pacman_y + PACSIZE + 3;
		new_y3 = pacman_y + PACSIZE + 3;

		if (map[new_y1 / 20][new_x1 / 20] == 1 || map[new_y2 / 20][new_x2 / 20] == 1 || map[new_y3 / 20][new_x3 / 20] == 1)//don't forget x3
			return 1;
	}
	return 0;
}
ghost::ghost() {

	xpos = 0;
	ypos = 0;
	xpos2 = 0;
	ypos2 = 0;
	xpos3 = 0;
	ypos3 = 0;
	dead = 0;

}
void ghost::initghost(int x, int y, char c, int directon, bool dead, int speed) {

	xpos = x;
	ypos = y;
	xpos2 = x;
	ypos2 = y;
	xpos3 = x;
	ypos3 = y;
	dead = 0;
	color = c;
}
bool ghost::isdead(){

	return dead;

}
void ghost::drawghost(char c){

	/*if (char color = 'r') {
		al_draw_filled_rectangle(xpos, ypos, xpos + 25, ypos + 25, al_map_rgb(252, 59, 16));
	}*/
	if (c == 'b') {
		al_draw_filled_rectangle(xpos, ypos, xpos + 16, ypos + 16, al_map_rgb(252, 59, 16));
	}
	if (c == 'p') {
		al_draw_filled_rectangle(xpos, ypos, xpos + 16, ypos + 16, al_map_rgb(253, 159, 216));
	}
	if (c == 'c') {
		al_draw_filled_rectangle(xpos, ypos, xpos + 16, ypos + 16, al_map_rgb(252, 159, 16));
	}
	if (c == 'i') {
		al_draw_filled_rectangle(xpos, ypos, xpos + 16, ypos + 16, al_map_rgb(74, 223, 203));
	}

}
void ghost::printinfo(){



}
void ghost::chase(int x, int y, int map[33][28]){


	/* Directions
	1 = left
	2 = up
	3 = right
	4 = down*/

	//this is just for testing
	/*  cout << "state is " << direction << endl;
	if (wallCollide(xpos, ypos, 1, field))
	cout << "there's a wall to the left of me" << endl;
	if (wallCollide(xpos, ypos, 2, field))
	cout << "there's a wall above me" << endl;
	if (wallCollide(xpos, ypos, 3, field))
	cout << "there's a wall to the right of me" << endl;
	if (wallCollide(xpos, ypos, 4, field))
	cout << "there's a wall below me" << endl;*/

	/////////////////////////////////LEFT STATE (1)//////////////////////////////////////////////////////////////////

	//if we're moving left and there's an opening above and pacman is above, move up
	if ((direction == LEFT) && !wallCollide(xpos, ypos, UP, map) && y<ypos)
		while (!wallCollide(xpos, ypos, 2, map)) {
			// cout << "trying to move through hole above!" << endl;
			direction = UP;
			ypos -= 4;
			return;
		}

	//if we're moving left and there's an opening below and pacman is below, move down
	if ((direction == LEFT) && !wallCollide(xpos, ypos, DOWN, map) && y>ypos)
		while (!wallCollide(xpos, ypos, 4, map)) {
			//  cout << "trying to move through hole below!" << endl;
			direction = DOWN;
			ypos += 4;
			return;
		}


	if (direction == LEFT)//left
		while (!wallCollide(xpos, ypos, LEFT, map)) {
			xpos -= 4;
			return;
		}

	////////////////////////////////UP STATE (2)///////////////////////////////////////////////////////////////

	//if we're moving up and there's an opening left and pacman is left, move left
	if ((direction == UP) && !wallCollide(xpos, ypos, LEFT, map) && x<xpos)
		while (!wallCollide(xpos, ypos, LEFT, map)) {
			//   cout << "trying to move through hole to left!" << endl;
			direction = LEFT;
			xpos -= 4;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((direction == UP) && !wallCollide(xpos, ypos, RIGHT, map) && x>xpos)
		while (!wallCollide(xpos, ypos, 3, map)) {
			// cout << "trying to move through hole to right!" << endl;
			direction = RIGHT;
			xpos += 4;
			return;
		}
	if (direction == UP)//up
		while (!wallCollide(xpos, ypos, 2, map)) {
			ypos -= UP;
			return;
		}

	/////////RIGHT CASE (3)/////////////////////////////////////////////////////////////////////////////////////////////////////
	//if we're moving right and there's an opening above and pacman is above, move up
	if ((direction == RIGHT) && !wallCollide(xpos, ypos, UP, map) && y<ypos)
		while (!wallCollide(xpos, ypos, 2, map)) {
			//   cout << "trying to move through hole above!" << endl;
			direction = UP;
			ypos -= 2;
			return;
		}

	//if we're moving right and there's an opening below and pacman is below, move down
	if ((direction == RIGHT) && !wallCollide(xpos, ypos, DOWN, map) && y>ypos)
		while (!wallCollide(xpos, ypos, DOWN, map)) {
			//  cout << "trying to move through hole below!" << endl;
			direction = DOWN;
			ypos += 2;
			return;
		}


	if (direction == RIGHT)//right
		while (!wallCollide(xpos, ypos, RIGHT, map)) {
			xpos += 2;
			return;
		}


	//////////////DOWN CASE (4)/////////////////////////////////////////////////////////////////////////////////////
	//if we're moving up and there's an opening left and pacman is left, move left
	if ((direction == DOWN) && !wallCollide(xpos, ypos, LEFT, map) && x<xpos)
		while (!wallCollide(xpos, ypos, LEFT, map)) {
			// cout << "trying to move through hole to left!" << endl;
			direction = LEFT;
			xpos -= 2;
			return;
		}
	//if we're moving up and there's an opening right and pacman is right, move right
	if ((direction == UP) && !wallCollide(xpos, ypos, RIGHT, map) && x>xpos)
		while (!wallCollide(xpos, ypos, RIGHT, map)) {
			//   cout << "trying to move through hole to right!" << endl;
			direction = RIGHT;
			xpos += 2;
			return;
		}

	if (direction == DOWN)//down
		while (!wallCollide(xpos, ypos, DOWN, map)) {
			ypos += 2;
			return;
		}





	///////////////////////////////////////////////////////////////////////////////////////
	//if pacman is above and there's no wall there, move up
	if ((y<ypos) && !wallCollide(xpos, ypos, UP, map)) {
		//  cout << "direction is up" << endl;
		direction = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if ((y>ypos) && !wallCollide(xpos, ypos, DOWN, map)) {
		direction = DOWN;
		//   cout << "direction is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if ((x>xpos) && !wallCollide(xpos, ypos, RIGHT, map)) {
		direction = RIGHT;
		//  cout << "direction is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if ((x<xpos) && !wallCollide(xpos, ypos, LEFT, map)) {
		direction = LEFT;
		// cout << "direction is left" << endl;
		return;

	}

	//if pacman is above and there's no wall there, move up
	if (!wallCollide(xpos, ypos, UP, map)) {
		// cout << "direction is up" << endl;
		direction = UP;
		return;
	}


	//if pacman is below and there's no wall there, move down
	if (!wallCollide(xpos, ypos, DOWN, map)) {
		direction = DOWN;
		//cout << "direction is down" << endl;
		return;
	}

	//if pacman is right and there's no wall there, move right
	if (!wallCollide(xpos, ypos, RIGHT, map)) {
		direction = RIGHT;
		//  cout << "direction is right" << endl;
		return;
	}

	//if pacman is left and there's no wall there, move left
	if (!wallCollide(xpos, ypos, LEFT, map)) {
		direction = LEFT;
		// cout << "direction is left" << endl;
		return;

	}

}

bool ghost::getpacman(int pacx, int pacy){

	if (xpos + 16 > pacx && xpos < pacx + PACSIZE && ypos + 16 > pacy && ypos < pacy + PACSIZE) {
		dead = true;
		return 1;
	}

}
void ghost::setposition(int x, int y) {

	xpos = x;
	ypos = y;

}