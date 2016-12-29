#include<allegro5\allegro.h>
#include<allegro5\allegro_native_dialog.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_primitives.h>
#include <allegro5\allegro_audio.h>
#include <allegro5\allegro_acodec.h>
#include <allegro5\monitor.h>
#include <ctime>


	/////////////////////////////// CONFIGURATION ////////////////////////////////
#define FPS 60
#define SPEED_OF_BULLETS 6
#define SPEED_OF_ENEMIES 5
#define COOLDOWN_ON_SHOTS 0.6f
#define MAX_BULLETS 100 // MAX NUMBER OF BULLETS AT THE MAP
#define MAX_NUMBER_OF_ENEMIES 10
#define MAX_NUMBER_OF_ENEMIES_AT_THE_SAME_TIME MAX_NUMBER_OF_ENEMIES
#define SPEED_OF_PLAYER 5
#define COOLDOWN_ON_ENEMIES_DIRECTION 0.4f
#define COOLDOWN_ON_ENEMIES_SHOTS 0.5f
#define PROBABILITY_THAT_ENEMY_SHOOTS 400
#define PROBABILITY_THAT_ENEMY_RESPAWNS 1
#define PROBABILITY_THAT_BOSS_RESPAWNS 10


//////////////// BONUSES /////////////////////
#define CZAS_NIESMIERTELNOSCI 5

////////////////OBIECTS ON MAP////////////////
// 0 - NOTHING
// 1 - FLASK
////////////////////////////////// CONFIGURATION ////////////////////////////////

////////////VARIABLES//////////////
bool left = false;
bool is_back = true;
bool locked[] = { false };
bool keys[5] = { false, false, false, false, false };
bool keys1[5] = { false, false, false, false, false };
int menu[] = { 0, 0, 0 };
int count_enter = 0;
int remember, enter_once_more_time;
bool music_on = true;
bool last_music_option = true;
bool left_menu = false;
bool in_game = false;
bool number_of_players[2] = { false, false };
int count_enter_choice = 0;
int pos_player[2][2] = { 0 };
int player_alive[2] = { true, true };
int direction[2] = { -1 };
float DeltaTime = 1.0 / FPS;
int stage = 1;
int enemies_killed = 0;
int enemies_count = 0;
// cooldowns for players shoots, 0 - player1, 1 - player2
float player_timer_cooldown[2] = { -1.0f };
// cooldowns for enemy's, 0 - movement, 1 - shoot
float enemy_timer_cooldown[2][MAX_NUMBER_OF_ENEMIES] = { -1.0f };
enum KEYS { UP, DOWN, LEFT, RIGHT, SPACE };
enum KEYS1 { W, S, A, D, CAPS };

int objMap[12][28] =
{{0,1,1,1,1,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
{ 0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0},
{ 0,1,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0},
{ 0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,0,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
{ 0,1,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,0,1,0,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,1,0,0},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,1,0,},
{ 0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,},

};

// menu bitmaps
ALLEGRO_BITMAP *BMP_START = NULL;
ALLEGRO_BITMAP *BMP_NEWGAME = NULL;
ALLEGRO_BITMAP *BMP_CREDITS = NULL;
ALLEGRO_BITMAP *BMP_EXIT = NULL;
ALLEGRO_BITMAP *BMP_OPTIONS = NULL;
ALLEGRO_BITMAP *BMP_RECORDS = NULL;
ALLEGRO_BITMAP *icon = NULL;
ALLEGRO_BITMAP *BMP_ONE_PLAYER = NULL;
ALLEGRO_BITMAP *BMP_TWO_PLAYERS = NULL;
ALLEGRO_BITMAP *BMP_MUSIC = NULL;
ALLEGRO_BITMAP *BMP_MUSIC_CURRENT = NULL;
//character bitmaps//
ALLEGRO_BITMAP *BMP_CHARACTER_UP = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_DOWN = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_LEFT = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_RIGHT = NULL;
ALLEGRO_BITMAP *BMP_BULLET = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_UP_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_DOWN_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_LEFT_2 = NULL;
ALLEGRO_BITMAP *BMP_CHARACTER_RIGHT_2 = NULL;
ALLEGRO_BITMAP *BMP_BULLET_2 = NULL;
//////////
ALLEGRO_BITMAP *BMP_SYMBOL = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_1 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_2 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_3 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_4 = NULL;
ALLEGRO_BITMAP *BMP_TEXTURE_5 = NULL;
ALLEGRO_BITMAP *BMP_SEMESTER_OVER = NULL;
ALLEGRO_BITMAP *BMP_ENEMY = NULL;
ALLEGRO_BITMAP *BMP_ENEMY_BOSS = NULL;
ALLEGRO_BITMAP *BMP_BULLET_ENEMY = NULL;
// music sample //
ALLEGRO_SAMPLE *sample = NULL;
//

int width, height;

typedef struct obiekt {
	int x;
	int y;
	int alive;
	int kierunek;
	int czyj;
} obiekt;

obiekt bullets[MAX_BULLETS];

int add_bullet(int x, int y, int kierunek, int czyj_pocisk) // 0 - gracz 1 - komputer
{
	int i;

	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 0) {
			bullets[i].x = x;
			bullets[i].y = y;
			bullets[i].kierunek = kierunek;
			bullets[i].czyj = czyj_pocisk;
			return i;
		}
	}

	return(0);
}

