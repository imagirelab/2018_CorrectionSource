#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

// 画像を指定するID
enum class RENDER_IMAGE{
	PLAYER1 = 0;
	PLAYER1_NUM = 2;
	PLAYER2 = PLAYER1 + PLAYER1_NUM;
	PLAYER2_NUM = 2;
	PLAYER3 = PLAYER2 + PLAYER2_NUM;
	PLAYER3_NUM = 2;
	NAWA = PLAYER3 + PLAYER3_NUM;
	NAWA_NUM = 16;
	FONT1 = NAWA + NAWA_NUM;
	FONT1_NUM = 15;
	FONT2 = FONT1 + FONT1_NUM;
	FONT2_NUM = 68;
	FONT3 = FONT2 + FONT2_NUM;
	FONT3_NUM = 42;
	FONT4 = FONT3 + FONT3_NUM;
	FONT4_NUM = 35;
	FONT5 = FONT4 + FONT4_NUM;
	FONT5_NUM = 25;
	NUMBER1 = FONT5 + FONT5_NUM;
	NUMBER1_NUM = 20;
	NUMBER2 = NUMBER1 + NUMBER1_NUM;
	NUMBER2_NUM = 6;
	BOMB = NUMBER2 + NUMBER2_NUM;
	BOMB_NUM = 16;
	TITLE = BOMB + BOMB_NUM;
	TITLE_NUM = 1;
	ATTEND = TITLE + TITLE_NUM;
	ATTEND_NUM = 1;
	Z = ATTEND + ATTEND_NUM;
	Z_NUM = 1;
	
	IMAGE_NUM = Z + Z_NUM;
};

enum class RENDER_COLOR{
	BG = 0,
	GROUND = 1,
};


struct VECTOR2D
{
	float x;
	float y;
};
VECTOR2D operator+(const VECTOR2D& u,const VECTOR2D& v);

//時間計算
int CheackTime(int start)
{
	int end = GetNowCount();
	int interval = end - start;
	return interval / 1000;
}

class Player
{
private:
	VECTOR2D pos = {320.0f, 320.0f};
	VECTOR2D velocity = {0.0f, 0.0f};
	int attend_ = 0;
	int life = 1;
	int death = 0;
	bool onGround = FALSE;
	unsigned char jump_key_ = KEY_INPUT_SPACE;
public:
	Player(){}
	~Player(){}
	
	void setJumpKey(unsigned char key)
	{
		jump_key_ = key;
	}
	
	void Initialize()
	{
		life = 1;
		death = 0;
		attend_ = 0;
		onGround = FALSE;
		velocity = {0.0f, 0.0f};
	}
	
	bool CheckAttend(keyBuf)
	{
		if (keyBuf[jump_key_] == 1)
		{
			attend_ = 1;
		}
	}
	
	void Move()
	{
		const VECTOR2D Gravity = {0.0f, 0.8f};
		if (!onGround) velocity += GravVec;
		
		player->y += player->velocity;
	}
	
	void CheackonGround()
	{
		if (pos.y > 400.0f)
		{
			pos.y = 400.0f;
			onGround = TRUE;
			velocity.y = 0.0f;
		}
	}
	
	void Jump(char *keyBuf)
	{
		if (keyBuf[jump_key_] == 1 && player->onGround)
		{
			const VECTOR2D jumpVec = {0.0f, -15.0f };
			player->velocity = jumpVec;
			player->onGround = false;
		}
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
	
	bool setJumpKey(unsigned int idx, unsigned char key)
	{
		if(num_ <= idx) return false; // 存在しないプレイヤーを指定
		player_[idx].setJumpKey(key);
		
		return true;
	}

	void Initialize()
	{
		for( auto&& player : player_ )
		{
			player.Initialize();
		}
	}

	void Update()
	{
		for( auto&& player : player_ )
		{
			player.Move();
		}
	}

	void Jump()
	{
		for( auto&& player : player_ )
		{
			player.Jump();
		}
	}

	void CheackonGround()
	{
		for( auto&& player : player_ )
		{
			player.CheackonGround();
		}
	}
};

enum class SCENE_NAME{
	TITLE = 0,
	ATTEND,
	START,
	PLAY,
	GAMEOVER,
	
