#define is_Down(b) input->buttons[b].is_down
#define pressed(b) (input->buttons[b].is_down && input->buttons[b].changed)
#define released(b) (!input->buttons[b].is_down && input->buttons[b].changed)

float player_p = 0.f, player_dp;
float player_2p = 0.f, player_2dp;

float arena_half_size_x = 90, arena_half_size_y = 45;
float player_half_size_x = 2.5f, player_half_size_y = 12;

float ball_x, ball_y, ball_dp_x = 100, ball_dp_y, ball_half_size;

u32 player1_score, player2_score;

void HandleCollisionDetection() {
	if (player_p + player_half_size_y > arena_half_size_y) {
		player_p = arena_half_size_y - player_half_size_y;
	}
	else if (player_p - player_half_size_y < -arena_half_size_y) {
		player_p = -arena_half_size_y + player_half_size_y;
	}

	if (player_2p + player_half_size_y > arena_half_size_y) {
		player_2p = arena_half_size_y - player_half_size_y;
	}
	else if (player_2p - player_half_size_y < -arena_half_size_y) {
		player_2p = -arena_half_size_y + player_half_size_y;
	}
}

internal void SimulateGame(Input* input, float delta) {
	Clear_Screen(0xFFFFFF);

	Draw_Rect(0, 0, arena_half_size_x, arena_half_size_y, 0x000000);

	float player_dpp = 0.f;
	float player_2dpp = 0.f;

	if (is_Down(BUTTON_UP)) player_dpp += 2000;
	if (is_Down(BUTTON_DOWN)) player_dpp -= 2000;

	if (is_Down(BUTTON_W)) player_2dpp += 2000;
	if (is_Down(BUTTON_S)) player_2dpp -= 2000;

	player_dpp -= player_dp * 10.f;

	player_p = player_p + player_dp * delta + player_dpp * delta * delta * .5f;
	player_dp = player_dp + player_dpp * delta;

	player_2dpp -= player_2dp * 10.f;

	player_2p = player_2p + player_2dp * delta + player_2dpp * delta * delta * .5f;
	player_2dp = player_2dp + player_2dpp * delta;
	

	HandleCollisionDetection();

	ball_x += ball_dp_x * delta;
	ball_y += ball_dp_y * delta;

	Draw_Rect(ball_x, ball_y, 1, 1, 0xFFFFFF);

	if (ball_x + ball_half_size > 80 - player_half_size_x &&
		ball_x - ball_half_size < 80 + player_half_size_x &&
		ball_y + ball_half_size > player_p - player_half_size_y &&
		ball_y + ball_half_size < player_p + player_half_size_y) {

		ball_x = 80 - player_half_size_x - ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = (ball_y - player_p) * 2 + player_dp * .75f;
	}
	else if (ball_x + ball_half_size > -80 - player_half_size_x &&
		ball_x - ball_half_size < -80 + player_half_size_x &&
		ball_y + ball_half_size > player_2p - player_half_size_y &&
		ball_y + ball_half_size < player_2p + player_half_size_y) {

		ball_x = -80 + player_half_size_x + ball_half_size;
		ball_dp_x *= -1;
		ball_dp_y = (ball_y - player_2p) * 2 + player_2dp * .75f;
	}

	if (ball_y + ball_half_size > arena_half_size_y) {
		ball_y = arena_half_size_y - ball_half_size;
		ball_dp_y *= -1;
	}
	else if (ball_y + ball_half_size < -arena_half_size_y) {
		ball_y = -arena_half_size_y + ball_half_size;
		ball_dp_y *= -1;
	}

	if (ball_x + ball_half_size > arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_x = 0;
		ball_y = 0;
		player1_score++;
	}
	else if (ball_x - ball_half_size < -arena_half_size_x) {
		ball_dp_x *= -1;
		ball_dp_y = 0;
		ball_x = 0;
		ball_y = 0;
		player2_score++;
	}

	float at_x = -80;

	for (int i = 0; i < player1_score; i++) {
		Draw_Rect(at_x, 47.f, 1.f, 1.f, 0x000000);
		at_x += 2.5f;
	}

	at_x = 80;

	for (int i = 0; i < player2_score; i++) {
		Draw_Rect(at_x, 47.f, 1.f, 1.f, 0x000000);
		at_x -= 2.5f;
	}

	Draw_Rect(80, player_p, 2.5, 12, 0xFFFFFF);
	Draw_Rect(-80, player_2p, 2.5, 12, 0xFFFFFF);
}