#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <stdlib.h>
#include <time.h>

struct SIZE2D
{
	int width;
	int height;
};
struct POINT2DF
{
	float x;
	float y;
};
struct VECTOR2DF
{
	float x;
	float y;
};
struct CHARADATA
{
	int image;
	int r;
	int life;
	struct POINT2DF pos;
	BOOL onGround;
};

//タイトル画面～説明画面まで
void title()
{
	//経過秒数計測用の変数
	int start, end;
	double second;

	//ボタン入力が、１回しか認識されないようにする変数
	int change = 0;

	//フェードアウト・フェードイン演出用変数
	int right;

	//画像、音楽、効果音を入れる為の変数
	int image[3], font[68], se[2];

	//キー入力受付用変数
	char keyBuf[256];

	//画像、音楽、効果音を入れる
	image[0] = LoadGraph(".\\media\\image\\title.png");
	image[1] = LoadGraph(".\\media\\image\\title2.png");
	image[2] = LoadGraph(".\\media\\image\\Z.jpg");
	LoadDivGraph(".\\media\\image\\font2.bmp", 68, 4, 17, 112.75, 28.352941, font);
	se[0] = LoadSoundMem(".\\media\\se\\move.mp3");
	se[1] = LoadSoundMem(".\\media\\se\\start.mp3");

	//経過時間計測の最初の時間を変数に収納
	start = GetNowCount();

	//タイトル画面
	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		//タイトル画面表示
		DrawGraph(0, 0, image[0], FALSE);

		//経過時間計測の現在の時間を変数に収納
		end = GetNowCount();

		//経過時間を編集に収納
		second = (end - start) / 1000.0;

		//「PUSH START」の点滅表現
		if ((int)second % 2 == 0)
		{
			DrawRotaGraph(260, 360, 1.0, 0, font[14], TRUE);
			DrawRotaGraph(360, 360, 0.5, 0, image[2], TRUE);
		}
		ScreenFlip();

		//キー入力の確認
		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_Z] != 1)
			change = 1;
		if (keyBuf[KEY_INPUT_Z] == 1 && change == 1)
		{
			change = 0;
			PlaySoundMem(se[0], DX_PLAYTYPE_BACK);
			break;
		}
	}

	//フェードイン
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(255 - right, 255 - right, 255 - right);
		DrawGraph(0, 0, image[0], FALSE);
		ScreenFlip();
	}
	ClearDrawScreen();

	//フェードアウト
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(right, right, right);
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);
		ScreenFlip();
	}

	//説明画面
	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		//説明画面の表示
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);

		////経過時間計測の現在の時間を変数に収納
		end = GetNowCount();

		//経過時間を編集に収納
		second = (end - start) / 1000.0;

		ScreenFlip();

		//キー入力確認
		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_Z] != 1)
			change = 1;
		if (keyBuf[KEY_INPUT_Z] == 1 && change == 1)
		{
			change = 0;
			PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
			break;
		}
	}
	ClearDrawScreen();

	//フェードアウト
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(255 - right, 255 - right, 255 - right);
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);
		ScreenFlip();
	}
	ClearDrawScreen();
}

