#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

#define SAFE_DELETE(p) if(p) delete (p); (p)=nullptr;

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

//時間計算
int CheackTime(int start)
{
	int interval = GetNowCount() - start;
	return interval / 1000;
}

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
		image_[RENDER_IMAGE::PLAYER3 + 0] = LoadGraph(".\\media\\image\\3stand.png");
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

class Nawa{
private:
	bool change_;
	int anime_;
	int speed_;

	//スピード変化
	void changeSpeed(int score)
	{
		const int table[] = {
			5, 4, 3, 2, 4, 2, 3, 2, 5, 3, // 0-49
			4,2,5,2,1, // 50 -
		};

		int rank = score / 5;
		if(sizeof(table)/sizeof(table[0]) <= rank) speed_ = 1;// テーブルのスコアを超えていたら最高速

		speed_ = table[rank];
	}	
	
public:
	Nawa(){Initialize();}
	~Nawa(){}
	
	void Initialize()
	{
		change_ = true;
		anime_ = 0;
		speed_ = 5;
	}
	
	bool Update(int score)
	{
		bool is_clear = false;

		//縄の基本動作
		if (change_)
		{
			anime_++;
			if (anime_> (speed_ * 16) - 2)// 意味が分からない…
			{
				change_ = FALSE;
				is_clear = true;
			}
		}else{
			anime_--;
			if (anime_ < (speed_ - 1))
			{
				change_ = TRUE;
				changeSpeed(score);
			}
		}

		return is_clear;
	}
	
	bool IsHitTime()
	{
		return (speed_ * 16) - 10 < anime_;// 意味が分からない…
	}
	
	void Draw(Renderer &render)
	{
		render.Draw(27, 235, RENDER_IMAGE::NAWA, anime_ / speed_);
	}

};

struct INIT_PLAYER
{
	int x;					// 横位置
	int image_idx;			// 表示用の番号
	unsigned char jump_key;	// 押すとジャンプするキー
};

class Player
{
private:
	int x_ = 320;
	float y_ = 320.0f;// 加速度運動をするので、浮動小数点で管理
	float velocity_y_ = 0.0f;
	bool attend_;
	int life_ = 1;
	int death_cnt_ = 0;
	bool onGround_ = false;
	unsigned char jump_key_ = KEY_INPUT_SPACE;
	
	int image_idx_ = 0;
public:
	Player(){}
	~Player(){}
	
	bool has_left() const
	{
		return (death_cnt_ <= 16);
	}
	
	// 一度だけの初期化
	void SetUp(const INIT_PLAYER &data)
	{
		x_ = data.x;
		image_idx_ = data.image_idx;
		jump_key_ = data.jump_key;
		
		Initialize();
	}
	
	// ゲームごとの初期化
	void Initialize()
	{
		life_ = 1;
		death_cnt_ = 0;
		attend_ = false;
		onGround_ = FALSE;
		velocity_y_ = 0.0f;
	}
	
	bool CheckAttend(const char *keyBuf)
	{
		if (keyBuf[jump_key_] == 1)
		{
			attend_ = true;
		}
		
		return attend_;
	}
	
	void CheackonGround()
	{
		const float max_height = 400.0f;
		
		if (pos.y > max_height)
		{
			pos.y = max_height;
			onGround_ = TRUE;
			velocity_y_ = 0.0f;
		}
	}
	
	bool ChackFailed()
	{
		if (!attend_) return false;

		if (onGround_)
		{
			life_ = 0;
			return true;
		}
		
		return false;
	}
	
	void CheckAlive()
	{
		if (!attend_) returm;
		
		if (attend_ && life_ == 0){
			death_cnt_++;
		}
	}
	
	void Move()
	{
		if (!attend_) returm;

		if (!onGround_) velocity_y_ += 0.8f;// 重力
		
		y_ += velocity_y_;
	}
	
	void Jump(const char *keyBuf)
	{
		if (!attend_) return;

		if (keyBuf[jump_key_] == 1 && onGround_)
		{
			velocity_y_ = -15.0f;
			onGround_ = false;
		}
	}