typedef struct przeciwnik {
	int x;
	int y;
	int alive;
	int lifes;
	int died;
	int boss;
} przeciwnik;
bool collision(int pos_x1, int width_x1, int pos_x2, int pos_y1, int height_y1, int pos_y2, int height_y2) {
	if (pos_x1 + width_x1 >= pos_x2 && pos_x1 <= pos_x2 + width_x1 && pos_y1 + height_y1 >= pos_y2 && pos_y1 <= pos_y2 + height_y2)
		return true;
	else return false;
}
void create_enemy(przeciwnik Przeciwnik[]) {
	for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++) {
		if ((enemies_count < MAX_NUMBER_OF_ENEMIES_AT_THE_SAME_TIME) && !Przeciwnik[i].alive && Przeciwnik[i].died != 1) {
			if (rand() % PROBABILITY_THAT_ENEMY_RESPAWNS == 0) {
				Przeciwnik[i].alive = 1;
				Przeciwnik[i].lifes = 1;
				enemies_count++;
				Przeciwnik[i].x = (rand() % width) + 30;
				Przeciwnik[i].y = (rand() % height - 60) + 50;
				for (int t = 0; t <13; t++) {
					for (int z = 0; z < 28; z++) {
						if (objMap[t][z] != 0 && collision(Przeciwnik[i].x, al_get_bitmap_width(BMP_ENEMY), z * 48, Przeciwnik[i].y, al_get_bitmap_height(BMP_ENEMY), t * 70, al_get_bitmap_width(BMP_TEXTURE_1))) {
							Przeciwnik[i].x = (rand() % width) + 30;
							Przeciwnik[i].y = (rand() % height - 60) + 50;
						}
					}
				}
				while (Przeciwnik[i].x >= width / 2 - 150  && Przeciwnik[i].x <= width / 2 + al_get_bitmap_width(BMP_TEXTURE_1) * 2 && Przeciwnik[i].y >= height - 150 - al_get_bitmap_width(BMP_TEXTURE_1)) {
					Przeciwnik[i].x = (rand() % width) + 30;
				}
				if (rand() % PROBABILITY_THAT_BOSS_RESPAWNS == 0) {
					Przeciwnik[i].boss = 1;
					Przeciwnik[i].lifes = 2;
				}
			}
		}
	}
}

void draw_bullets(obiekt bullets[]) {
	int i;
	for (i = 0; i < MAX_BULLETS; i++) {
		if (bullets[i].alive == 1)
		{
			if (bullets[i].czyj == 0)
				al_draw_bitmap(BMP_BULLET, bullets[i].x, bullets[i].y, 0);
			if (bullets[i].czyj == 1)
				al_draw_bitmap(BMP_BULLET_ENEMY, bullets[i].x, bullets[i].y, 0);
			if (bullets[i].czyj == 2)
				al_draw_bitmap(BMP_BULLET_2, bullets[i].x, bullets[i].y, 0);
			switch (bullets[i].kierunek) {
			case 0: bullets[i].y -= SPEED_OF_BULLETS; break;
			case 1: bullets[i].y += SPEED_OF_BULLETS; break;
			case 2: bullets[i].x -= SPEED_OF_BULLETS; break;
			case 3: bullets[i].x += SPEED_OF_BULLETS; break;
			}
		}
	}
}
void draw_enemies(przeciwnik Przeciwnik[]) {
	int i;
	for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
		if (Przeciwnik[i].alive == 1 && Przeciwnik[i].died == 0) {
			al_draw_bitmap(BMP_ENEMY, Przeciwnik[i].x, Przeciwnik[i].y, 0);
			if (Przeciwnik[i].boss == 1) {
				al_draw_bitmap(BMP_ENEMY_BOSS, Przeciwnik[i].x, Przeciwnik[i].y, 0);
			}
		}
}
przeciwnik Przeciwnik[MAX_NUMBER_OF_ENEMIES];