//キー入力確認
void ImputKey(int death, struct CHARADATA *player,  int CatImg[4], struct VECTOR2DF moveVec, float dt, int *JumpStop, struct VECTOR2DF *inerVec, struct VECTOR2DF jumpVec, struct VECTOR2DF scroVec, int *JumpPoint, int *UPmove, int se[], int *CatAniL, int *CatAniR, int *CatDir)
{
	//キー入力を受け付ける為の変数
	char keyBuf[256];

	//Zキーの処理についての変数
	int keyZNow;  //Zの入力を確認する為の変数

	//キー入力確認処理
	GetHitKeyStateAll(keyBuf);
	//左を押しているとき
	if (keyBuf[KEY_INPUT_LEFT] == 1 && !death)
	{
		//画像の差し替え
		if (player->onGround)
			player->image = CatImg[*CatAniL];

		//足を動かさせる動作
		(*CatAniL)++;
		if (*CatAniL > 1)
			*CatAniL = 0;

		//左を向かせる設定
		*CatDir = 0;

		//キャラクターの移動
		if (player->onGround)
			player->pos.x -= moveVec.x * dt;
	}
	//右を押しているとき
	if (keyBuf[KEY_INPUT_RIGHT] == 1 && !death)
	{
		//画像の差し替え
		if (player->onGround)
			player->image = CatImg[*CatAniR];

		//足を動かさせる動作
		(*CatAniR)++;
		if (*CatAniR > 3)
			*CatAniR = 2;

		//右を向かせる設定
		*CatDir = 1;

		//キャラクターの移動
		if (player->onGround)
			player->pos.x += (moveVec.x + scroVec.x) * dt;
	}
	//上を押しているとき
	if (keyBuf[KEY_INPUT_UP] == 1 && !death)
	{
		//左上に行くときの処理
		if (*CatDir == 0 && keyBuf[KEY_INPUT_LEFT] != 1 && player->onGround)
		{
			//画像の差し替え
			player->image = CatImg[*CatAniL];

			//足を動かさせる動作
			(*CatAniL)++;
			if (*CatAniL > 1)
				*CatAniL = 0;
		}
		//右上に行くときの処理
		if (*CatDir == 1 && keyBuf[KEY_INPUT_RIGHT] != 1 && player->onGround)
		{
			//画像の差し替え
			player->image = CatImg[*CatAniR];

			//足を動かさせる動作
			(*CatAniR)++;
			if (*CatAniR > 3)
				*CatAniR = 2;
		}

		//キャラクターの移動
		if (player->onGround)
			player->pos.y -= moveVec.y * dt;
	}
	//下を押しているとき
	if (keyBuf[KEY_INPUT_DOWN] == 1 && !death)
	{
		//左下に行くときの処理
		if (*CatDir == 0 && keyBuf[KEY_INPUT_LEFT] != 1 && player->onGround)
		{
			//画像の差し替え
			player->image = CatImg[*CatAniL];

			//足を動かさせる動作
			(*CatAniL)++;
			if (*CatAniL > 1)
				*CatAniL = 0;
		}
		//右下に行くときの処理
		if (*CatDir == 1 && keyBuf[KEY_INPUT_RIGHT] != 1 && player->onGround)
		{
			//画像の差し替え
			player->image = CatImg[*CatAniR];

			//足を動かさせる動作
			(*CatAniR)++;
			if (*CatAniR > 3)
				*CatAniR = 2;
		}

		//キャラクターの移動
		if (player->onGround)
			player->pos.y += moveVec.y * dt;
	}

	//Ｚを押しているとき
	if (keyBuf[KEY_INPUT_Z] == 1 && !death)
		keyZNow = 1;
	else
		keyZNow = 0;

	//Ｚ入力後の処理
	if (keyZNow == 1 && *JumpStop == 0)
	{
		player->onGround = FALSE;    //地面にはいない判定を適応
		*JumpStop = TRUE;            //ジャンプの連続入力防止用
		*inerVec = jumpVec;          //ジャンプ時のベクトルを適応
		*JumpPoint = player->pos.y;  //着地地点の計測用
		PlaySoundMem(se[0], DX_PLAYTYPE_BACK);  //効果音を鳴らす
		player->image = CatImg[1 + (*CatDir) * 2]; //向きに応じてキャラクターの画像を表示

		 //ジャンプ時の方向確認
		if (keyBuf[KEY_INPUT_LEFT] == 1)  //左
			inerVec->x -= moveVec.x - scroVec.x;
		if (keyBuf[KEY_INPUT_RIGHT] == 1) //右
			inerVec->x += moveVec.x + scroVec.x;
		if (keyBuf[KEY_INPUT_UP] == 1)    //上
			*UPmove = TRUE;
		if (keyBuf[KEY_INPUT_DOWN] == 1)  //下
			*JumpPoint += 100;
	}
}

