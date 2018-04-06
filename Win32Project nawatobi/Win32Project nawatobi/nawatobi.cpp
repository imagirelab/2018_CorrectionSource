#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

class VECTOR2D
{
public:
	float x = 0.0f;
	float y = 0.0f;
};
VECTOR2D operator+(const VECTOR2D& u,const VECTOR2D& v);

class Player
{
private:
	int x = 320;
	int y = 400;
	int life = 1;
	int image[2];
	int death = 0;
	int in = 0;
	int on = 0;
	int onGround = FALSE;
	VECTOR2D inerVec;
public:
	Player(){}
	~Player(){}
	
	void ApplyGravity(const VECTOR2D &GravVec)
	{
		if (!onGround) inerVec += GravVec;
	}
};

class PlayerList
{
private:
	int num_ = 0;
	Player *player_=nullptr;
public:
	PlayerList(int num){
		num_ = num;
		player_ = new Player[num];
	}
	~PlayerList(){
		delete[] player_;
		num_ = 0;
	}
	
	void ApplyGravity(const VECTOR2D &GravVec)
	{
		for( auto&& player : player_ )
			player.ApplyGravity(GravVec);
		}
	}
};

//画像読み込み
void LoadResource(int *nawa, Player *player, Player *player2, Player *player3, int *color, int *font1, int *font2, int *font3, int *font4, int *font5, int *number1, int *number2, int *bomb, int *z, int *title, int *attend)
{
	player->image[0] = LoadGraph(".\\media\\image\\1stand.png");
	player->image[1] = LoadGraph(".\\media\\image\\1jump.png");
	player2->image[0] = LoadGraph(".\\media\\image\\2stand.png");
	player2->image[1] = LoadGraph(".\\media\\image\\2jump.png");
	player3->image[0] = LoadGraph(".\\media\\image\\3stand.png");
	player3->image[1] = LoadGraph(".\\media\\image\\3jump.png");

	LoadDivGraph(".\\media\\image\\nawa.png", 16, 1, 16, 585, 172.625, nawa);
	LoadDivGraph(".\\media\\image\\font.png", 15, 1, 15, 600, 60, font1);
	LoadDivGraph(".\\media\\image\\font2.png", 68, 4, 17, 240, 60, font2);
	LoadDivGraph(".\\media\\image\\font3.png", 42, 3, 14, 300, 60, font3);
	LoadDivGraph(".\\media\\image\\font4.png", 35, 5, 7, 120, 60, font4);
	LoadDivGraph(".\\media\\image\\font5.png", 25, 5, 5, 180, 60, font5);
	LoadDivGraph(".\\media\\image\\number.png", 20, 10, 2, 60, 60, number1);
	LoadDivGraph(".\\media\\image\\number2.png", 6, 6, 1, 30, 60, number2);
	LoadDivGraph(".\\media\\image\\bomb.jpg", 16, 8, 2, 128, 128, bomb);
	LoadDivGraph(".\\media\\image\\Z.jpg", 1, 1, 1, 47, 51, z);

	LoadDivGraph(".\\media\\image\\title.jpg", 1, 1, 1, 640, 480, title);
	LoadDivGraph(".\\media\\image\\attend.png", 1, 1, 1, 640, 480, attend);


	color[0] = GetColor(255, 255, 255);
	color[1] = GetColor(0, 255, 0);
}

//タイトル
void Title(char *keyBuf, int *title, int *font2, int *z, int *start, int *scene)
{

	int end, second;
	DrawGraph(0, 0, *title, FALSE);

	end = GetNowCount();
	second = (end - *start) / 1000.0;

	if (second % 2 == 0)
	{
		DrawRotaGraph(300, 360, 1, 0, font2[14], TRUE);
		DrawRotaGraph(450, 355, 1, 0, *z, TRUE);
	}

	GetHitKeyStateAll(keyBuf);
	if (keyBuf[KEY_INPUT_Z] == 1)
		*scene = 2;
}