void rysuj_poziom(int ktory) {
	switch (ktory) {
	case 1: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/I.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/1_semestr_koniec.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_boss.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		al_flip_display();
		break;
	}
	case 2: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/2_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 3: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/3_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 4: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY= al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/4_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 5: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/5_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 6: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	case 7: {
		BMP_START = al_load_bitmap("mapybmp/pierwsza.png");
		BMP_TEXTURE_1 = al_load_bitmap("mapybmp/fiolka.png");
		BMP_SYMBOL = al_load_bitmap("mapybmp/II.png");
		BMP_ENEMY = al_load_bitmap("przeciwnicy/calka.png");
		BMP_ENEMY_BOSS = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_BULLET_ENEMY = al_load_bitmap("przeciwnicy/calka_pocisk.png");
		BMP_SEMESTER_OVER = al_load_bitmap("mapybmp/6_semestr_koniec.png");
		al_flip_display();
		break;
	}
	}
}
void clean_everything() {
	if (number_of_players[0] == true) {
		pos_player[0][0] = width / 2 - 100;
		pos_player[0][1] = height - 80;
		direction[0] = 0;
	}
	if (number_of_players[1] == true) {
		pos_player[1][0] = width / 2 + 100;
		pos_player[1][1] = height - 80;
		direction[0] = 0;
		direction[1] = 0;
	}

	for (int j = 0; j < 2; j++) 
		for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			enemy_timer_cooldown[j][i] = -1.0f;

	for (int i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
		Przeciwnik[i].died = 0;

	for(int j = 0; j < MAX_BULLETS; j++)
		bullets[j].alive = 0;

	al_flip_display();
}
void pre_start_game() {
	ALLEGRO_EVENT_QUEUE *event_queue_stage = NULL;
	ALLEGRO_TIMER *timer_stage = NULL;
	srand(time(NULL));
	int i, temp;
	event_queue_stage = al_create_event_queue();
	timer_stage = al_create_timer(DeltaTime);
	al_start_timer(timer_stage);
	al_register_event_source(event_queue_stage, al_get_keyboard_event_source());
	al_register_event_source(event_queue_stage, al_get_timer_event_source(timer_stage));
	rysuj_poziom(1);
	int width_bullet = al_get_bitmap_width(BMP_BULLET);
	int width_flask = al_get_bitmap_width(BMP_TEXTURE_1);
	int height_bullet = al_get_bitmap_height(BMP_BULLET);
	int height_flask = al_get_bitmap_height(BMP_TEXTURE_1);
	int width_symbol = al_get_bitmap_width(BMP_SYMBOL);
	int height_symbol = al_get_bitmap_height(BMP_SYMBOL);
	int height_character = al_get_bitmap_height(BMP_CHARACTER_UP);
	int width_character = al_get_bitmap_width(BMP_CHARACTER_UP);
	int width_enemy = al_get_bitmap_width(BMP_ENEMY);
	int height_enemy = al_get_bitmap_height(BMP_ENEMY);
	int last_pos_player[2][2] = { pos_player[0][0], pos_player[0][1], pos_player[1][0], pos_player[1][1] };
	if (number_of_players[1] == true) {
		player_alive[1] = true;
	}
	else player_alive[1] = false;
	int last_pos_comp[MAX_NUMBER_OF_ENEMIES][2];
	int SYMBOL[2] = { width / 2, height - 65 };
	clean_everything();
	while (left_menu && in_game) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue_stage, &ev);
		if (player_timer_cooldown[0] > 0.0f)
			player_timer_cooldown[0] = player_timer_cooldown[0] - (DeltaTime);
		if (player_timer_cooldown[1] > 0.0f)
			player_timer_cooldown[1] = player_timer_cooldown[1] - (DeltaTime);
		for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			if (enemy_timer_cooldown[1][i] > 0.0f)
				enemy_timer_cooldown[1][i] = enemy_timer_cooldown[1][i] - (DeltaTime);
		for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			if (enemy_timer_cooldown[0][i] > 0.0f)
				enemy_timer_cooldown[0][i] = enemy_timer_cooldown[0][i] - (DeltaTime);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
				if (number_of_players[1] == true) {
			case ALLEGRO_KEY_W:
				keys1[W] = true;
				direction[1] = 0;
				break;
			case ALLEGRO_KEY_S:
				keys1[S] = true;
				direction[1] = 1;
				break;
			case ALLEGRO_KEY_A:
				keys1[A] = true;
				direction[1] = 2;
				break;
			case ALLEGRO_KEY_D:
				keys1[D] = true;
				direction[1] = 3;
				break;
			case ALLEGRO_KEY_CAPSLOCK:
				keys1[CAPS] = true;
				if (player_timer_cooldown[1] <= 0 && player_alive[1]) {
					temp = add_bullet(pos_player[1][0], pos_player[1][1] + 20, direction[1], 2);
					bullets[temp].alive = 1;
					player_timer_cooldown[1] = COOLDOWN_ON_SHOTS;
				}
				break;
			}
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				direction[0] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				direction[0] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				direction[0] = 2;
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				direction[0] = 3;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				if (player_timer_cooldown[0] <= 0 && player_alive[0]) {
					temp = add_bullet(pos_player[0][0], pos_player[0][1] + 20, direction[0], 0);
					bullets[temp].alive = 1;
					player_timer_cooldown[0] = COOLDOWN_ON_SHOTS;
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
				if (number_of_players[1] == true) {
			case ALLEGRO_KEY_W:
				keys1[W] = false;
				break;
			case ALLEGRO_KEY_S:
				keys1[S] = false;
				break;
			case ALLEGRO_KEY_A:
				keys1[A] = false;
				break;
			case ALLEGRO_KEY_D:
				keys1[D] = false;
				break;
			case ALLEGRO_KEY_CAPSLOCK:
				keys1[CAPS] = false;
				break;
				}
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			case ALLEGRO_KEY_ESCAPE:
				in_game = false;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			al_draw_bitmap(BMP_START, 0, 0, 0);
			al_draw_bitmap(BMP_SYMBOL, SYMBOL[0], SYMBOL[1], 0);
			draw_bullets(bullets);
			draw_enemies(Przeciwnik);

			for (i = 0; i < MAX_NUMBER_OF_ENEMIES; i++)
			{
				if (Przeciwnik[i].alive)
				{
					int random_x[MAX_NUMBER_OF_ENEMIES];
					int random_y[MAX_NUMBER_OF_ENEMIES];
					int losowa[MAX_NUMBER_OF_ENEMIES];
					if (enemy_timer_cooldown[0][i] <= 0) {
						losowa[i] = rand() % 4;
						enemy_timer_cooldown[0][i] = COOLDOWN_ON_ENEMIES_DIRECTION;
						random_x[i] = rand() % SPEED_OF_ENEMIES;
						random_y[i] = rand() % SPEED_OF_ENEMIES;
					}
					if (enemy_timer_cooldown[1][i] <= 0 && rand() % PROBABILITY_THAT_ENEMY_SHOOTS == 0) {
						temp = add_bullet(Przeciwnik[i].x, Przeciwnik[i].y + 20, direction[0], 1);
						bullets[temp].alive = 1;
						enemy_timer_cooldown[1][i] = COOLDOWN_ON_ENEMIES_SHOTS;
					}
					last_pos_comp[i][0] = Przeciwnik[i].x;
					last_pos_comp[i][1] = Przeciwnik[i].y;
					switch (losowa[i]) {
					case 0: Przeciwnik[i].x += random_x[i]; break;
					case 1: Przeciwnik[i].y += random_y[i]; break;
					case 2: Przeciwnik[i].x -= random_x[i]; break;
					case 3:Przeciwnik[i].y -= random_y[i]; break;
					}
					for (int t = 0; t <13; t++) {
						for (int z = 0; z < 28; z++) {
							if (objMap[t][z] == 1 && collision(Przeciwnik[i].x, width_enemy, z * 48, Przeciwnik[i].y, height_enemy, t * 70, height_flask)) {
								Przeciwnik[i].x = last_pos_comp[i][0];
								Przeciwnik[i].y = last_pos_comp[i][1];
							}
						}
					}
					if (Przeciwnik[i].x < 0) { Przeciwnik[i].x = 0; enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].y < 0) { Przeciwnik[i].y = 0; enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].y + height_enemy >= height) { Przeciwnik[i].y = height - height_enemy;  enemy_timer_cooldown[0][i] = -1.0f; }
					if (Przeciwnik[i].x + width_enemy >= width) {
						Przeciwnik[i].x = width - width_enemy; enemy_timer_cooldown[0][i] = -1.0f;
					}
					if (collision(Przeciwnik[i].x, width_enemy, SYMBOL[0], Przeciwnik[i].y, height_enemy, SYMBOL[1], height_symbol)) {
						Przeciwnik[i].x = last_pos_comp[i][0];
						Przeciwnik[i].y = last_pos_comp[i][1];
					}
					if (collision(Przeciwnik[i].x, width_enemy, pos_player[0][0], Przeciwnik[i].y, height_enemy, pos_player[0][1], height_character)) {
						Przeciwnik[i].x = last_pos_comp[i][0];
						Przeciwnik[i].y = last_pos_comp[i][1];
						pos_player[0][0] = last_pos_player[0][0];
						pos_player[0][1] = last_pos_player[0][1];
					}
					if (collision(Przeciwnik[i].x, width_enemy, pos_player[1][0], Przeciwnik[i].y, height_enemy, pos_player[1][1], height_character)) {
						Przeciwnik[i].x = last_pos_comp[i][0];
						Przeciwnik[i].y = last_pos_comp[i][1];
						pos_player[1][0] = last_pos_player[1][0];
						pos_player[1][1] = last_pos_player[1][1];
					}
				}
			}
			for (i = 0; i < MAX_BULLETS; i++)
			{
				if (bullets[i].alive)
				{
					if (bullets[i].x < 0 || bullets[i].y < 0 || bullets[i].x > width || bullets[i].y > height)
						bullets[i].alive = 0;
					for (int j = 0; j <MAX_NUMBER_OF_ENEMIES; j++) {
						if (bullets[i].czyj != 1 && Przeciwnik[j].died == 0 && Przeciwnik[j].alive && collision(bullets[i].x, width_bullet, Przeciwnik[j].x, bullets[i].y, height_bullet, Przeciwnik[j].y, height_enemy)) {
							bullets[i].alive = 0;
							if (--Przeciwnik[j].lifes == 0) {
								Przeciwnik[j].alive = 0;
								Przeciwnik[j].died = 1;
								enemies_killed++;
								enemies_count--;
							}
							if (enemies_killed == MAX_NUMBER_OF_ENEMIES) {
								al_draw_bitmap(BMP_START, 0, 0, 0);
								al_draw_bitmap(BMP_SEMESTER_OVER, width / 3, height / 2, 0);
								al_flip_display();
								al_stop_timer(timer_stage);
								clean_everything();
								enemies_killed = 0;
								al_rest(10.0);
								al_start_timer(timer_stage);
								rysuj_poziom(++stage);
							}
						}
					}
					if (bullets[i].czyj == 1 && collision(bullets[i].x, width_bullet, pos_player[0][0], bullets[i].y, height_bullet, pos_player[0][1], height_character)) {
						bullets[i].alive = 0;
						player_alive[0] = false;
						pos_player[0][0] = -100;
						pos_player[0][1] = -100;
					}
					if (bullets[i].czyj == 1 && collision(bullets[i].x, width_bullet, pos_player[1][0], bullets[i].y, height_bullet, pos_player[1][1], height_character)) {
						bullets[i].alive = 0;
						player_alive[1] = false;
						pos_player[1][0] = -100;
						pos_player[1][1] = -100;
					}
					for (int t = 0; t <13; t++) {
						for (int z = 0; z < 28; z++) {
							if (objMap[t][z] == 1 && collision(bullets[i].x, width_bullet, z * 48, bullets[i].y, height_bullet, t * 70, height_flask)) {
								bullets[i].alive = 0;
								objMap[t][z] = 0;
							}
						}
					}
					if (collision(bullets[i].x, width_bullet, SYMBOL[0], bullets[i].y, height_bullet, SYMBOL[1], height_symbol)) {
						bullets[i].alive = 0;
						in_game = false;
					}
				}
			}
			last_pos_player[0][0] = pos_player[0][0];
			last_pos_player[0][1] = pos_player[0][1];
			last_pos_player[1][0] = pos_player[1][0];
			last_pos_player[1][1] = pos_player[1][1];
			if (player_alive[0]) {
				pos_player[0][1] -= keys[UP] * SPEED_OF_PLAYER;
				pos_player[0][1] += keys[DOWN] * SPEED_OF_PLAYER;
				pos_player[0][0] -= keys[LEFT] * SPEED_OF_PLAYER;
				pos_player[0][0] += keys[RIGHT] * SPEED_OF_PLAYER;
			}
			if (number_of_players[1] == true && player_alive[1]) {
				pos_player[1][1] -= keys1[W] * SPEED_OF_PLAYER;
				pos_player[1][1] += keys1[S] * SPEED_OF_PLAYER;
				pos_player[1][0] -= keys1[A] * SPEED_OF_PLAYER;
				pos_player[1][0] += keys1[D] * SPEED_OF_PLAYER;
			}
			if (player_alive[0] && player_alive[1] && collision(pos_player[0][0], width_character, pos_player[1][0], pos_player[0][1], height_character, pos_player[1][1], height_character)){
				pos_player[0][0] = last_pos_player[0][0];
				pos_player[0][1] = last_pos_player[0][1];
				pos_player[1][0] = last_pos_player[1][0];
				pos_player[1][1] = last_pos_player[1][1];
			}
			if (player_alive[0]) {
				for (int t = 0; t <13; t++) {
					for (int i = 0; i < 28 ; i++) {
						if (objMap[t][i] == 1 && collision(pos_player[0][0], width_character, i*48, pos_player[0][1], height_character, t*70, height_flask)) {
							pos_player[0][0] = last_pos_player[0][0];
							pos_player[0][1] = last_pos_player[0][1];
						}
					}
				}
				if (pos_player[0][1] <= 0) pos_player[0][1] = 0;
				if (pos_player[0][0] <= 0) pos_player[0][0] = 0;
				if (pos_player[0][1] + height_character >= height) pos_player[0][1] = height - height_character;
				if (pos_player[0][0] + width_character >= width) pos_player[0][0] = width - width_character;
				if (collision(pos_player[0][0], width_character, SYMBOL[0], pos_player[0][1], height_character, SYMBOL[1], height_symbol)) {
					pos_player[0][0] = last_pos_player[0][0];
					pos_player[0][1] = last_pos_player[0][1];
				}
			}
			if (number_of_players[1] == true && player_alive[1]) {
				for (int t = 0; t <13; t++) {
					for (int i = 0; i < 28; i++) {
						if (objMap[t][i] == 1 && collision(pos_player[1][0], width_character, i * 48, pos_player[1][1], height_character, t * 70, height_flask)) {
							pos_player[1][0] = last_pos_player[1][0];
							pos_player[1][1] = last_pos_player[1][1];
						}
					}
				}
			if (pos_player[1][1] <= 0) pos_player[1][1] = 0;
			if (pos_player[1][0] <= 0) pos_player[1][0] = 0;
				if (pos_player[1][1] + height_character >= height) pos_player[1][1] = height - height_character;
				if (pos_player[1][0] + width_character >= width) pos_player[1][0] = width - width_character;
				if (collision(pos_player[1][0], width_character, SYMBOL[0], pos_player[1][1], height_character, SYMBOL[1], height_symbol)) {
					pos_player[1][0] = last_pos_player[1][0];
					pos_player[1][1] = last_pos_player[1][1];
				}
			}
			switch (direction[0]) {
			case 0:al_draw_bitmap(BMP_CHARACTER_UP, pos_player[0][0], pos_player[0][1], 0); break;
			case 1:al_draw_bitmap(BMP_CHARACTER_DOWN, pos_player[0][0], pos_player[0][1], 0); break;
			case 2:al_draw_bitmap(BMP_CHARACTER_LEFT, pos_player[0][0], pos_player[0][1], 0); break;
			case 3: al_draw_bitmap(BMP_CHARACTER_RIGHT, pos_player[0][0], pos_player[0][1], 0); break;
			}
			if (number_of_players[1] == true) {
				switch (direction[1]) {
				case 0:al_draw_bitmap(BMP_CHARACTER_UP_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 1:al_draw_bitmap(BMP_CHARACTER_DOWN_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 2:al_draw_bitmap(BMP_CHARACTER_LEFT_2, pos_player[1][0], pos_player[1][1], 0); break;
				case 3: al_draw_bitmap(BMP_CHARACTER_RIGHT_2, pos_player[1][0], pos_player[1][1], 0); break;
				}
			}
			create_enemy(Przeciwnik);
			for (int t = 0; t < 13; t++) {

				for (int i = 0; i < 28; i++) {
					if (objMap[t][i] == 1)
						al_draw_bitmap(BMP_TEXTURE_1, i*48, t*70, 0);
				}

			}
			al_flip_display();
		}

	}
}