//当たり判定の確認
void GrandCheak(int *death, struct CHARADATA *player, int score, int trapX[18])
{
	//死亡を判定する為の計算式３つ
	int mass1, mass2, mass3;

	//自機が画面外に行かない為の処理(横)
	if (player->pos.x > 500)
		player->pos.x = 500;
	if (player->pos.x < 50)
		player->pos.x = 50;

	//自機が画面外に行かない為の処理(縦)
	if (player->onGround)
	{
		if (player->pos.y < 100)
			player->pos.y = 100;
		if (player->pos.y > 400)
			player->pos.y = 400;
	}

	//以下海(死亡トラップ地点)の当たり判定
	if (player->onGround)
	{
		if (score > 500)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[0] && player->pos.x <= trapX[0] + 150)
					*death = TRUE;
			}
		}

		if (score > 1000)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[1] && player->pos.x <= trapX[1] + 80)
					*death = TRUE;
			}
		}
		if (score > 1300)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[2] && player->pos.x <= trapX[2] + 80)
					*death = TRUE;
			}
		}
		if (score > 1600)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[3] && player->pos.x <= trapX[3] + 80)
					*death = TRUE;
			}
		}

		if (score > 1800)
		{
			if (player->pos.y >= 90 && player->pos.y <= 265)
			{
				if (player->pos.x >= trapX[4] && player->pos.x <= trapX[4] + 150)
					*death = TRUE;
			}
		}
		if (score > 2100)
		{
			if (player->pos.y >= 265 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[5] && player->pos.x <= trapX[5] + 150)
					*death = TRUE;
			}
		}
		if (score > 2400)
		{
			if (player->pos.y >= 90 && player->pos.y <= 265)
			{
				if (player->pos.x >= trapX[6] && player->pos.x <= trapX[6] + 150)
					*death = TRUE;
			}
		}
		if (score > 2700)
		{
			if (player->pos.y >= 265 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[7] && player->pos.x <= trapX[7] + 150)
					*death = TRUE;
			}
		}

		if (score > 3000)
		{
			if (player->pos.y >= 90 && player->pos.y <= 200)
			{
				if (player->pos.x >= trapX[8] && player->pos.x <= trapX[8] + 150)
					*death = TRUE;
			}
		}
		if (score > 3200)
		{
			if (player->pos.y >= 200 && player->pos.y <= 330)
			{
				if (player->pos.x >= trapX[9] && player->pos.x <= trapX[9] + 150)
					*death = TRUE;
			}
		}
		if (score > 3000)
		{
			if (player->pos.y >= 330 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[10] && player->pos.x <= trapX[10] + 150)
					*death = TRUE;
			}
		}

		if (score > 3600)
		{
			if (player->pos.y >= 90 && player->pos.y <= 200)
			{
				if (player->pos.x >= trapX[11] && player->pos.x <= trapX[11] + 150)
					*death = TRUE;
			}
		}
		if (score > 3700)
		{
			if (player->pos.y >= 200 && player->pos.y <= 330)
			{
				if (player->pos.x >= trapX[12] && player->pos.x <= trapX[12] + 150)
					*death = TRUE;
			}
		}
		if (score > 3800)
		{
			if (player->pos.y >= 330 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[13] && player->pos.x <= trapX[13] + 150)
					*death = TRUE;
			}
		}

		if (score > 4100)
		{
			if (player->pos.y >= 90 && player->pos.y <= 200)
			{
				if (player->pos.x >= trapX[14] && player->pos.x <= trapX[14] + 150)
					*death = TRUE;
			}
		}
		if (score > 4000)
		{
			if (player->pos.y >= 200 && player->pos.y <= 330)
			{
				if (player->pos.x >= trapX[15] && player->pos.x <= trapX[15] + 150)
					*death = TRUE;
			}
		}
		if (score > 3900)
		{
			if (player->pos.y >= 330 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[16] && player->pos.x <= trapX[16] + 150)
					*death = TRUE;
			}
		}

		if (score > 4500)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[17] && player->pos.x <= trapX[17] + 200)
					*death = TRUE;
			}
		}

		if (score > 5200)
		{
			if (player->pos.y >= 90 && player->pos.y <= 200)
			{
				if (player->pos.x >= trapX[18] && player->pos.x <= trapX[18] + 150)
					*death = TRUE;
			}
		}
		if (score > 5000)
		{
			if (player->pos.y >= 200 && player->pos.y <= 330)
			{
				if (player->pos.x >= trapX[19] && player->pos.x <= trapX[19] + 150)
					*death = TRUE;
			}
		}
		if (score > 5200)
		{
			if (player->pos.y >= 330 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[20] && player->pos.x <= trapX[20] + 150)
					*death = TRUE;
			}
		}

		if (score > 5500)
		{
			if (player->pos.y >= 90 && player->pos.y <= 240)
			{
				if (player->pos.x >= trapX[21] && player->pos.x <= trapX[21] + 600)
					*death = TRUE;
			}
		}
		if (score > 5500)
		{
			if (player->pos.y >= 290 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[22] && player->pos.x <= trapX[22] + 600)
					*death = TRUE;
			}
		}
		if (score > 6100)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[23] && player->pos.x <= trapX[23] + 200)
					*death = TRUE;
			}
		}

		if (score > 6500)
		{
			if (player->pos.y >= 90 && player->pos.y <= 175)
			{
				if (player->pos.x >= trapX[24] && player->pos.x <= trapX[24] + 200)
					*death = TRUE;
			}
		}
		if (score > 6700)
		{
			if (player->pos.y >= 175 && player->pos.y <= 260)
			{
				if (player->pos.x >= trapX[25] && player->pos.x <= trapX[25] + 200)
					*death = TRUE;
			}
		}
		if (score > 6500)
		{
			if (player->pos.y >= 260 && player->pos.y <= 345)
			{
				if (player->pos.x >= trapX[26] && player->pos.x <= trapX[26] + 200)
					*death = TRUE;
			}
		}
		if (score > 6700)
		{
			if (player->pos.y >= 345 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[27] && player->pos.x <= trapX[27] + 200)
					*death = TRUE;
			}
		}

		if (score > 6900)
		{
			if (player->pos.y >= 90 && player->pos.y <= 175)
			{
				if (player->pos.x >= trapX[28] && player->pos.x <= trapX[28] + 200)
					*death = TRUE;
			}
		}
		if (score > 7100)
		{
			if (player->pos.y >= 175 && player->pos.y <= 260)
			{
				if (player->pos.x >= trapX[29] && player->pos.x <= trapX[29] + 200)
					*death = TRUE;
			}
		}
		if (score > 6900)
		{
			if (player->pos.y >= 260 && player->pos.y <= 345)
			{
				if (player->pos.x >= trapX[30] && player->pos.x <= trapX[30] + 200)
					*death = TRUE;
			}
		}
		if (score > 7100)
		{
			if (player->pos.y >= 345 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[31] && player->pos.x <= trapX[31] + 200)
					*death = TRUE;
			}
		}

		if (score > 7600)
		{
			if (player->pos.y >= 160 && player->pos.y <= 390)
			{
				if (player->pos.x >= trapX[32] && player->pos.x <= trapX[32] + 600)
					*death = TRUE;
			}
		}

		if (score > 8500)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[33] && player->pos.x <= trapX[33] + 200)
					*death = TRUE;
			}
		}
		if (score > 8800)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[34] && player->pos.x <= trapX[34] + 200)
					*death = TRUE;
			}
		}
		if (score > 9100)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[35] && player->pos.x <= trapX[35] + 200)
					*death = TRUE;
			}
		}
		if (score > 9400)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[36] && player->pos.x <= trapX[36] + 200)
					*death = TRUE;
			}
		}
		if (score > 9700)
		{
			if (player->pos.y >= 90 && player->pos.y <= 440)
			{
				if (player->pos.x >= trapX[37] && player->pos.x <= trapX[37] + 300)
					*death = TRUE;
			}
		}
	}
}

