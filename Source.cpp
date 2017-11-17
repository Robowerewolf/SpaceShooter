#include <allegro5/allegro.h>
#include <iostream>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <list>

#include "Gameobjects.h"
#include "SpaceShip.h"

#include "Globals.h"
using namespace std;

bool keys[] = { false, false, false, false, false };
enum KEYS{UP, DOWN, LEFT, RIGHT, SPACE};

SpaceShip *ship;

list<GameObject *> objects;
list<GameObject *>::iterator iter;
list<GameObject *>::iterator iter2;
int main() {
	bool done = false;
	bool render = false;

	float gameTime = 0;
	int frames = 0;
	int gameFPS = 0;

	ship = new SpaceShip();

	ALLEGRO_BITMAP *shipImage = NULL;

	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font = NULL;


	al_init();

	display = al_create_display(WIDTH, HEIGHT);

	al_install_keyboard();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_audio();
	al_init_acodec_addon();

	font = al_load_font("spaceage.ttf", 64, 0);
	al_reserve_samples(15);

	shipImage = al_load_bitmap("ship.png");
	//al_convert_mask_to_alpha(shipImage, al_map_rgb(255, 0, 255));
	ship->init(shipImage);

	objects.push_back(ship);

	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60);

	//al_draw_text(font, al_map_rgb(255, 0, 0), 400, 150, ALLEGRO_ALIGN_CENTRE, "SpaceShooter");

	al_flip_display();
	al_rest(2);


	/*ship = al_create_bitmap(32, 32);
	al_set_target_bitmap(ship);
	al_clear_to_color(al_map_rgb(255, 255, 255));*/

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//cout << "flag1" << endl;
	al_start_timer(timer);
	gameTime = al_current_time();


	while (!done) {
		cout << "game loop" << endl;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			render = true;

			frames++;
			if (al_current_time() - gameTime >= 1) {
				gameTime = al_current_time();
				gameFPS = frames;
				frames = 0;
			}


			if (keys[UP])
				ship->MoveUp();
			else if (keys[DOWN])
				ship->MoveDown();
			else
				ship->ResetAnimation(1);
			if (keys[LEFT])
				ship->MoveLeft();
			else if (keys[RIGHT])
				ship->MoveRight();
			else
				ship->ResetAnimation(0);

			for (iter = objects.begin(); iter != objects.end(); ++iter)
				(*iter)->Update();
		}


		//RENDER SECTION!!

		if (render && al_is_event_queue_empty(event_queue)) {
			render = false;

			for (iter = objects.begin(); iter != objects.end(); ++iter)
				(*iter)->Render();


			//al_draw_bitmap(ship, 400, 175, 0);

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));
		}
	}//end game loop

	for (iter = objects.begin(); iter != objects.end(); ) {
		(*iter)->Destroy();
		delete (*iter);
		iter = objects.erase(iter);
	}


	return 0;
}//end main