	NUM,
	
	INVALID,
};

// 各シーンのFSM
class FSM
{
public:
	FSM(){}	
	virtual ~FSM(){}
	
	virtual void Initialize() = 0;
	virtual SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) = 0;
};

class FSM_TITLE : public FSM
{
	int start_;
public:
	FSM_TITLE(){}	
	~FSM_TITLE(){}
	
	void Initialize() override 
	{
		*anime = 0;
		*change = TRUE;
		*speed = 5;
		*score = 0;
		*check = 0;
		
		players->Initialize();
		start_ = GetNowCount();
	}
	
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override
	{
		render.Draw(0,0,RENDER_IMAGE::TITLE);

		int scene_time = CheackTime(start_);

		if (scene_time % 2 == 0)
		{
			render.Draw(300, 360, RENDER_IMAGE::FONT2, 14);
			render.Draw(450, 355, RENDER_IMAGE::Z);
		}

		if (keyBuf[KEY_INPUT_Z] == 1)
			return SCENE_NAME::ATTEND;
		}
	
	return SCENE_NAME::INVALID;
};

class FSM_ATTEND : public FSM
{
public:
	FSM_ATTEND(){}	
	~FSM_ATTEND(){}
	
	void Initialize() override {}
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		// char *keyBuf, Playerlist *players, int *attend, int *font4, int *scene)
		bool all_attended = players->CheckAttend(keyBuf);

		render.Draw(0, 0, RENDER_IMAGE::ATTEND);

		if (player->attend_ == 1)
		{
			render.Draw(260, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(287, 300,RENDER_IMAGE::PLAYER1, 0);
		}
		if (player2->attend_ == 1)
		{
			render.Draw(100, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(127, 300,RENDER_IMAGE::PLAYER2, 0);
		}
		if (player3->attend_ == 1)
		{
			render.Draw(420, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(447, 300,RENDER_IMAGE::PLAYER3, 0);
		}

		if (keyBuf[KEY_INPUT_Z] == 1 && all_attended)
		{
			return SCENE_NAME::START;
		}
		
		return SCENE_NAME::INVALID;
	}
};

class FSM_IN_GAME : public FSM
{
protected:
	int start_;

	void DrawScreen(Renderer &render)
	{
		//地面と背景の表示
		render.DrawBox(0, 0, 640, 480, RENDER_COLOR::BG);
		render.DrawBox(0, 400, 640, 480, RENDER_COLOR::GROUND, TRUE);

		//主人公の表示
		if (player.attend_ == 1)
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
		if (player2->attend_ == 1)
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
		if (player3->attend_ == 1)
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

public:
	FSM_START(){}	
	~FSM_START(){}
	
	void Initialize() override {
		start_ = GetNowCount();
	};
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		players->CheackonGround();

		players->Update();
		players->Jump(keyBuf);

		DrawScreen(player, &player2, &player3, color, nawa, &anime, &speed, bomb);
	}
};

//スタートシーン
class FSM_START : public FSM_IN_GAME
{
public:
	FSM_START(){}	
	~FSM_START(){}
	
	void Initialize() override 
	{
	}
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		// int *font3, int *font4, int *number1, int *number2
		

		FSM_IN_GAME::Updada();
		
		int time = CheackTime(start_);
		if (3 <= time){
			return SCENE_NAME::PLAY;
		}else
		if(2 <= time){
			DrawGraph(310, 230, font4[0], TRUE);
			DrawGraph(400, 230, number2[5], TRUE);
		}else {
			DrawGraph(310, 230, font3[25], TRUE);
			DrawGraph(460, 230, number1[10], TRUE);
		}
		return SCENE_NAME::INVALID;
	}
};

class FSM_PLAY : public FSM_IN_GAME
{
public:
	FSM_PLAY(){}	
	~FSM_PLAY(){}
	