//描画処理
void GraphImput(int death, struct CHARADATA player, int score, int *trapX, struct VECTOR2DF scroVec, float dt, int font2[42], int font3[35], int *anime, int font1[15], int se[2], int bomb[16], int loop)
{
	//キー入力を受け付ける為の変数
	char keyBuf[256];

	//海
	DrawBox(0, 0, 640, 480, GetColor(157, 204, 224), TRUE);
	//空
	DrawBox(0, 0, 640, 90, GetColor(225, 225, 255), TRUE);
	//地上
	DrawBox(0, 120, 640, 440, GetColor(144, 238, 144), TRUE);

	//以下トラップ(死亡ポイントである海の穴)の表示
	if (score > 500)
	{
		trapX[0] -= scroVec.x * dt;
		DrawBox(trapX[0], 90, trapX[0] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 1000)
	{
		trapX[1] -= scroVec.x * dt;
		DrawBox(trapX[1], 90, trapX[1] + 80, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 1300)
	{
		trapX[2] -= scroVec.x * dt;
		DrawBox(trapX[2], 90, trapX[2] + 80, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 1600) 
	{
		trapX[3] -= scroVec.x * dt;
		DrawBox(trapX[3], 90, trapX[3] + 80, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 1800)
	{
		trapX[4] -= scroVec.x * dt;
		DrawBox(trapX[4], 90, trapX[4] + 150, 265, GetColor(157, 204, 224), TRUE);
	}
	if (score > 2100)
	{
		trapX[5] -= scroVec.x * dt;
		DrawBox(trapX[5], 265, trapX[5] + 150, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 2400)
	{
		trapX[6] -= scroVec.x * dt;
		DrawBox(trapX[6], 90, trapX[6] + 150, 265, GetColor(157, 204, 224), TRUE);
	}
	if (score > 2700)
	{
		trapX[7] -= scroVec.x * dt;
		DrawBox(trapX[7], 265, trapX[7] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 3000)
	{
		trapX[8] -= scroVec.x * dt;
		DrawBox(trapX[8], 90, trapX[8] + 150, 200, GetColor(157, 204, 224), TRUE);
	}
	if (score > 3200)
	{
		trapX[9] -= scroVec.x * dt;
		DrawBox(trapX[9], 200, trapX[9] + 150, 330, GetColor(157, 204, 224), TRUE);
	}
	if (score > 3000)
	{
		trapX[10] -= scroVec.x * dt;
		DrawBox(trapX[10], 330, trapX[10] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 3600)
	{
		trapX[11] -= scroVec.x * dt;
		DrawBox(trapX[11], 90, trapX[11] + 150, 200, GetColor(157, 204, 224), TRUE);
	}
	if (score > 3700)
	{
		trapX[12] -= scroVec.x * dt;
		DrawBox(trapX[12], 200, trapX[12] + 150, 330, GetColor(157, 204, 224), TRUE);
	}
	if (score > 3800)
	{
		trapX[13] -= scroVec.x * dt;
		DrawBox(trapX[13], 330, trapX[13] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 4100)
	{
		trapX[14] -= scroVec.x * dt;
		DrawBox(trapX[14], 90, trapX[14] + 150, 200, GetColor(157, 204, 224), TRUE);
	}
	if (score > 4000)
	{
		trapX[15] -= scroVec.x * dt;
		DrawBox(trapX[15], 200, trapX[15] + 150, 330, GetColor(157, 204, 224), TRUE);
	}
	if (score > 3900)
	{
		trapX[16] -= scroVec.x * dt;
		DrawBox(trapX[16], 330, trapX[16] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 4500)
	{
		trapX[17] -= scroVec.x * dt;
		DrawBox(trapX[17], 90, trapX[17] + 200, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 5200)
	{
		trapX[18] -= scroVec.x * dt;
		DrawBox(trapX[18], 90, trapX[18] + 150, 200, GetColor(157, 204, 224), TRUE);
	}
	if (score > 5000)
	{
		trapX[19] -= scroVec.x * dt;
		DrawBox(trapX[19], 200, trapX[19] + 150, 330, GetColor(157, 204, 224), TRUE);
	}
	if (score > 5200)
	{
		trapX[20] -= scroVec.x * dt;
		DrawBox(trapX[20], 330, trapX[20] + 150, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 5500)
	{
		trapX[21] -= scroVec.x * dt;
		DrawBox(trapX[21], 90, trapX[21] + 600, 240, GetColor(157, 204, 224), TRUE);
	}
	if (score > 5500)
	{
		trapX[22] -= scroVec.x * dt;
		DrawBox(trapX[22], 290, trapX[22] + 600, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 6100)
	{
		trapX[23] -= scroVec.x * dt;
		DrawBox(trapX[23], 90, trapX[23] + 200, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 6500)
	{
		trapX[24] -= scroVec.x * dt;
		DrawBox(trapX[24], 90, trapX[24] + 200, 175, GetColor(157, 204, 224), TRUE);
	}
	if (score > 6700)
	{
		trapX[25] -= scroVec.x * dt;
		DrawBox(trapX[25], 175, trapX[25] + 200, 260, GetColor(157, 204, 224), TRUE);
	}
	if (score > 6500)
	{
		trapX[26] -= scroVec.x * dt;
		DrawBox(trapX[26], 260, trapX[26] + 200, 345, GetColor(157, 204, 224), TRUE);
	}
	if (score > 6700)
	{
		trapX[27] -= scroVec.x * dt;
		DrawBox(trapX[27], 345, trapX[27] + 200, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 6900)
	{
		trapX[28] -= scroVec.x * dt;
		DrawBox(trapX[28], 90, trapX[28] + 200, 175, GetColor(157, 204, 224), TRUE);
	}
	if (score > 7100)
	{
		trapX[29] -= scroVec.x * dt;
		DrawBox(trapX[29], 175, trapX[29] + 200, 260, GetColor(157, 204, 224), TRUE);
	}
	if (score > 6900)
	{
		trapX[30] -= scroVec.x * dt;
		DrawBox(trapX[30], 260, trapX[30] + 200, 345, GetColor(157, 204, 224), TRUE);
	}
	if (score > 7100)
	{
		trapX[31] -= scroVec.x * dt;
		DrawBox(trapX[31], 345, trapX[31] + 200, 440, GetColor(157, 204, 224), TRUE);
	}

	if (score > 7600)
	{
		trapX[32] -= scroVec.x * dt;
		DrawBox(trapX[32], 160, trapX[32] + 600, 390, GetColor(157, 204, 224), TRUE);
	}

	if (score > 8500)
	{
		trapX[33] -= scroVec.x * dt;
		DrawBox(trapX[33], 90, trapX[33] + 200, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 8800)
	{
		trapX[34] -= scroVec.x * dt;
		DrawBox(trapX[34], 90, trapX[34] + 200, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 9100)
	{
		trapX[35] -= scroVec.x * dt;
		DrawBox(trapX[35], 90, trapX[35] + 200, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 9400)
	{
		trapX[36] -= scroVec.x * dt;
		DrawBox(trapX[36], 90, trapX[36] + 200, 440, GetColor(157, 204, 224), TRUE);
	}
	if (score > 9700)
	{
		trapX[37] -= scroVec.x * dt;
		DrawBox(trapX[37], 90, trapX[37] + 300, 440, GetColor(157, 204, 224), TRUE);
	}


	//猫の表示
	if (!death)
		DrawRotaGraph((int)player.pos.x, (int)player.pos.y, 0.5, 0, player.image, TRUE);
	if (death)//死んだ時
	{
		if (*anime == 1)
			PlaySoundMem(se[1], DX_PLAYTYPE_BACK);
		if (*anime <= 17)
			DrawRotaGraph((int)player.pos.x, (int)player.pos.y, 1, 0, bomb[*anime], TRUE);
		else
			DrawRotaGraph(310, 230, 1.0, 0, font1[3], TRUE);
	}

	if (loop == 0)
	{
		//『ready』の表示
		if (score >= 0 && score < 300)
			DrawRotaGraph(310, 230, 1.0, 0, font2[25], TRUE);
		//『go』の表示
		if (score >= 300 && score < 600)
			DrawRotaGraph(310, 230, 1.0, 0, font3[0], TRUE);
	}
}

void game()
{
	//画面の明度設定
	SetDrawBright(255, 255, 255);

	//for文用
	int i;

	//各移動数値収納用変数(調整用)
	const float dt = (float)1 / 60;                //細かい調整するために、ベクトル計算にかける為の変数
	const struct VECTOR2DF moveVec = { 180, 150 }; //プレイヤーの移動力調整用
	const struct VECTOR2DF jumpVec = { 0, -500 };  //ジャンプ力の調整用
	const struct VECTOR2DF gravVec = { 0, 20 };    //重力のかけ具合調整用
	const struct VECTOR2DF scroVec = { 240, 0 };   //強制スクロールの調整用
	struct VECTOR2DF inerVec = { 0, 0 };           //ジャンプ時のベクトル計算用

												   //収納用変数
	int bomb[16];                         //爆発アニメーション
	int font1[15], font2[42], font3[35];  //フォント
	int CatImg[4];                        //主人公(猫)
	int se[2];                            //SE

	//キー入力を受け付ける為の変数
	char keyBuf[256];

	//プレイヤー変数の宣言
	struct CHARADATA player;

	//画像・音楽・効果音を入れる
	LoadDivGraph(".\\media\\image\\character1.bmp", 4, 2, 2, 150, 150, CatImg); //プレイヤーの画像
	LoadDivGraph(".\\media\\image\\bomb.jpg", 16, 8, 2, 128, 128, bomb);        //死亡時の爆発
	LoadDivGraph(".\\media\\image\\font.jpg", 15, 1, 15, 600, 60, font1);       //フォント一式その１
	LoadDivGraph(".\\media\\image\\font3.bmp", 42, 3, 14, 300, 60, font2);      //フォント一式その２
	LoadDivGraph(".\\media\\image\\font4.png", 35, 5, 7, 120, 60, font3);       //フォント一式その３
	se[0] = LoadSoundMem(".\\media\\se\\jump.mp3");                             //ジャンプ音
	se[1] = LoadSoundMem(".\\media\\se\\bomb.mp3");                             //死亡音

	//プレイヤーの初期設定
	player.r = 20;            //当たり判定の大きさ
	player.life = 1;          //生きてるか死んでるかの判定(1=生きている状態、0=死んでいる状態)
	player.image = CatImg[1]; //現在行っているアニメーションを収納
	player.pos.x = 320;       //プレイヤーのX座標の位置
	player.pos.y = 350;       //プレイヤーのY座標の位置

	 //猫のアニメーションについての変数
	int CatAniL = 0;  //猫が左を向いている時、２パターンのアニメーションを動かすための変数
	int CatAniR = 2;  //猫が右を向いている時、２パターンのアニメーションを動かすための変数
	int CatDir = 0;  //猫が向いている方向の判定(0=左を向いた状態、1=右を向いた状態)

	//判定用変数
	player.onGround = TRUE;  //プレイヤーが地上にいるかいないかを判定(TRUE=地上にいる、FALSE=空中にいる)
	int JumpStop = FALSE;    //ジャンプが連続で発生いしないための変数(FALSE=ジャンプOK、TRUE＝ジャンプできませーん)
	int UPmove = FALSE;      //上方向へのジャンプをするかしないかを判定(TRUE=してます、FALSE=してません)
	int death = FALSE;       //ゲームが終了したかしてないかを判定(FALSE=継続、TRUE=終了)

	//下方向へのジャンプ時に、着地地点を決定し、プレイヤーがそこで止まる様にする為の変数
	int JumpPoint = 0;

	//スクロールした距離
	int score = 0;

	//１ステージ分の周回回数を数える
	int loop = 0;

	//穴のX座標
	int trapX[38];
	for (i = 0; i < 38; i++)
		trapX[i] = 640;

	//死亡時の爆発のアニメーションを動かす為の変数
	int anime = 0;

	while (ProcessMessage() == 0)
	{
		//キー入力確認
		ImputKey(death, &player, CatImg, moveVec, dt, &JumpStop, &inerVec, jumpVec, scroVec, &JumpPoint, &UPmove, se, &CatAniL, &CatAniR, &CatDir);

		//break用キー入力確認処理
		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_ESCAPE] == 1 && !death)
			break;

		//重力処理
		if (!player.onGround)
			inerVec.y += gravVec.y;

		//移動処理
		player.pos.x += inerVec.x*dt;
		player.pos.y += inerVec.y*dt;

		//着地地点計算
		if (!UPmove)
		{
			if (player.pos.y > JumpPoint)
				player.onGround = TRUE;
			if (player.pos.y > 400)
				player.onGround = TRUE;
		}
		if (UPmove)
		{
			if (inerVec.y > 0)
			{
				player.onGround = TRUE;
				UPmove = FALSE;
			}
		}

		//地面にいるときの初期化
		if (player.onGround)
		{
			JumpStop = FALSE;
			inerVec.x = 0;
			inerVec.y = 0;
		}

		//当たり判定の確認
		GrandCheak(&death, &player, score, trapX);

		//描画処理
		GraphImput(death, player, score , trapX, scroVec, dt, font2, font3, &anime, font1, se, bomb, loop);
		DrawFormatString(0, 0, GetColor(0, 0, 0), "<進んだ距離>%5dm", score + (11000 * loop));
		ScreenFlip();

		//プレイヤーにスクロール効果をかける
		player.pos.x -= scroVec.x * dt;

		//周回計測
		if (score > 11000)
		{
			score = 0;
			loop++;
			for (i = 0; i < 38; i++)
				trapX[i] = 640;
		}

		//死んだ時の処理
		if (death)
		{
			anime++;
			if (keyBuf[KEY_INPUT_Z] == 1)
				break;
		}
		else
			score += scroVec.x * dt;  //スコア(進んだ距離)の計算
	}
}


int WINAPI WinMain(HINSTANCE hI, HINSTANCE hP, LPSTR lpC, int nC)
{
	const SIZE2D screen = { 640, 480 };
	const int bitNum = 64;

	srand((unsigned int)time(NULL));
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)
		return -1;
	SetDrawScreen(DX_SCREEN_BACK);

	title();
	game();

	DxLib_End();
	return 0;
}