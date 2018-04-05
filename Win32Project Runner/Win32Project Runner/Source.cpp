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

//�^�C�g����ʁ`������ʂ܂�
void title()
{
	//�o�ߕb���v���p�̕ϐ�
	int start, end;
	double second;

	//�{�^�����͂��A�P�񂵂��F������Ȃ��悤�ɂ���ϐ�
	int change = 0;

	//�t�F�[�h�A�E�g�E�t�F�[�h�C�����o�p�ϐ�
	int right;

	//�摜�A���y�A���ʉ�������ׂ̕ϐ�
	int image[3], font[68], se[2];

	//�L�[���͎�t�p�ϐ�
	char keyBuf[256];

	//�摜�A���y�A���ʉ�������
	image[0] = LoadGraph(".\\media\\image\\title.png");
	image[1] = LoadGraph(".\\media\\image\\title2.png");
	image[2] = LoadGraph(".\\media\\image\\Z.jpg");
	LoadDivGraph(".\\media\\image\\font2.bmp", 68, 4, 17, 112.75, 28.352941, font);
	se[0] = LoadSoundMem(".\\media\\se\\move.mp3");
	se[1] = LoadSoundMem(".\\media\\se\\start.mp3");

	//�o�ߎ��Ԍv���̍ŏ��̎��Ԃ�ϐ��Ɏ��[
	start = GetNowCount();

	//�^�C�g�����
	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		//�^�C�g����ʕ\��
		DrawGraph(0, 0, image[0], FALSE);

		//�o�ߎ��Ԍv���̌��݂̎��Ԃ�ϐ��Ɏ��[
		end = GetNowCount();

		//�o�ߎ��Ԃ�ҏW�Ɏ��[
		second = (end - start) / 1000.0;

		//�uPUSH START�v�̓_�ŕ\��
		if ((int)second % 2 == 0)
		{
			DrawRotaGraph(260, 360, 1.0, 0, font[14], TRUE);
			DrawRotaGraph(360, 360, 0.5, 0, image[2], TRUE);
		}
		ScreenFlip();

		//�L�[���͂̊m�F
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

	//�t�F�[�h�C��
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(255 - right, 255 - right, 255 - right);
		DrawGraph(0, 0, image[0], FALSE);
		ScreenFlip();
	}
	ClearDrawScreen();

	//�t�F�[�h�A�E�g
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(right, right, right);
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);
		ScreenFlip();
	}

	//�������
	while (ProcessMessage() != -1)
	{
		ClearDrawScreen();

		//������ʂ̕\��
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);

		////�o�ߎ��Ԍv���̌��݂̎��Ԃ�ϐ��Ɏ��[
		end = GetNowCount();

		//�o�ߎ��Ԃ�ҏW�Ɏ��[
		second = (end - start) / 1000.0;

		ScreenFlip();

		//�L�[���͊m�F
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

	//�t�F�[�h�A�E�g
	for (right = 0; right < 255; right += 2)
	{
		SetDrawBright(255 - right, 255 - right, 255 - right);
		DrawRotaGraph(320, 240, 1, 0, image[1], TRUE);
		ScreenFlip();
	}
	ClearDrawScreen();
}