	void Initialize() override {}
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		SCENE_NAME ret = SCENE_NAME::INVALID;
		
		int getnow = (*anime);
		if (player->onGround == TRUE)
		{
			if (((*speed) * 16) - 10 < getnow && player->attend_ == 1)
			{
				player->life = 0;
				ret = SCENE_NAME::INVALID;
			}
		}
		if (player2->onGround == TRUE)
		{
			if (((*speed) * 16) - 10 < getnow && player2->attend_ == 1)
			{
				player2->life = 0;
				ret = SCENE_NAME::INVALID;
			}
		}
		if (player3->onGround == TRUE)
		{
			if (((*speed) * 16) - 10 < getnow && player3->attend_ == 1)
			{
				player3->life = 0;
				ret = SCENE_NAME::INVALID;
			}
		}

		NawaMove(&anime, &change, &speed, &score);
		ChangeSpeed(&change, &speed, &score);

		FSM_IN_GAME::Updada();
		
		return ret;
	}
};

class FSM_GAMEOVER : public FSM_IN_GAME
{
private:
	int start_;
public:
	FSM_GAMEOVER(){}	
	~FSM_GAMEOVER(){}
	
	void Initialize() override {
		start_ = GetNowCount();
	}
	
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
// int score, int *font1, int *font3, int *number1
		FSM_IN_GAME::Updada();		
		
		int digit0 = score % 10;
		int digit1 = score / 10;

		DrawGraph(310, 150, font1[3], TRUE);
		DrawGraph(200, 230, font3[17], TRUE);
		DrawGraph(450, 230, number1[digit1], TRUE);
		DrawGraph(510, 230, number1[digit0], TRUE);

		if (CheackTime(start_) > 20)
		{
			return SCENE_NAME::TITLE;
		}
		
		return SCENE_NAME::TITLE;
	}
};

// シーン管理
class Scene{
	FSM **fsm_[SCENE_NAME::NUM];
	FSM *current_;
public:
	Scene(){
		// SCENE_NAME の番号と合わせる
		fsm_[0] = new FSM_TITLE();
		fsm_[1] = new FSM_ATTEND();
		fsm_[2] = new FSM_START();
		fsm_[3] = new FSM_PLAY();
		fsm_[4] = new FSM_GAMEOVER();
		
		current_ = fsm_[0];
		current_->Initialize();
	}
	~Scene(){
		for( auto&& fsm : fsm_ )
		{
			delete fsm;
		}
	}
	
	void Update(PlayerList *players, Renderer &render, const char *keyBuf)
	{
		
		SCENE_NAME next = current_->Update();
		
		if(next != SCENE_NAME::INVALID && next < SCENE_NAME::NUM){
			current_ = fsm_[next];
			current_->Initialize();
		}
	}
};

class Renderer
{
private:
	int image[RENDER_IMAGE::IMAGE_NUM];
	int color[2];
public:
	renderer(){}
	~renderer(){}
	
