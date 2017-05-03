#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_primitives.h>
//function to detect collision
//paramaters are x & y coordinates of top left corner of first box, then its width and height
//then same for other box
int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h);

class Brick {
private:
	int width, height;
	int xPos, yPos;
	bool dead;
public:
	void set_values(int x, int y, int w, int h);
	void drawBrick();
	bool isDead();//checks if something is dead
	void killBrick();//kills brick
};




//using constants in this game so you have sensible words, not unknown numbers in code
const float FPS = 60;
const int SCREEN_W = 980;
const int SCREEN_H = 800;
const int BOUNCER_W = 200;
const int BOUNCER_H = 30;
//an enumeration is like a user-defined variable, with all the given values it can hold
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
enum MYKEYS2 {
	KEY_W, KEY_S, KEY_A, KEY_D
};


int main()
{
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *bouncer = NULL;
	int color = 1;
	int lives = 3;
	int speed = 12;
	bool music = true;
	float bouncer_x = 0;
	float bouncer_y = 770;
	bool key[4] = { false, false, false, false };
	ALLEGRO_BITMAP *bouncer3 = NULL;
	ALLEGRO_BITMAP *bouncer4 = NULL;
	float bouncer3_x = 400;
	float bouncer3_y = 200;
	float bouncer3_dx = -3.0, bouncer3_dy = 3.0;
	float bouncer4_x = 100;
	float bouncer4_y = 200;
	float bouncer4_dx = -3.0, bouncer4_dy = 3.0;
	bool redraw = true;
	bool doexit = false;

	//including some error messages to help debug
	al_init();
	al_init_primitives_addon();
	al_install_keyboard();
	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(SCREEN_W, SCREEN_H);
	event_queue = al_create_event_queue();


	//hook up fonts for on-screen text
	al_init_font_addon();
	al_init_ttf_addon();
	ALLEGRO_FONT *font1 = al_load_ttf_font("barq.ttf", 100, 0);
	ALLEGRO_FONT *font2 = al_load_ttf_font("barq.ttf", 30, 0);
	ALLEGRO_FONT *font3 = al_load_ttf_font("Butcherman-Regular.ttf", 72, 0);

	//set up the two squares
	bouncer = al_create_bitmap(BOUNCER_W, BOUNCER_H);

	bouncer3 = al_create_bitmap(50, 50);

	al_set_target_bitmap(bouncer);

	al_clear_to_color(al_map_rgb(255, 0, 255));

	al_set_target_bitmap(bouncer3);

	al_clear_to_color(al_map_rgb(255, 255, 255));

	bouncer4 = al_create_bitmap(50, 50);

	al_set_target_bitmap(bouncer4);

	al_clear_to_color(al_map_rgb(210, 255, 255));

	al_set_target_bitmap(al_get_backbuffer(display));

	//set up your event queue
	event_queue = al_create_event_queue();

	//audio stuff

	ALLEGRO_SAMPLE *sample1 = NULL; //variable to hold the audio file
	ALLEGRO_SAMPLE *sample2 = NULL;
	ALLEGRO_SAMPLE *sample3 = NULL;
	ALLEGRO_SAMPLE *sample4 = NULL;
	ALLEGRO_SAMPLE *sample5 = NULL;
	ALLEGRO_SAMPLE *sample6 = NULL;
	ALLEGRO_SAMPLE *sample7 = NULL;
	ALLEGRO_SAMPLE *sample8 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance1 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance2 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance3 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance4 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance5 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance6 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance7 = NULL;
	ALLEGRO_SAMPLE_INSTANCE *instance8 = NULL;


	//these two functions initalize the two included audio libraries
	al_install_audio();

	al_init_acodec_addon();

	al_reserve_samples(8);//decides how many samples we're using, creates a default mixer

						  //returns a memory address to the music file. make sure it's in your project folder!
	sample1 = al_load_sample("Wrong.wav");
	sample2 = al_load_sample("Paddle.wav");
	sample3 = al_load_sample("Brick1.wav");
	sample4 = al_load_sample("Brick2.wav");
	sample5 = al_load_sample("OMNOM.wav");
	sample6 = al_load_sample("BackgroundMusic.wav");
	sample7 = al_load_sample("GameOver.wav");
	sample8 = al_load_sample("YouWin.wav");

	instance1 = al_create_sample_instance(sample1);
	instance2 = al_create_sample_instance(sample2);
	instance3 = al_create_sample_instance(sample3);
	instance4 = al_create_sample_instance(sample4);
	instance5 = al_create_sample_instance(sample5);
	instance6 = al_create_sample_instance(sample6);
	instance7 = al_create_sample_instance(sample7);
	instance8 = al_create_sample_instance(sample8);

	al_set_sample_instance_playmode(instance6, ALLEGRO_PLAYMODE_LOOP);

	al_attach_sample_instance_to_mixer(instance1, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance2, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance3, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance4, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance5, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance6, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance7, al_get_default_mixer());
	al_attach_sample_instance_to_mixer(instance8, al_get_default_mixer());

	//end audio stuff

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_register_event_source(event_queue, al_get_timer_event_source(timer));

	al_register_event_source(event_queue, al_get_keyboard_event_source());

	//draw the initial screen, start up the timer
	al_clear_to_color(al_map_rgb(0, 0, 0));

	al_flip_display();

	al_start_timer(timer);


	//instantiate (stamp) your bricks
	/////////////////////////////ROW 1//////////////////
	Brick Brick1;
	Brick1.set_values(0, 0, 100, 40);
	Brick Brick2;
	Brick2.set_values(110, 0, 100, 40);
	Brick Brick3;
	Brick3.set_values(220, 0, 100, 40);
	Brick Brick4;
	Brick4.set_values(330, 0, 100, 40);
	Brick Brick5;
	Brick5.set_values(440, 0, 100, 40);
	Brick Brick6;
	Brick6.set_values(550, 0, 100, 40);
	Brick Brick7;
	Brick7.set_values(660, 0, 100, 40);
	Brick Brick8;
	Brick8.set_values(770, 0, 100, 40);
	Brick Brick9;
	Brick9.set_values(880, 0, 100, 40);
	/////////////////////////////ROW 2//////////////////
	Brick Brick10;
	Brick10.set_values(0, 50, 100, 40);
	Brick Brick11;
	Brick11.set_values(110, 50, 100, 40);
	Brick Brick12;
	Brick12.set_values(220, 50, 100, 40);
	Brick Brick13;
	Brick13.set_values(330, 50, 100, 40);
	Brick Brick14;
	Brick14.set_values(440, 50, 100, 40);
	Brick Brick15;
	Brick15.set_values(550, 50, 100, 40);
	Brick Brick16;
	Brick16.set_values(660, 50, 100, 40);
	Brick Brick17;
	Brick17.set_values(770, 50, 100, 40);
	Brick Brick18;
	Brick18.set_values(880, 50, 100, 40);
	/////////////////////////////ROW 3//////////////////
	Brick Brick19;
	Brick19.set_values(0, 100, 100, 40);
	Brick Brick20;
	Brick20.set_values(110, 100, 100, 40);
	Brick Brick21;
	Brick21.set_values(220, 100, 100, 40);
	Brick Brick22;
	Brick22.set_values(330, 100, 100, 40);
	Brick Brick23;
	Brick23.set_values(440, 100, 100, 40);
	Brick Brick24;
	Brick24.set_values(550, 100, 100, 40);
	Brick Brick25;
	Brick25.set_values(660, 100, 100, 40);
	Brick Brick26;
	Brick26.set_values(770, 100, 100, 40);
	Brick Brick27;
	Brick27.set_values(880, 100, 100, 40);
	while (!doexit)
	{
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//if a clock ticks, check if we should be moving a square (by checking key or key2)
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//	al_play_sample_instance(instance6);
			al_play_sample(sample6, 0.25, 0.0, 1, ALLEGRO_PLAYMODE_ONCE, NULL);
			if (lives == 0) {
				music = false;
				al_rest(1);
				al_play_sample_instance(instance7);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				//al_draw_text(font1, al_map_rgb(255, 255, 255), 480, 400, ALLEGRO_ALIGN_CENTRE, "Game Over");
				al_flip_display();
				al_rest(3);
				al_destroy_sample(sample1);
				al_destroy_sample_instance(instance1);
				al_destroy_sample(sample2);
				al_destroy_sample_instance(instance2);
				al_destroy_sample(sample3);
				al_destroy_sample_instance(instance3);
				al_destroy_sample(sample4);
				al_destroy_sample_instance(instance4);
				al_destroy_sample(sample5);
				al_destroy_sample_instance(instance5);
				al_destroy_sample(sample6);
				al_destroy_sample_instance(instance6);
				al_destroy_sample(sample7);
				al_destroy_sample_instance(instance7);
				al_destroy_sample(sample8);
				al_destroy_sample_instance(instance8);
				al_destroy_bitmap(bouncer);
				al_destroy_bitmap(bouncer3);
				al_destroy_bitmap(bouncer4);
				al_destroy_timer(timer);
				al_destroy_display(display);
				al_destroy_event_queue(event_queue);
			}

			if (Brick1.isDead() && Brick2.isDead() && Brick3.isDead() && Brick4.isDead() && Brick5.isDead() && Brick6.isDead() && Brick7.isDead() && Brick8.isDead() && Brick9.isDead() && Brick10.isDead() && Brick11.isDead() && Brick12.isDead() && Brick13.isDead() && Brick14.isDead() && Brick15.isDead() && Brick16.isDead() && Brick17.isDead() && Brick18.isDead() && Brick19.isDead() && Brick20.isDead() && Brick21.isDead() && Brick22.isDead() && Brick23.isDead() && Brick24.isDead() && Brick25.isDead() && Brick26.isDead() && Brick27.isDead()) {
				music = false;
				al_rest(1);
				al_play_sample_instance(instance8);
				al_clear_to_color(al_map_rgb(0, 0, 0));
				al_flip_display();
				//al_draw_text(font1, al_map_rgb(255, 255, 255), 480, 400, ALLEGRO_ALIGN_CENTRE, "You Win");
				al_flip_display();
				al_rest(3);
				al_destroy_sample(sample1);
				al_destroy_sample_instance(instance1);
				al_destroy_sample(sample2);
				al_destroy_sample_instance(instance2);
				al_destroy_sample(sample3);
				al_destroy_sample_instance(instance3);
				al_destroy_sample(sample4);
				al_destroy_sample_instance(instance4);
				al_destroy_sample(sample5);
				al_destroy_sample_instance(instance5);
				al_destroy_sample(sample6);
				al_destroy_sample_instance(instance6);
				al_destroy_sample(sample7);
				al_destroy_sample_instance(instance7);
				al_destroy_sample(sample8);
				al_destroy_sample_instance(instance8);
				al_destroy_bitmap(bouncer);
				al_destroy_bitmap(bouncer3);
				al_destroy_bitmap(bouncer4);
				al_destroy_timer(timer);
				al_destroy_display(display);
				al_destroy_event_queue(event_queue);
			}

			if (!Brick1.isDead() && bounding_box_collision(0, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick1.killBrick(); //when you hit it, kill it
				}
			}
			if (!Brick2.isDead() && bounding_box_collision(110, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick2.killBrick();
				}
			}
			if (!Brick3.isDead() && bounding_box_collision(220, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick3.killBrick();
				}
			}
			if (!Brick4.isDead() && bounding_box_collision(330, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick4.killBrick();
				}
			}
			if (!Brick5.isDead() && bounding_box_collision(440, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick5.killBrick();
				}
			}
			if (!Brick6.isDead() && bounding_box_collision(550, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick6.killBrick();
				}
			}
			if (!Brick7.isDead() && bounding_box_collision(660, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick7.killBrick();
				}
			}
			if (!Brick8.isDead() && bounding_box_collision(770, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick8.killBrick();
				}
			}
			if (!Brick9.isDead() && bounding_box_collision(880, 0, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer3_dy = -bouncer3_dy;
					Brick9.killBrick();
				}
			}
			///////////////////////////////ROW 2//////////////////

			if (!Brick10.isDead() && bounding_box_collision(0, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick10.killBrick();
				}
			}
			if (!Brick11.isDead() && bounding_box_collision(110, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick11.killBrick();
				}
			}
			if (!Brick12.isDead() && bounding_box_collision(220, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick12.killBrick();
				}
			}
			if (!Brick13.isDead() && bounding_box_collision(330, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick13.killBrick();
				}
			}
			if (!Brick14.isDead() && bounding_box_collision(440, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick14.killBrick();
				}
			}
			if (!Brick15.isDead() && bounding_box_collision(550, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick15.killBrick();
				}
			}
			if (!Brick16.isDead() && bounding_box_collision(660, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick16.killBrick();
				}
			}
			if (!Brick17.isDead() && bounding_box_collision(770, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick17.killBrick();
				}
			}
			if (!Brick18.isDead() && bounding_box_collision(880, 50, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer3_dy = -bouncer3_dy;
					Brick18.killBrick();
				}
			}
			///////////////////////////////ROW 3//////////////////

			if (!Brick19.isDead() && bounding_box_collision(0, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick19.killBrick();
				}
			}
			if (!Brick20.isDead() && bounding_box_collision(110, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick20.killBrick();
				}
			}
			if (!Brick21.isDead() && bounding_box_collision(220, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick21.killBrick();
				}
			}
			if (!Brick22.isDead() && bounding_box_collision(330, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick22.killBrick();
				}
			}
			if (!Brick23.isDead() && bounding_box_collision(440, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick23.killBrick();
				}
			}
			if (!Brick24.isDead() && bounding_box_collision(550, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick24.killBrick();
				}
			}
			if (!Brick25.isDead() && bounding_box_collision(660, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick25.killBrick();
				}
			}
			if (!Brick26.isDead() && bounding_box_collision(770, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick26.killBrick();
				}
			}
			if (!Brick27.isDead() && bounding_box_collision(880, 100, 100, 40, bouncer3_x, bouncer3_y, 50, 50)) {
				if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer3_dy = -bouncer3_dy;
					Brick27.killBrick();
				}
			}

			///

			if (!Brick1.isDead() && bounding_box_collision(0, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick1.killBrick(); //when you hit it, kill it
				}
			}
			if (!Brick2.isDead() && bounding_box_collision(110, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick2.killBrick();
				}
			}
			if (!Brick3.isDead() && bounding_box_collision(220, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick3.killBrick();
				}
			}
			if (!Brick4.isDead() && bounding_box_collision(330, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick4.killBrick();
				}
			}
			if (!Brick5.isDead() && bounding_box_collision(440, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick5.killBrick();
				}
			}
			if (!Brick6.isDead() && bounding_box_collision(550, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick6.killBrick();
				}
			}
			if (!Brick7.isDead() && bounding_box_collision(660, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick7.killBrick();
				}
			}
			if (!Brick8.isDead() && bounding_box_collision(770, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick8.killBrick();
				}
			}
			if (!Brick9.isDead() && bounding_box_collision(880, 0, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance5);
					bouncer4_dy = -bouncer4_dy;
					Brick9.killBrick();
				}
			}
			///////////////////////////////ROW 2//////////////////

			if (!Brick10.isDead() && bounding_box_collision(0, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick10.killBrick();
				}
			}
			if (!Brick11.isDead() && bounding_box_collision(110, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick11.killBrick();
				}
			}
			if (!Brick12.isDead() && bounding_box_collision(220, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick12.killBrick();
				}
			}
			if (!Brick13.isDead() && bounding_box_collision(330, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick13.killBrick();
				}
			}
			if (!Brick14.isDead() && bounding_box_collision(440, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick14.killBrick();
				}
			}
			if (!Brick15.isDead() && bounding_box_collision(550, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick15.killBrick();
				}
			}
			if (!Brick16.isDead() && bounding_box_collision(660, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick16.killBrick();
				}
			}
			if (!Brick17.isDead() && bounding_box_collision(770, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick17.killBrick();
				}
			}
			if (!Brick18.isDead() && bounding_box_collision(880, 50, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance4);
					bouncer4_dy = -bouncer4_dy;
					Brick18.killBrick();
				}
			}
			if (!Brick19.isDead() && bounding_box_collision(0, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick19.killBrick();
				}
			}
			if (!Brick20.isDead() && bounding_box_collision(110, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick20.killBrick();
				}
			}
			if (!Brick21.isDead() && bounding_box_collision(220, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick21.killBrick();
				}
			}
			if (!Brick22.isDead() && bounding_box_collision(330, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick22.killBrick();
				}
			}
			if (!Brick23.isDead() && bounding_box_collision(440, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick23.killBrick();
				}
			}
			if (!Brick24.isDead() && bounding_box_collision(550, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick24.killBrick();
				}
			}
			if (!Brick25.isDead() && bounding_box_collision(660, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick25.killBrick();
				}
			}
			if (!Brick26.isDead() && bounding_box_collision(770, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick26.killBrick();
				}
			}
			if (!Brick27.isDead() && bounding_box_collision(880, 100, 100, 40, bouncer4_x, bouncer4_y, 50, 50)) {
				if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
					//flip the y direction
					al_play_sample_instance(instance3);
					bouncer4_dy = -bouncer4_dy;
					Brick27.killBrick();
				}
			}

			//if the box hits the left wall OR the right wall
			if (bouncer3_x < 0 || bouncer3_x > 980 - 32) {
				//flip the x direction
				bouncer3_dx = -bouncer3_dx;
			}
			if (bouncer4_x < 0 || bouncer4_x > 980 - 32) {
				//flip the x direction
				bouncer4_dx = -bouncer4_dx;
			}
			//if the box hits the top wall OR the bottom wall
			if (bouncer3_y > 800 - 32) {
				//flip the y direction
				bouncer3_x = 200;
				bouncer3_y = 200;
				//bouncer3_dy = -12.0, bouncer3_dy = 12.0;
				//lives = lives--;
				al_play_sample_instance(instance1);
			}
			if (bouncer4_y > 800 - 32) {
				//flip the y direction
				bouncer4_x = 200;
				bouncer4_y = 200;
				//bouncer4_dy = -12.0, bouncer4_dy = 12.0;
				//lives = lives--;
				al_play_sample_instance(instance1);
			}
			if (bouncer3_y < 0 - 32) {
				//flip the y direction
				bouncer3_dy = -12.0, bouncer3_dy = 12.0;
			}
			if (bouncer4_y < 0 - 32) {
				//flip the y direction
				bouncer4_dy = -12.0, bouncer4_dy = 12.0;
			}

			//really important code!
			//move the box in a diagonal
			bouncer3_x += bouncer3_dx;
			bouncer3_y += bouncer3_dy;
			bouncer4_x += bouncer4_dx;
			bouncer4_y += bouncer4_dy;

			if (key[KEY_LEFT] && bouncer_x >= 4.0) {
				bouncer_x -= speed;
				color++;
			}

			if (key[KEY_RIGHT] && bouncer_x <= SCREEN_W - BOUNCER_H - 170) {
				bouncer_x += speed;
				color++;
			}

			redraw = true;
		}

		//kill program if window "X" has been clicked
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}

		//update key/key2 if a key has been pressed
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			}
		}
		//update key/key2 if a key has been released
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;


			}
		}
		////////////////////LE RENDER SECTION./////////////////////////////////////////////////////
		//take all the above information and update screen
		al_clear_to_color(al_map_rgb(color - 100, color * 3, color * 4));


		//le bricks
		if (Brick1.isDead() == false) {
			Brick1.drawBrick();
		}
		if (Brick2.isDead() == false) {
			Brick2.drawBrick();
		}
		if (Brick3.isDead() == false) {
			Brick3.drawBrick();
		}
		if (Brick4.isDead() == false) {
			Brick4.drawBrick();
		}
		if (Brick5.isDead() == false) {
			Brick5.drawBrick();
		}
		if (Brick6.isDead() == false) {
			Brick6.drawBrick();
		}
		if (Brick7.isDead() == false) {
			Brick7.drawBrick();
		}
		if (Brick8.isDead() == false) {
			Brick8.drawBrick();
		}
		if (Brick9.isDead() == false) {
			Brick9.drawBrick();
		}

		if (Brick10.isDead() == false) {
			Brick10.drawBrick();
		}
		if (Brick11.isDead() == false) {
			Brick11.drawBrick();
		}
		if (Brick12.isDead() == false) {
			Brick12.drawBrick();
		}
		if (Brick13.isDead() == false) {
			Brick13.drawBrick();
		}
		if (Brick14.isDead() == false) {
			Brick14.drawBrick();
		}
		if (Brick15.isDead() == false) {
			Brick15.drawBrick();
		}
		if (Brick16.isDead() == false) {
			Brick16.drawBrick();
		}
		if (Brick17.isDead() == false) {
			Brick17.drawBrick();
		}
		if (Brick18.isDead() == false) {
			Brick18.drawBrick();
		}

		if (Brick19.isDead() == false) {
			Brick19.drawBrick();
		}
		if (Brick20.isDead() == false) {
			Brick20.drawBrick();
		}
		if (Brick21.isDead() == false) {
			Brick21.drawBrick();
		}
		if (Brick22.isDead() == false) {
			Brick22.drawBrick();
		}
		if (Brick23.isDead() == false) {
			Brick23.drawBrick();
		}
		if (Brick24.isDead() == false) {
			Brick24.drawBrick();
		}
		if (Brick25.isDead() == false) {
			Brick25.drawBrick();
		}
		if (Brick26.isDead() == false) {
			Brick26.drawBrick();
		}
		if (Brick27.isDead() == false) {
			Brick27.drawBrick();
		}


		//	al_draw_filled_rectangle(0, 0, 980, 800, al_map_rgb(color-100, color*3,color*4));

		al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);

		al_draw_bitmap(bouncer3, bouncer3_x, bouncer3_y, 0);

		al_draw_bitmap(bouncer4, bouncer4_x, bouncer4_y, 0);
		//call the bounding box function. if it returns 1, print out your collision message
		//al_draw_text(font2, al_map_rgb(255, 255, 255), 50, 350, ALLEGRO_ALIGN_CENTRE, "lives");
		//al_draw_textf(font2, al_map_rgb(255, 255, 255), 90, 350, ALLEGRO_ALIGN_CENTRE, "%d", lives);

		if (bounding_box_collision(bouncer_x, bouncer_y, 200, 30, bouncer3_x, bouncer3_y, 50, 50)) {
			if (bouncer3_y < bouncer_y || bouncer3_y > bouncer_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				bouncer3_dy = -bouncer3_dy;
			}
		}
		if (bounding_box_collision(bouncer_x, bouncer_y, 200, 30, bouncer4_x, bouncer4_y, 50, 50)) {
			if (bouncer4_y < bouncer_y || bouncer4_y > bouncer_y - 32) {
				//flip the y direction
				al_play_sample_instance(instance2);
				bouncer4_dy = -bouncer4_dy;
			}
		}
		al_flip_display();

	}

	al_destroy_sample(sample1);
	al_destroy_sample_instance(instance1);
	al_destroy_sample(sample2);
	al_destroy_sample_instance(instance2);
	al_destroy_sample(sample3);
	al_destroy_sample_instance(instance3);
	al_destroy_sample(sample4);
	al_destroy_sample_instance(instance4);
	al_destroy_sample(sample5);
	al_destroy_sample_instance(instance5);
	al_destroy_sample(sample6);
	al_destroy_sample_instance(instance6);
	al_destroy_sample(sample7);
	al_destroy_sample_instance(instance7);
	al_destroy_sample(sample8);
	al_destroy_sample_instance(instance8);
	al_destroy_bitmap(bouncer);
	al_destroy_bitmap(bouncer3);
	al_destroy_bitmap(bouncer4);
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}


void Brick::set_values(int x, int y, int w, int h) {
	xPos = x;
	yPos = y;

	width = w;
	height = h;
	dead = false;
}

void Brick::drawBrick() {
	al_draw_filled_rectangle(xPos, yPos, xPos + width, yPos + height, al_map_rgb(120, 80, 255));


}
bool Brick::isDead() {
	return dead;
}


int bounding_box_collision(int b1_x, int b1_y, int b1_w, int b1_h, int b2_x, int b2_y, int b2_w, int b2_h)
{
	if ((b1_x > b2_x + b2_w - 1) || // is b1 on the right side of b2?
		(b1_y > b2_y + b2_h - 1) || // is b1 under b2?
		(b2_x > b1_x + b1_w - 1) || // is b2 on the right side of b1?
		(b2_y > b1_y + b1_h - 1))   // is b2 under b1?
	{
		// no collision
		return 0;
	}

	// collision
	return 1;
}

void Brick::killBrick() {
	dead = true;
}