//�L�[���͊m�F
void ImputKey(int death, struct CHARADATA *player,  int CatImg[4], struct VECTOR2DF moveVec, float dt, int *JumpStop, struct VECTOR2DF *inerVec, struct VECTOR2DF jumpVec, struct VECTOR2DF scroVec, int *JumpPoint, int *UPmove, int se[], int *CatAniL, int *CatAniR, int *CatDir)
{
	//�L�[���͂��󂯕t����ׂ̕ϐ�
	char keyBuf[256];

	//Z�L�[�̏����ɂ��Ă̕ϐ�
	int keyZNow;  //Z�̓��͂��m�F����ׂ̕ϐ�

	//�L�[���͊m�F����
	GetHitKeyStateAll(keyBuf);
	//���������Ă���Ƃ�
	if (keyBuf[KEY_INPUT_LEFT] == 1 && !death)
	{
		//�摜�̍����ւ�
		if (player->onGround)
			player->image = CatImg[*CatAniL];

		//���𓮂������铮��
		(*CatAniL)++;
		if (*CatAniL > 1)
			*CatAniL = 0;

		//������������ݒ�
		*CatDir = 0;

		//�L�����N�^�[�̈ړ�
		if (player->onGround)
			player->pos.x -= moveVec.x * dt;
	}
	//�E�������Ă���Ƃ�
	if (keyBuf[KEY_INPUT_RIGHT] == 1 && !death)
	{
		//�摜�̍����ւ�
		if (player->onGround)
			player->image = CatImg[*CatAniR];

		//���𓮂������铮��
		(*CatAniR)++;
		if (*CatAniR > 3)
			*CatAniR = 2;

		//�E����������ݒ�
		*CatDir = 1;

		//�L�����N�^�[�̈ړ�
		if (player->onGround)
			player->pos.x += (moveVec.x + scroVec.x) * dt;
	}
	//��������Ă���Ƃ�
	if (keyBuf[KEY_INPUT_UP] == 1 && !death)
	{
		//����ɍs���Ƃ��̏���
		if (*CatDir == 0 && keyBuf[KEY_INPUT_LEFT] != 1 && player->onGround)
		{
			//�摜�̍����ւ�
			player->image = CatImg[*CatAniL];

			//���𓮂������铮��
			(*CatAniL)++;
			if (*CatAniL > 1)
				*CatAniL = 0;
		}
		//�E��ɍs���Ƃ��̏���
		if (*CatDir == 1 && keyBuf[KEY_INPUT_RIGHT] != 1 && player->onGround)
		{
			//�摜�̍����ւ�
			player->image = CatImg[*CatAniR];

			//���𓮂������铮��
			(*CatAniR)++;
			if (*CatAniR > 3)
				*CatAniR = 2;
		}

		//�L�����N�^�[�̈ړ�
		if (player->onGround)
			player->pos.y -= moveVec.y * dt;
	}
	//���������Ă���Ƃ�
	if (keyBuf[KEY_INPUT_DOWN] == 1 && !death)
	{
		//�����ɍs���Ƃ��̏���
		if (*CatDir == 0 && keyBuf[KEY_INPUT_LEFT] != 1 && player->onGround)
		{
			//�摜�̍����ւ�
			player->image = CatImg[*CatAniL];

			//���𓮂������铮��
			(*CatAniL)++;
			if (*CatAniL > 1)
				*CatAniL = 0;
		}
		//�E���ɍs���Ƃ��̏���
		if (*CatDir == 1 && keyBuf[KEY_INPUT_RIGHT] != 1 && player->onGround)
		{
			//�摜�̍����ւ�
			player->image = CatImg[*CatAniR];

			//���𓮂������铮��
			(*CatAniR)++;
			if (*CatAniR > 3)
				*CatAniR = 2;
		}

		//�L�����N�^�[�̈ړ�
		if (player->onGround)
			player->pos.y += moveVec.y * dt;
	}

	//�y�������Ă���Ƃ�
	if (keyBuf[KEY_INPUT_Z] == 1 && !death)
		keyZNow = 1;
	else
		keyZNow = 0;

	//�y���͌�̏���
	if (keyZNow == 1 && *JumpStop == 0)
	{
		player->onGround = FALSE;    //�n�ʂɂ͂��Ȃ������K��
		*JumpStop = TRUE;            //�W�����v�̘A�����͖h�~�p
		*inerVec = jumpVec;          //�W�����v���̃x�N�g����K��
		*JumpPoint = player->pos.y;  //���n�n�_�̌v���p
		PlaySoundMem(se[0], DX_PLAYTYPE_BACK);  //���ʉ���炷
		player->image = CatImg[1 + (*CatDir) * 2]; //�����ɉ����ăL�����N�^�[�̉摜��\��

		 //�W�����v���̕����m�F
		if (keyBuf[KEY_INPUT_LEFT] == 1)  //��
			inerVec->x -= moveVec.x - scroVec.x;
		if (keyBuf[KEY_INPUT_RIGHT] == 1) //�E
			inerVec->x += moveVec.x + scroVec.x;
		if (keyBuf[KEY_INPUT_UP] == 1)    //��
			*UPmove = TRUE;
		if (keyBuf[KEY_INPUT_DOWN] == 1)  //��
			*JumpPoint += 100;
	}
}