//参加人数確認
void Attend(char *keyBuf, Player *player, Player *player2, Player *player3, int *attend, int *font4, int *start, int *scene, int *check)
{
	DrawGraph(0, 0, *attend, FALSE);

	GetHitKeyStateAll(keyBuf);
	if (keyBuf[KEY_INPUT_SPACE] == 1)
	{
		player->in = 1;
		player->on = 1;

	}
	if (keyBuf[KEY_INPUT_1] == 1)
	{
		player2->in = 1;
		player2->on = 1;
	}
	if (keyBuf[KEY_INPUT_RETURN] == 1)
	{
		player3->in = 1;
		player3->on = 1;
	}

	if (player->on == 1)
	{
		DrawGraph(260, 250, font4[4], TRUE);
		DrawGraph(287, 300, player->image[0], TRUE);
	}
	if (player2->on == 1)
	{
		DrawGraph(100, 250, font4[4], TRUE);
		DrawGraph(127, 300, player2->image[0], TRUE);
	}
	if (player3->on == 1)
	{
		DrawGraph(420, 250, font4[4], TRUE);
		DrawGraph(447, 300, player3->image[0], TRUE);
	}

	if (keyBuf[KEY_INPUT_Z] == 1 && *check == 1)
	{
		*start = GetNowCount();
		*scene = 3;
	}

	if (keyBuf[KEY_INPUT_Z] != 1)
	{
		if (player->on == 1 || player2->on == 1 || player3->on == 1)
			*check = 1;
	}
}

//時間計算
void CheackTime(int start, int end, int interval, int *second)
{
	end = GetNowCount();
	interval = end - start;
	*second = interval / 1000.0;
}

//スタートシーン
void StartScene(int *font3, int *font4, int *number1, int *number2, int *second, int *scene)
{
	int i = 0;
	if (*second >= 0 && *second < 2)
	{
		DrawRotaGraph(310, 230, 1.0, 0, font3[25], TRUE);
		DrawRotaGraph(460, 230, 1.0, 0, number1[10], TRUE);
	}
	if (*second >= 2 && *second < 3)
	{
		DrawRotaGraph(310, 230, 1.0, 0, font4[0], TRUE);
		DrawRotaGraph(400, 230, 1.0, 0, number2[5], TRUE);
	}
	if (*second >= 3)
		*scene = 4;
}

//縦移動
void CharaMove(Player *player, Player *player2, Player *player3)
{
	player->y += player->inerVec;
	player2->y += player2->inerVec;
	player3->y += player3->inerVec;
}

//縄の挙動
void NawaMove(int *anime, int *change, int *speed, int *score)
{
	int i;

	//縄の基本動作
	if (*change == TRUE)
	{
		(*anime)++;
		if (*anime > ((*speed) * 16) - 2)
		{
			*change = FALSE;
			(*score)++;
		}
	}
	if (*change == FALSE)
	{
		(*anime)--;
		if (*anime < (*speed - 1))
		{
			*change = TRUE;

		}
	}
}

//スピード変化
void SpeedChange(int *change, int *speed, int *score)
{
	if (5 == (*score) && *change == TRUE)
		*speed = 4;
	if (10 == (*score) && *change == TRUE)
		*speed = 3;
	if (15 == (*score) && *change == TRUE)
		*speed = 2;
	if (20 == (*score) && *change == TRUE)
		*speed = 4;
	if (25 == (*score) && *change == TRUE)
		*speed = 2;
	if (30 == (*score) && *change == TRUE)
		*speed = 3;
	if (35 == (*score) && *change == TRUE)
		*speed = 2;
	if (40 == (*score) && *change == TRUE)
		*speed = 5;
	if (45 == (*score) && *change == TRUE)
		*speed = 3;
	if (50 == (*score) && *change == TRUE)
		*speed = 4;
	if (55 == (*score) && *change == TRUE)
		*speed = 2;
	if (65 == (*score) && *change == TRUE)
		*speed = 5;
	if (70 == (*score) && *change == TRUE)
		*speed = 2;
	if (75 == (*score) && *change == TRUE)
		*speed = 1;

}