	void Initialize()
	{
		//画像読み込み
		image_[RENDER_IMAGE::PLAYER1 + 0] = LoadGraph(".\\media\\image\\1stand.png");
		image_[RENDER_IMAGE::PLAYER1 + 1] = LoadGraph(".\\media\\image\\1jump.png");
		image_[RENDER_IMAGE::PLAYER1 + 0] = LoadGraph(".\\media\\image\\2stand.png");
		image_[RENDER_IMAGE::PLAYER2 + 1] = LoadGraph(".\\media\\image\\2jump.png");
		image_[RENDER_IMAGE::PLAYER3 + 1] = LoadGraph(".\\media\\image\\3stand.png");
		image_[RENDER_IMAGE::PLAYER3 + 1] = LoadGraph(".\\media\\image\\3jump.png");

		LoadDivGraph(".\\media\\image\\nawa.png", 16, 1, 16, 585, 172.625, image_+RENDER_IMAGE::NAWA);
		LoadDivGraph(".\\media\\image\\font.png", 15, 1, 15, 600, 60, image_+RENDER_IMAGE::FONT1);
		LoadDivGraph(".\\media\\image\\font2.png", 68, 4, 17, 240, 60, image_+RENDER_IMAGE::FONT2);
		LoadDivGraph(".\\media\\image\\font3.png", 42, 3, 14, 300, 60, image_+RENDER_IMAGE::FONT3);
		LoadDivGraph(".\\media\\image\\font4.png", 35, 5, 7, 120, 60, image_+RENDER_IMAGE::FONT4);
		LoadDivGraph(".\\media\\image\\font5.png", 25, 5, 5, 180, 60, image_+RENDER_IMAGE::FONT5);
		LoadDivGraph(".\\media\\image\\number.png", 20, 10, 2, 60, 60, image_+RENDER_IMAGE::NUMBER1);
		LoadDivGraph(".\\media\\image\\number2.png", 6, 6, 1, 30, 60, image_+RENDER_IMAGE::NUMBER2);
		LoadDivGraph(".\\media\\image\\bomb.jpg", 16, 8, 2, 128, 128, image_+RENDER_IMAGE::BOMB);
		LoadDivGraph(".\\media\\image\\Z.jpg", 1, 1, 1, 47, 51, image_+RENDER_IMAGE::Z);
		LoadDivGraph(".\\media\\image\\title.jpg", 1, 1, 1, 640, 480, image_+RENDER_IMAGE::TITLE));
		LoadDivGraph(".\\media\\image\\attend.png", 1, 1, 1, 640, 480, image_+RENDER_IMAGE::ATTEND));


		color_[0] = GetColor(255, 255, 255);// BG
		color_[1] = GetColor(0, 255, 0); // GROUND
	}

	void DrawBox(int x1 , int y1 , int x2 , int y2, RENDER_COLOR color)
	{
		DrawBox(x1, y1, x2, y2, color_[color], TRUE);
	}

	void Draw(int x, int y, RENDER_IMAGE image, int image_sub = 0, bool turn = false)
	{
		int GrHandle = image_[image + image_sub];
		
		if(turn){
			DrawTurnGraph(x, y, GrHandle, TRUE);
		}else{
			DrawGraph(x, y, GrHandle, TRUE);
		}
	}
};



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
int ChangeSpeed(bool change, int speed, int score)
{
	const int table[] = {
		5, 4, 3, 2, 4, 2, 3, 2, 5, 3, // 0-49
		4,2,5,2,1, // 50-
	};
	
	if(change){
		int rank = score / 5;
		if(sizeof(table)/sizeof(table[0]) <= rank) return 1;
		
		return table[rank];
	}
	
	return speed;
}

class Game
{
	int anime = 0, change = TRUE, speed = 5, score = 0, scene = 1, check = 0;
private:
public:
	
};

int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	srand((unsigned int)time(NULL));
	
	PlayerList *players = new PlayerList(3);
	Scene scene;
	Game game;
	Renderer renderer;

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	renderer.Initialie();

	players->setJumpKey(0, KEY_INPUT_SPACE);
	players->setJumpKey(1, KEY_INPUT_1);
	players->setJumpKey(2, KEY_INPUT_RETURN);
	
	
	while (ProcessMessage() == 0)
	{
		// 入力
		char keyBuf[256];
		GetHitKeyStateAll(keyBuf);
		
		// 更新
		scene.Update(players, renderer, keyBuf);

		// 画面切り替え
		ScreenFlip();
		
		// 修了リクエスト
		if (keyBuf[KEY_INPUT_ESCAPE]) break;
	}
	DxLib_End();
	
	delete players;
	return 0;
}