//�����蔻��̊m�F
void GrandCheak(int *death, struct CHARADATA *player, int score, int trapX[18])
{
	//���S�𔻒肷��ׂ̌v�Z���R��
	int mass1, mass2, mass3;

	//���@����ʊO�ɍs���Ȃ��ׂ̏���(��)
	if (player->pos.x > 500)
		player->pos.x = 500;
	if (player->pos.x < 50)
		player->pos.x = 50;

	//���@����ʊO�ɍs���Ȃ��ׂ̏���(�c)
	if (player->onGround)
	{
		if (player->pos.y < 100)
			player->pos.y = 100;
		if (player->pos.y > 400)
			player->pos.y = 400;
	}

	//�ȉ��C(���S�g���b�v�n�_)�̓����蔻��
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

//�`�揈��
void GraphImput(int death, struct CHARADATA player, int score, int *trapX, struct VECTOR2DF scroVec, float dt, int font2[42], int font3[35], int *anime, int font1[15], int se[2], int bomb[16], int loop)
{
	//�L�[���͂��󂯕t����ׂ̕ϐ�
	char keyBuf[256];

	//�C
	DrawBox(0, 0, 640, 480, GetColor(157, 204, 224), TRUE);
	//��
	DrawBox(0, 0, 640, 90, GetColor(225, 225, 255), TRUE);
	//�n��
	DrawBox(0, 120, 640, 440, GetColor(144, 238, 144), TRUE);

	//�ȉ��g���b�v(���S�|�C���g�ł���C�̌�)�̕\��
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


	//�L�̕\��
	if (!death)
		DrawRotaGraph((int)player.pos.x, (int)player.pos.y, 0.5, 0, player.image, TRUE);
	if (death)//���񂾎�
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
		//�wready�x�̕\��
		if (score >= 0 && score < 300)
			DrawRotaGraph(310, 230, 1.0, 0, font2[25], TRUE);
		//�wgo�x�̕\��
		if (score >= 300 && score < 600)
			DrawRotaGraph(310, 230, 1.0, 0, font3[0], TRUE);
	}
}