	void Draw(Renderer &render)
	{
		if (!attend_) return;

		if (life_ == 1)
		{
			bool is_turn = onGround_;
			render.Draw(x_, y_ - 100, image_idx_, is_turn ? 0 : 1, is_turn);
		}
		else
		{
			if (has_left()){
				render.Draw(x_, y_ - 100, RENDER_IMAGE::BOMB, death_cnt_);
			}
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
	
	void SetUp(const PLAYER_DATA *a_player_data)
	{
		for( int i = 0; i < num_; i++ )
		{
			player_[i].SetUp(a_player_data[i]);
		}
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

	unsigned int CheckAttend(const char *keyBuf)
	{
		unsigned int flag = 0;
		
		for( int i = 0; i < num_; i++ )
		{
			if(player_[i].CheckAttend(keyBuf)){
				flag += (1 << i);
			}
		}
		
		return flag;
	}

	bool ChackFailed()
	{
		bool failed = false;
		
		for( auto&& player : player_ )
		{
			failed |= player.ChackFailed();
		}
		
		return failed;
	}
	
	void Draw(Renderer &render)
	{
		for( int i = 0; i < num_; i++ )
		{
			player_[i].Draw(render, i);
		}
	}
};

class Game
{
private:
	int score_;
	Nawa nawa_;
	PlayerList *players_;
public:
	Game(){
		players_ = new PlayerList(3);
	}
	
	~Game(){
		SAFE_DELETE(players_);
	}
	
	// 一度だけの初期化
	void SetUp(const PLAYER_DATA *a_player_data)
	{
		players_.SetUp(a_player_data);
	}
	
	// ゲームごとの初期化
	void Initialize()
	{
		score_ = 0;
		players_.Initialize();
		nawa_.Initialize();
	}
	
	unsigned int CheckAttend(const char *keyBuf)
	{
		return players_->CheckAttend(keyBuf);
	}

	bool ChackFailed()
	{
		if(!nawa_.IsHitTime()) return false;

		return players_->ChackFailed();
	}
	
	void Update(const char *keyBuf) 
	{
		players_->CheackonGround();

		players_->Update();
		players_->Jump(keyBuf);
		players_->CheckAlive();
	}
	
	void Draw(Renderer &render)
	{
		//主人公の表示
		players_->Draw(render);
			
		nawa_.Draw(render);
	}
};


enum class SCENE_NAME
{
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
	
	virtual void Initialize(Game &game) = 0;
	virtual SCENE_NAME Update(Game &game, Renderer &render, const char *keyBuf) = 0;
};

class FSM_TITLE : public FSM
{
	int start_;
public:
	FSM_TITLE(){}	
	~FSM_TITLE(){}
	
	void Initialize(Game &game) override 
	{
		start_ = GetNowCount();
	}
	
	SCENE_NAME Update(Game &game, Renderer &render, const char *keyBuf) override
	{
		render.Draw(0,0,RENDER_IMAGE::TITLE);

		int time = CheackTime(start_);

		if (time & 1)// 一秒ごと点滅
		{
			render.Draw(300, 360, RENDER_IMAGE::FONT2, 14);
			render.Draw(450, 355, RENDER_IMAGE::Z);
		}

		if (keyBuf[KEY_INPUT_Z] == 1)
			return SCENE_NAME::ATTEND;
		}
	
		return SCENE_NAME::INVALID;
	}
};

class FSM_ATTEND : public FSM
{
public:
	FSM_ATTEND(){}	
	~FSM_ATTEND(){}
	
	void Initialize(Game &game) override 
	{
		game.Initialize();
	}
	
	SCENE_NAME Update(Game &game, Renderer &render, const char *keyBuf) override 
	{
		SCENE_NAME ret = SCENE_NAME::INVALID;
		
		bool attended = game.CheckAttend(keyBuf);
		
		bool is_all_attended = (attended & 0x7) == 0x7;
		if (keyBuf[KEY_INPUT_Z] == 1 && is_all_attended)
		{
			ret = SCENE_NAME::START;
		}
		
		render.Draw(0, 0, RENDER_IMAGE::ATTEND);

		if (attended & (1<<0))
		{
			render.Draw(260, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(287, 300,RENDER_IMAGE::PLAYER1, 0);
		}
		if (attended & (1<<1))
		{
			render.Draw(100, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(127, 300,RENDER_IMAGE::PLAYER2, 0);
		}
		if (attended & (1<<2))
		{
			render.Draw(420, 250,RENDER_IMAGE::FONT4, 4);
			render.Draw(447, 300,RENDER_IMAGE::PLAYER3, 0);
		}

		return ret;
	}
};

class FSM_IN_GAME : public FSM
{
protected:
	int start_;

	void Draw(conat Game &game, Renderer &render)
	{
		//地面と背景の表示
		render.DrawBox(0, 0, 640, 480, RENDER_COLOR::BG);
		render.DrawBox(0, 400, 640, 480, RENDER_COLOR::GROUND, TRUE);
		
		game.Draw(render);
	}

public:
	FSM_START(){}	
	~FSM_START(){}
	
	virtual void Initialize(Game &game) override {
		start_ = GetNowCount();
	};
	virtual SCENE_NAME Update(Game &game, Renderer &render, const char *keyBuf) override 
	{
		game.Update(keyBuf);

		Draw(game, render);
		
		return SCENE_NAME::INVALID;
	}
};

//スタートシーン
class FSM_START : public FSM_IN_GAME
{
public:
	FSM_START(){}	
	~FSM_START(){}
	
	void Initialize(Game &game) override
	{
		FSM_IN_GAME::Initialize();
	}
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		FSM_IN_GAME::Updada(players, render, keyBuf);
		
		int time = CheackTime(start_);
		if (3 <= time){
			return SCENE_NAME::PLAY;
		}else
		if(2 <= time){
			render.Draw(310, 230, RENDER_IMAGE::FONT4, 0);
			render.Draw(400, 230, RENDER_IMAGE::NUMBER2,5);
		}else {
			render.Draw(310, 230, RENDER_IMAGE::FONT3, 25);
			render.Draw(460, 230, RENDER_IMAGE::NUMBER1, 10);
		}
		
		return SCENE_NAME::INVALID;
	}
};

class FSM_PLAY : public FSM_IN_GAME
{
public:
	FSM_PLAY(){}	
	~FSM_PLAY(){}
	
	void Initialize(Game &game) override {}
	SCENE_NAME Update(PlayerList *players, Renderer &render, const char *keyBuf) override 
	{
		SCENE_NAME ret = SCENE_NAME::INVALID;
		
		bool failed = game.ChackFailed();
		if(failed) {ret = SCENE_NAME::GAMEOVER;}

		bool is_clear = nawa_.Update();
		if(is_clear) {score++;}

		FSM_IN_GAME::Updada(players, render, keyBuf);
		
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
	
	void Initialize(Game &game) override {
		start_ = GetNowCount();
	}
	
	SCENE_NAME Update(Gmae &game, Renderer &render, const char *keyBuf) override 
	{
		SCENE_NAME ret = SCENE_NAME::INVALID;
		
		FSM_IN_GAME::Updada();
		
		int time = CheackTime(start_);
		if (time > 20)
		{
			ret SCENE_NAME::TITLE;
		}
		
		render.Draw(310, 150, RENDER_IMAGE::FONT1, 3);
		render.Draw(200, 230, RENDER_IMAGE::FONT3, 17);
		render.Draw(450, 230, RENDER_IMAGE::NUMBER1, score / 10);
		render.Draw(510, 230, RENDER_IMAGE::NUMBER1, score % 10);

		return ret;
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
	}
	~Scene(){
		for( auto&& fsm : fsm_ )
		{
			delete fsm;
		}
	}
	
	void Initialize()
	{
		current_ = fsm_[0];
		current_->Initialize();
	}
	
	void Update(PlayerList *players, Renderer &render, const char *keyBuf)
	{
		SCENE_NAME next = current_->Update(players, render, keyBuf);
		
		// 返り値に SCENE_NAME::INVALID 以外が来たら切り替え
		if(next != SCENE_NAME::INVALID && next < SCENE_NAME::NUM){
			current_ = fsm_[next];
			current_->Initialize();
		}
	}
};


int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	Scene scene;
	Game game;
	Renderer renderer;

	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	const PLAYER_DATA player_data[] = {
		{RENDER_IMAGE::PLAYER1, 320 -33, KEY_INPUT_SPACE},
		{RENDER_IMAGE::PLAYER2, 320-131, KEY_INPUT_1},
		{RENDER_IMAGE::PLAYER3, 320 +65, KEY_INPUT_RETURN},
	};
	
	renderer.Initialie();
	game.SetUp(player_data);
	scene.Initialize();

	while (ProcessMessage() == 0)
	{
		char keyBuf[256];
		GetHitKeyStateAll(keyBuf);
		
		scene.Update(game, renderer, keyBuf);

		ScreenFlip();
		
		if (keyBuf[KEY_INPUT_ESCAPE]) break;
	}

	DxLib_End();
	delete players;
	
	return 0;
}