//ボタン入力(ジャンプ)
void CharaJump(char *keyBuf, VECTOR2D jumpVec, Player *player, Player *player2, Player *player3)
{
	GetHitKeyStateAll(keyBuf);
	if (keyBuf[KEY_INPUT_SPACE] == 1 && player->onGround == TRUE)
	{
		player->inerVec = jumpVec.y;
		player->onGround = FALSE;
	}
	if (keyBuf[KEY_INPUT_1] == 1 && player2->onGround == TRUE)
	{
		player2->inerVec = jumpVec.y;
		player2->onGround = FALSE;
	}
	if (keyBuf[KEY_INPUT_RETURN] == 1 && player3->onGround == TRUE)
	{
		player3->inerVec = jumpVec.y;
		player3->onGround = FALSE;
	}
}

//地面の判定
void CheackonGround(Player *player, Player *player2, Player *player3)
{

	if (player->y > 400)
	{
		player->y = 400;
		player->onGround = TRUE;
		player->inerVec = 0;
	}
	if (player2->y > 400)
	{
		player2->y = 400;
		player2->onGround = TRUE;
		player2->inerVec = 0;
	}
	if (player3->y > 400)
	{
		player3->y = 400;
		player3->onGround = TRUE;
		player3->inerVec = 0;
	}
}

//死亡判定
void CheackDeath(Player *player, Player *player2, Player *player3, int *anime, int *speed, int *scene, int *start)
{
	int getnow = (*anime);
	if (player->onGround == TRUE)
	{
		if (((*speed) * 16) - 10 < getnow && player->in == 1)
		{
			player->life = 0;
			*scene = 5;
			*start = GetNowCount();
		}
	}
	if (player2->onGround == TRUE)
	{
		if (((*speed) * 16) - 10 < getnow && player2->in == 1)
		{
			player2->life = 0;
			*scene = 5;
			*start = GetNowCount();
		}
	}
	if (player3->onGround == TRUE)
	{
		if (((*speed) * 16) - 10 < getnow && player3->in == 1)
		{
			player3->life = 0;
			*scene = 5;
			*start = GetNowCount();
		}
	}
}

//画像表示
void DrawScreen(Player player, Player *player2, Player *player3, int *color, int *nawa, int *anime, int *speed, int *bomb)
{
	//地面と背景の表示
	DrawBox(0, 0, 640, 480, color[0], TRUE);
	DrawBox(0, 400, 640, 480, color[1], TRUE);

	//主人公の表示
	if (player.in == 1)
	{
		if (player.life == 1)
		{
			if (player.onGround == FALSE)
				DrawGraph(player.x - 33, player.y - 100, player.image[1], TRUE);
			else
				DrawTurnGraph(player.x - 33, player.y - 100, player.image[0], TRUE);
		}
		else
		{
			if (player.death < 17)
				DrawGraph(player.x - 33, player.y - 100, bomb[player.death], TRUE);
			(player.death)++;
		}
	}
	if (player2->in == 1)
	{
		if (player2->life == 1)
		{
			if (player2->onGround == FALSE)
				DrawGraph(player2->x - 131, player2->y - 100, player2->image[1], TRUE);
			else
				DrawTurnGraph(player2->x - 131, player2->y - 100, player2->image[0], TRUE);
		}
		else
		{
			if ((player2->death) < 17)
				DrawGraph(player.x - 131, player.y - 100, bomb[player2->death], TRUE);
			(player2->death)++;
		}
	}
	if (player3->in == 1)
	{
		if (player3->life == 1)
		{
			if (player3->onGround == FALSE)
				DrawGraph(player3->x + 65, player3->y - 100, player3->image[1], TRUE);
			else
				DrawTurnGraph(player3->x + 65, player3->y - 100, player3->image[0], TRUE);
		}
		else
		{
			if ((player3->death) < 17)
				DrawGraph(player3->x - 65, player3->y - 100, bomb[player.death], TRUE);
			(player3->death)++;
		}
	}

	//なわの表示
	DrawGraph(27, 235, nawa[(*anime) / (*speed)], TRUE);
}