void game()
{
	//��ʂ̖��x�ݒ�
	SetDrawBright(255, 255, 255);

	//for���p
	int i;

	//�e�ړ����l���[�p�ϐ�(�����p)
	const float dt = (float)1 / 60;                //�ׂ����������邽�߂ɁA�x�N�g���v�Z�ɂ�����ׂ̕ϐ�
	const struct VECTOR2DF moveVec = { 180, 150 }; //�v���C���[�̈ړ��͒����p
	const struct VECTOR2DF jumpVec = { 0, -500 };  //�W�����v�͂̒����p
	const struct VECTOR2DF gravVec = { 0, 20 };    //�d�͂̂���������p
	const struct VECTOR2DF scroVec = { 240, 0 };   //�����X�N���[���̒����p
	struct VECTOR2DF inerVec = { 0, 0 };           //�W�����v���̃x�N�g���v�Z�p

												   //���[�p�ϐ�
	int bomb[16];                         //�����A�j���[�V����
	int font1[15], font2[42], font3[35];  //�t�H���g
	int CatImg[4];                        //��l��(�L)
	int se[2];                            //SE

	//�L�[���͂��󂯕t����ׂ̕ϐ�
	char keyBuf[256];

	//�v���C���[�ϐ��̐錾
	struct CHARADATA player;

	//�摜�E���y�E���ʉ�������
	LoadDivGraph(".\\media\\image\\character1.bmp", 4, 2, 2, 150, 150, CatImg); //�v���C���[�̉摜
	LoadDivGraph(".\\media\\image\\bomb.jpg", 16, 8, 2, 128, 128, bomb);        //���S���̔���
	LoadDivGraph(".\\media\\image\\font.jpg", 15, 1, 15, 600, 60, font1);       //�t�H���g�ꎮ���̂P
	LoadDivGraph(".\\media\\image\\font3.bmp", 42, 3, 14, 300, 60, font2);      //�t�H���g�ꎮ���̂Q
	LoadDivGraph(".\\media\\image\\font4.png", 35, 5, 7, 120, 60, font3);       //�t�H���g�ꎮ���̂R
	se[0] = LoadSoundMem(".\\media\\se\\jump.mp3");                             //�W�����v��
	se[1] = LoadSoundMem(".\\media\\se\\bomb.mp3");                             //���S��

	//�v���C���[�̏����ݒ�
	player.r = 20;            //�����蔻��̑傫��
	player.life = 1;          //�����Ă邩����ł邩�̔���(1=�����Ă����ԁA0=����ł�����)
	player.image = CatImg[1]; //���ݍs���Ă���A�j���[�V���������[
	player.pos.x = 320;       //�v���C���[��X���W�̈ʒu
	player.pos.y = 350;       //�v���C���[��Y���W�̈ʒu

	 //�L�̃A�j���[�V�����ɂ��Ă̕ϐ�
	int CatAniL = 0;  //�L�����������Ă��鎞�A�Q�p�^�[���̃A�j���[�V�����𓮂������߂̕ϐ�
	int CatAniR = 2;  //�L���E�������Ă��鎞�A�Q�p�^�[���̃A�j���[�V�����𓮂������߂̕ϐ�
	int CatDir = 0;  //�L�������Ă�������̔���(0=������������ԁA1=�E�����������)

	//����p�ϐ�
	player.onGround = TRUE;  //�v���C���[���n��ɂ��邩���Ȃ����𔻒�(TRUE=�n��ɂ���AFALSE=�󒆂ɂ���)
	int JumpStop = FALSE;    //�W�����v���A���Ŕ��������Ȃ����߂̕ϐ�(FALSE=�W�����vOK�ATRUE���W�����v�ł��܂��[��)
	int UPmove = FALSE;      //������ւ̃W�����v�����邩���Ȃ����𔻒�(TRUE=���Ă܂��AFALSE=���Ă܂���)
	int death = FALSE;       //�Q�[�����I�����������ĂȂ����𔻒�(FALSE=�p���ATRUE=�I��)

	//�������ւ̃W�����v���ɁA���n�n�_�����肵�A�v���C���[�������Ŏ~�܂�l�ɂ���ׂ̕ϐ�
	int JumpPoint = 0;

	//�X�N���[����������
	int score = 0;

	//�P�X�e�[�W���̎���񐔂𐔂���
	int loop = 0;

	//����X���W
	int trapX[38];
	for (i = 0; i < 38; i++)
		trapX[i] = 640;

	//���S���̔����̃A�j���[�V�����𓮂����ׂ̕ϐ�
	int anime = 0;

	while (ProcessMessage() == 0)
	{
		//�L�[���͊m�F
		ImputKey(death, &player, CatImg, moveVec, dt, &JumpStop, &inerVec, jumpVec, scroVec, &JumpPoint, &UPmove, se, &CatAniL, &CatAniR, &CatDir);

		//break�p�L�[���͊m�F����
		GetHitKeyStateAll(keyBuf);
		if (keyBuf[KEY_INPUT_ESCAPE] == 1 && !death)
			break;

		//�d�͏���
		if (!player.onGround)
			inerVec.y += gravVec.y;

		//�ړ�����
		player.pos.x += inerVec.x*dt;
		player.pos.y += inerVec.y*dt;

		//���n�n�_�v�Z
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

		//�n�ʂɂ���Ƃ��̏�����
		if (player.onGround)
		{
			JumpStop = FALSE;
			inerVec.x = 0;
			inerVec.y = 0;
		}

		//�����蔻��̊m�F
		GrandCheak(&death, &player, score, trapX);

		//�`�揈��
		GraphImput(death, player, score , trapX, scroVec, dt, font2, font3, &anime, font1, se, bomb, loop);
		DrawFormatString(0, 0, GetColor(0, 0, 0), "<�i�񂾋���>%5dm", score + (11000 * loop));
		ScreenFlip();

		//�v���C���[�ɃX�N���[�����ʂ�������
		player.pos.x -= scroVec.x * dt;

		//����v��
		if (score > 11000)
		{
			score = 0;
			loop++;
			for (i = 0; i < 38; i++)
				trapX[i] = 640;
		}

		//���񂾎��̏���
		if (death)
		{
			anime++;
			if (keyBuf[KEY_INPUT_Z] == 1)
				break;
		}
		else
			score += scroVec.x * dt;  //�X�R�A(�i�񂾋���)�̌v�Z
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