int main(void) {
	ALLEGRO_MONITOR_INFO Monitor_info;
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	if (!al_init())
		return -1;
	if (!al_install_audio()) {
		return -1;
	}
	if (!al_init_acodec_addon()) {
		return -1;
	}
	if (!al_reserve_samples(1)) {
		return -1;
	}
	al_get_monitor_info(0, &Monitor_info);
	width = (int)Monitor_info.x2;
	height = (int)Monitor_info.y2;

	al_set_new_display_flags(ALLEGRO_FULLSCREEN_WINDOW);
	display = al_create_display(width, height);
	if (!display)
		return -1;
	sample = al_load_sample("dzwieki/main.wav");
	if (!sample) {
		return -1;
	}
	///////////// addony////////////////
	al_init_font_addon();
	al_install_mouse();
	al_init_ttf_addon();
	al_init_image_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	//ladowanie bitmap/////
	al_set_window_title(display, "Priceless Paper");
	BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
	BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.png");
	BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png");
	BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png");
	BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png");
	BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.PNG");
	BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ.png");
	BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png");
	BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
	BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WLACZONA.png");
	BMP_CHARACTER_UP = al_load_bitmap("studentbmp/polnoc.png");
	BMP_CHARACTER_DOWN = al_load_bitmap("studentbmp/poludnie.png");
	BMP_CHARACTER_RIGHT = al_load_bitmap("studentbmp/prawo.png");
	BMP_CHARACTER_LEFT = al_load_bitmap("studentbmp/lewo.png");
	BMP_BULLET = al_load_bitmap("studentbmp/atak_podstawowy.png");
	BMP_CHARACTER_UP_2 = al_load_bitmap("studentbmp/polnoc_2.png");
	BMP_CHARACTER_DOWN_2 = al_load_bitmap("studentbmp/poludnie_2.png");
	BMP_CHARACTER_RIGHT_2 = al_load_bitmap("studentbmp/prawo_2.png");
	BMP_CHARACTER_LEFT_2 = al_load_bitmap("studentbmp/lewo_2.png");
	BMP_BULLET_2 = al_load_bitmap("studentbmp/atak_podstawowy2.png");
	icon = al_load_bitmap("wejsciowe/ikona.png");
	al_set_display_icon(display, icon);
	al_hide_mouse_cursor(display);
	////////// rysowanie poczatkowego menu //////////////////
	al_draw_bitmap(BMP_START, 0, 0, 0);
	al_draw_bitmap(BMP_NEWGAME, width / 2 - 190, height / 2 - 40, 0);
	al_draw_bitmap(BMP_RECORDS, width / 2 - 190, height / 2 + 15, 0);
	al_draw_bitmap(BMP_OPTIONS, width / 2 - 190, height / 2 + 70, 0);
	al_draw_bitmap(BMP_CREDITS, width / 2 - 190, height / 2 + 125, 0);
	al_draw_bitmap(BMP_EXIT, width / 2 - 190, height / 2 + 180, 0);
	///////////////////// queue + timer 
	event_queue = al_create_event_queue();
	timer = al_create_timer(DeltaTime);
	/////////////// eventy
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);
	al_flip_display();
	al_stop_samples();
	//al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
	while (!left && !left_menu) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				if (!locked[0]) if (--menu[0] == -1) menu[0] = 0;
				if (locked[0]) if (--menu[1] == -1) menu[1] = 0;
				if (locked[0]) if (--menu[2] == -1) menu[2] = 0;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				if (!locked[0]) if (++menu[0] == 5) menu[0] = 4;
				if (locked[0]) if (++menu[1] == 2) menu[1] = 1;
				if (locked[0]) if (++menu[2] == 2) menu[2] = 1;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				if (remember == 2) {
					if (music_on) {
						al_destroy_bitmap(BMP_START);
						BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						al_draw_bitmap(BMP_START, 0, 0, 0);
						al_destroy_bitmap(BMP_MUSIC);
						BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
						al_draw_bitmap(BMP_MUSIC, width / 2 - 350, height / 2 - 40, 0);
						al_destroy_bitmap(BMP_MUSIC_CURRENT);
						BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WYLACZONA.png");
						al_draw_bitmap(BMP_MUSIC_CURRENT, width / 2 - 50, height / 2 - 40, 0);
						music_on = false;
						al_stop_samples();
						al_flip_display();
					}
				}
				break;

			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				if (remember == 2) {
					if (!music_on) {
						al_destroy_bitmap(BMP_START);
						BMP_START = al_load_bitmap("wejsciowe/tlo_podstawowe2.png");
						al_draw_bitmap(BMP_START, 0, 0, 0);
						al_destroy_bitmap(BMP_MUSIC);
						BMP_MUSIC = al_load_bitmap("wejsciowe/MUZYKA.png");
						al_draw_bitmap(BMP_MUSIC, width / 2 - 350, height / 2 - 40, 0);
						al_destroy_bitmap(BMP_MUSIC_CURRENT);
						BMP_MUSIC_CURRENT = al_load_bitmap("wejsciowe/WLACZONA.png");
						al_draw_bitmap(BMP_MUSIC_CURRENT, width / 2 - 50, height / 2 - 40, 0);
						al_flip_display();
						al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_LOOP, NULL);
						music_on = true;
					}
				}
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_ESCAPE:
				count_enter--;
				if (remember == 0 || remember == 2) remember = -10;
				if (count_enter == -1) {
					left = true;
				}
				if (count_enter == 0) {
					al_draw_bitmap(BMP_START, 0, 0, 0);
					locked[0] = false;
					menu[1] = menu[2] = 0;
				}
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_ENTER:
				count_enter++;
				switch (menu[0]) {
				case 0:
					if (!locked[0]) {
						is_back = false;
						locked[0] = true;
					}
					al_draw_bitmap(BMP_START, 0, 0, 0);
					al_draw_bitmap(BMP_ONE_PLAYER, width / 2 - 300, height / 2 - 40, 0);
					al_draw_bitmap(BMP_TWO_PLAYERS, width / 2 - 300, height / 2 + 15, 0);
					al_flip_display();
					remember = 0;
					if (count_enter > 1) {
						switch (menu[1]) {
						case 0: number_of_players[0] = true; number_of_players[1] = false; left_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0)); pre_start_game(); break;
						case 1:  number_of_players[0] = true; number_of_players[1] = true; left_menu = true; in_game = true; al_clear_to_color(al_map_rgb(0, 0, 0));  pre_start_game(); break;
						}
						count_enter--;
					}
					if (count_enter_choice > 1) {
						left_menu = true;
					}
					break;
				case 1: count_enter--; break;
				case 2:
					remember = 2;
					if (!locked[0]) {
						is_back = false;
						locked[0] = true;
					}
					al_draw_bitmap(BMP_START, 0, 0, 0);
					al_draw_bitmap(BMP_MUSIC, width / 2 - 350, height / 2 - 40, 0);
					al_draw_bitmap(BMP_MUSIC_CURRENT, width / 2 - 50, height / 2 - 40, 0);
					al_flip_display();
					if (++count_enter > 1) count_enter = 1;
					break;

				case 3:  count_enter--;  break;
				case 4: left = true; break;
				}
			}
		}
		else if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (keys[DOWN] == false && !locked[0] && !left_menu) {
				switch (menu[0]) {
				case 0:  al_destroy_bitmap(BMP_NEWGAME); al_destroy_bitmap(BMP_RECORDS);  BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA_WYBRANE.PNG"); BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.png"); break;
				case 1:  al_destroy_bitmap(BMP_RECORDS); al_destroy_bitmap(BMP_NEWGAME); al_destroy_bitmap(BMP_OPTIONS); BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY_WYBRANE.png"); BMP_NEWGAME = al_load_bitmap("wejsciowe/NOWA_GRA.png"); BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png"); break;
				case 2:  al_destroy_bitmap(BMP_CREDITS); al_destroy_bitmap(BMP_RECORDS); al_destroy_bitmap(BMP_OPTIONS); BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE_WYBRANE.png"); BMP_RECORDS = al_load_bitmap("wejsciowe/REKORDY.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				case 3:  al_destroy_bitmap(BMP_OPTIONS); al_destroy_bitmap(BMP_CREDITS); al_destroy_bitmap(BMP_EXIT); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS_WYBRANE.png"); BMP_OPTIONS = al_load_bitmap("wejsciowe/OPCJE.png"); BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE.png"); break;
				case 4:  al_destroy_bitmap(BMP_EXIT); al_destroy_bitmap(BMP_CREDITS);  BMP_EXIT = al_load_bitmap("wejsciowe/ZAKONCZ_GRE_WYBRANE.png"); BMP_CREDITS = al_load_bitmap("wejsciowe/CREDITS.png"); break;
				}
				al_draw_bitmap(BMP_NEWGAME, width / 2 - 190, height / 2 - 40, 0);
				al_draw_bitmap(BMP_RECORDS, width / 2 - 190, height / 2 + 15, 0);
				al_draw_bitmap(BMP_OPTIONS, width / 2 - 190, height / 2 + 70, 0);
				al_draw_bitmap(BMP_CREDITS, width / 2 - 190, height / 2 + 125, 0);
				al_draw_bitmap(BMP_EXIT, width / 2 - 190, height / 2 + 180, 0);
			}
			if (keys[DOWN] == false && locked[0] && remember == 0 && !left_menu) {
				switch (menu[1]) {
				case 0:  al_destroy_bitmap(BMP_ONE_PLAYER); al_destroy_bitmap(BMP_TWO_PLAYERS);  BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ_WYBRANE.PNG"); BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY.png"); break;
				case 1:  al_destroy_bitmap(BMP_ONE_PLAYER); al_destroy_bitmap(BMP_TWO_PLAYERS);   BMP_ONE_PLAYER = al_load_bitmap("wejsciowe/JEDEN_GRACZ.PNG"); BMP_TWO_PLAYERS = al_load_bitmap("wejsciowe/DWOCH_GRACZY_WYBRANE.png");  break;
				}
				al_draw_bitmap(BMP_ONE_PLAYER, width / 2 - 300, height / 2 - 40, 0);
				al_draw_bitmap(BMP_TWO_PLAYERS, width / 2 - 300, height / 2 + 15, 0);
			}
			if (keys[DOWN] == false && locked[0] && remember == 2 && !left_menu) {

			}
			al_flip_display();
		}
	}
	al_destroy_bitmap(BMP_START);
	al_destroy_bitmap(BMP_NEWGAME);
	al_destroy_bitmap(BMP_CREDITS);
	al_destroy_bitmap(BMP_EXIT);
	al_destroy_bitmap(BMP_OPTIONS);
	al_destroy_bitmap(BMP_RECORDS);
	al_destroy_bitmap(BMP_ONE_PLAYER);
	al_destroy_sample(sample);
	al_destroy_bitmap(BMP_TWO_PLAYERS);
	al_destroy_bitmap(BMP_MUSIC);
	al_destroy_bitmap(BMP_MUSIC_CURRENT);
	al_destroy_bitmap(BMP_CHARACTER_UP);
	al_destroy_bitmap(BMP_CHARACTER_DOWN);
	al_destroy_bitmap(BMP_CHARACTER_LEFT);
	al_destroy_bitmap(BMP_CHARACTER_RIGHT);
	al_destroy_bitmap(BMP_CHARACTER_UP_2);
	al_destroy_bitmap(BMP_CHARACTER_DOWN_2);
	al_destroy_bitmap(BMP_CHARACTER_LEFT_2);
	al_destroy_bitmap(BMP_CHARACTER_RIGHT_2);
	al_destroy_bitmap(BMP_TEXTURE_1);
	al_destroy_bitmap(BMP_TEXTURE_2);
	al_destroy_bitmap(BMP_TEXTURE_3);
	al_destroy_bitmap(BMP_TEXTURE_4);
	al_destroy_bitmap(BMP_TEXTURE_5);
	al_destroy_bitmap(BMP_SEMESTER_OVER);
	al_destroy_bitmap(BMP_BULLET);
	al_destroy_bitmap(BMP_BULLET_2);
	al_destroy_bitmap(BMP_ENEMY);
	al_destroy_bitmap(BMP_ENEMY_BOSS);
	al_destroy_bitmap(BMP_BULLET_ENEMY);
	al_destroy_bitmap(BMP_SYMBOL);
	al_destroy_bitmap(icon);
	al_destroy_event_queue(event_queue);
}