//ゲームオーバー
void GameOver(int *score, int *font1, int *font3, int *number1, int *start, int *scene, int *check, int *anime, int *change, int *speed, struct CHARADATA *player, CHARADATA *player2, CHARADATA *player3)
{
	int i, j, end, second;
	i = *score % 10;
	j = *score / 10;

	end = GetNowCount();
	second = (end - *start) / 1000.0;

	DrawRotaGraph(310, 150, 1.0, 0, font1[3], TRUE);
	DrawRotaGraph(200, 230, 1.0, 0, font3[17], TRUE);
	DrawRotaGraph(450, 230, 1.0, 0, number1[j], TRUE);
	DrawRotaGraph(510, 230, 1.0, 0, number1[i], TRUE);

	if (second > 20)
	{
		*anime = 0;
		*change = TRUE;
		*speed = 5;
		*score = 0;
		*scene = 1;
		*check = 0;

		player->life = 1;
		player2->life = 1;
		player3->life = 1;
		player->death = 0;
		player2->death = 0;
		player3->death = 0;
		player->in = 0;
		player2->in = 0;
		player3->in = 0;
		player->on = 0;
		player2->on = 0;
		player3->on = 0;
		player->onGround = FALSE;
		player2->onGround = FALSE;
		player3->onGround = FALSE;
		player->inerVec = 0;
		player2->inerVec = 0;
		player3->inerVec = 0;
	}
}

//ソース本文
int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	PlayerList *players = new PlayerList(3);
	VECTOR2D jumpVec = { -15 };
	VECTOR2D GraVec = { 0.8 };
	int nawa[16], color[2], font1[15], font2[68], font3[42], font4[35], font5[25], number1[20], number2[6], bomb[16], title, attend, z;
	int interval = 0, end = 0, start, second;
	int anime = 0, change = TRUE, speed = 5, score = 0, scene = 1, check = 0;
	char keyBuf[256];

	srand((unsigned int)time(NULL));
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);
	start = GetNowCount();
	LoadResource(nawa, &player, &player2, &player3, color, font1, font2, font3, font4, font5, number1, number2, bomb, &z, &title, &attend);


	while (ProcessMessage() == 0)
	{
		if (scene == 3 || scene == 4 || scene == 5)
		{
			CheackTime(start, end, interval, &second);
			if (scene == 4)
			{
				CheackDeath(&player, &player2, &player3, &anime, &speed, &scene, &start);

				NawaMove(&anime, &change, &speed, &score);
				SpeedChange(&change, &speed, &score);
			}
			CheackonGround(&player, &player2, &player3);

			players->ApplyGravity();
			Gravity(&player, &player2, &player3, GraVec);
			CharaMove(&player, &player2, &player3);
			CharaJump(keyBuf, jumpVec, &player, &player2, &player3);

			DrawScreen(player, &player2, &player3, color, nawa, &anime, &speed, bomb);
			if (scene == 3)
				StartScene(font3, font4, number1, number2, &second, &scene);
			if (scene == 5)
				GameOver(&score, font1, font3, number1, &start, &scene, &check, &anime, &change, &speed, &player, &player2, &player3);
		}
		if (scene == 1)
			Title(keyBuf, &title, font2, &z, &start, &scene);
		if (scene == 2)
			Attend(keyBuf, &player, &player2, &player3, &attend, font4, &start, &scene, &check);

		ScreenFlip();
		if (keyBuf[KEY_INPUT_ESCAPE])
			break;
	}
	DxLib_End();
	
	delete players;
	return 0;
}
