#include <Novice.h>
#include<math.h>
const char kWindowTitle[] = "LC1B_COOLBOSS";

typedef struct Vector2 {
	float x;
	float y;
};

typedef struct Character {
	Vector2  pos;     //位置
	Vector2  velocity;//速さ
	Vector2 direction;//方向
	Vector2 accel;    //加速
	Vector2 size;
	int jumpFlag;   //ジャンプしているかどうか
	float atackflag;  //攻撃しているかどうか
	float length;
	float unitRadius;
	int WayGh[10];
	int WayL[2];
	int moveR[5];
	int moveL[5];
	int HP;

};

typedef struct Floatingflooar {
	Vector2 pos;
	Vector2 size;
	int rideFlag;
	int fallFlag;
};
typedef struct Line {
	Vector2  start;
	Vector2 end;
};
//ステージに使い背景等の情報
typedef struct StageSprite {
	Vector2 pos;
	Vector2 size;
	int gh;
	int shake;

};

//シーン
enum sceanNum {
	TITLE,
	BOSSPLAY,
	GAMECLEAR,
	GAMEOVER
};
//自機、敵機どちらのフェーズか
enum sceanPhase {
	PLAYERPHASE,
	BOSSPHASE,
	LASTPHASE
};
enum playerMode {
	PLAYERMOVE,
	KAMEHAMEHA,//かめはめ波
	RUSH,//連続攻撃
	SORD,//剣攻撃

};
enum bossMode {
	BOSSWAY,
	THUNDER,    //雷
	DEATHBALL,  //デスボール
	UPDOWNATACK,//上下攻撃
	GYARIKKUHOU //ギャリック法
};
enum playerDir {
	playerRight,
	playerLeft,

};
typedef struct Thunder {
	Vector2 posA;
	Vector2 posB;
	Vector2 posC;
	Vector2 size;
	Vector2 velocity;
	int gh;
	int ghDan;

};
typedef struct Gyarikkuhou {
	Vector2 pos;
	Vector2 size;
	int gh;
	int ghDan;
	int timer;
};
typedef struct DetheBall {
	Vector2 pos;
	Vector2 size;
	Vector2 velocity;
	int gh;
	int ghDan;
	int timer;
	float t;
	int tergetFlag;
};

typedef struct Kamehameha {
	Vector2 pos;
	Vector2 size;
	Vector2 velocity;
	int gh;
	int ghDan;
};
typedef struct Tmp {
	Character tmp;
};

//Vector2 ScreenTrans() {
//
//}
// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, 1280, 720);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };
	/*----------------------------------------------------------
						   自機の初期化
	------------------------------------------------------------*/
	Character player;
	Character playerScreen;
	player.pos = { 0,100 };
	player.size = { 50,100 };
	player.velocity = { 10,0 };
	player.length = 0.0f;
	player.unitRadius = 1.0f;
	player.jumpFlag = false;
	player.accel = { 0,-0.2 };
	player.WayGh[0] = Novice::LoadTexture("./Resources/jiki.png");
	player.WayGh[1] = Novice::LoadTexture("./Resources/jikiw.png");

	player.moveR[0] = Novice::LoadTexture("./Resources/jikimr1.png");
	player.moveR[1] = Novice::LoadTexture("./Resources/jikimr2.png");
	player.moveR[2] = Novice::LoadTexture("./Resources/jikimr3.png");
	player.moveR[3] = Novice::LoadTexture("./Resources/jikimr4.png");
	player.moveR[4] = Novice::LoadTexture("./Resources/jikimr5.png");

	player.WayL[0] = Novice::LoadTexture("./Resources/jikil.png");
	player.WayL[1] = Novice::LoadTexture("./Resources/jikilw.png");

	player.moveL[0] = Novice::LoadTexture("./Resources/jikiml1.png");
	player.moveL[1] = Novice::LoadTexture("./Resources/jikiml2.png");
	player.moveL[2] = Novice::LoadTexture("./Resources/jikiml3.png");
	player.moveL[3] = Novice::LoadTexture("./Resources/jikiml4.png");
	player.moveL[4] = Novice::LoadTexture("./Resources/jikiml5.png");
	player.HP = 100;

	Tmp playerT;
	Tmp playerTScreen;


	playerT.tmp.pos.x = player.pos.x;
	playerT.tmp.pos.y = player.pos.y;

	/*----------------------------------------------------------
							ボスの初期化
	------------------------------------------------------------*/
	Character boss;
	Character bossScreen;
	boss.pos = { 800,370 };
	boss.size = { 50,100 };
	boss.velocity = { 0,0 };
	boss.HP = 100;
	boss.WayGh[0] = Novice::LoadTexture("./Resources/boss.png");
	boss.WayGh[1] = Novice::LoadTexture("./Resources/boss1.png");
	boss.WayGh[2] = Novice::LoadTexture("./Resources/boss2.png");




	/*-----------------------------------------------------------
						 ボス戦の処理
	----------------------------------------------------------*/
	//ボス攻撃の選択
	int bossModeTimer = 0;
	int atackFlag = false;
	int atackTimer = 0;
	int bossAtackNum = 1;
	int playerAtackNum = 1;
	int bossAtackNumTmp = bossAtackNum;






	/*--------------床----------------------------*/

	//LEFT
	Floatingflooar floorLeft;
	Floatingflooar floorLeftScreen;
	floorLeft.pos = { 100,200 };
	floorLeft.size = { 250,10 };
	floorLeft.rideFlag = false;
	floorLeft.fallFlag = false;

	//right
	Floatingflooar floorRight;
	Floatingflooar floorRightScreen;
	floorRight.pos = { 930,200 };
	floorRight.size = { 250,10 };
	floorRight.rideFlag = false;
	floorRight.fallFlag = false;

	//senter
	Floatingflooar floorCenter;
	Floatingflooar floorCenterScreen;
	floorCenter.pos = { 515,400 };
	floorCenter.size = { 250,10 };
	floorCenter.rideFlag = false;
	floorCenter.fallFlag = false;

	//ステージ背景
	StageSprite title[2];
	title[0].pos = { 0,0 };
	title[0].gh = Novice::LoadTexture("./Resources/title.png");
	title[1].gh = Novice::LoadTexture("./Resources/title1.png");
	StageSprite gameClear;
	gameClear.pos = { 0,0 };
	gameClear.gh = Novice::LoadTexture("./Resources/gameClear.png");
	StageSprite gameOver;
	gameOver.pos = { 0,0 };
	gameOver.gh = Novice::LoadTexture("./Resources/gameOver.png");

	StageSprite background;
	background.pos = { -10,-10 };
	background.size = { 1300,740 };
	background.gh = Novice::LoadTexture("./Resources/background.png");
	background.shake = 0;

	StageSprite floor;
	floor.pos = { -10,670 };
	floor.size = { 1300,60 };

	StageSprite playerHP;
	playerHP.pos = { 25,25 };
	playerHP.size = { 500, 25 };
	playerHP.gh = Novice::LoadTexture("./Resources/playerHP.png");


	StageSprite bossHP;
	bossHP.pos = { 1255,25 };
	bossHP.size = { 500, 25 };
	bossHP.gh = Novice::LoadTexture("./Resources/bossHP.png");

	//ギャリック法の初期化
	Gyarikkuhou gyarikku;
	Gyarikkuhou gyarikkuScreen;
	gyarikku.pos = { 0,350 };
	gyarikku.size = { 1300,350 };
	gyarikku.gh = Novice::LoadTexture("./Resources/gyarikku.png");
	gyarikku.ghDan = Novice::LoadTexture("./Resources/gyarikkuDan.png");

	//デスボールの初期化
	DetheBall detheBall;
	DetheBall detheBallScreen;
	detheBall.pos = { 1000,1100 };
	detheBall.size = { 600,600 };
	detheBall.t = 0;
	detheBall.tergetFlag = true;
	detheBall.gh = Novice::LoadTexture("./Resources/detheBall.png");
	detheBall.ghDan = Novice::LoadTexture("./Resources/detheBallDan.png");

	//上下攻撃の初期化
	int isUpDownAtack = 0;
	int UpatackX = 0;
	int UpatackY = 0;
	int DownatackX = 0;
	int DownatackY = 480;
	int AtackWidth = 1280;
	int AtackHeight = 280;
	int upDownAtackGh = Novice::LoadTexture("./Resources/upDownAtack.png");
	int upDownAtackGhDan = Novice::LoadTexture("./Resources/upDownAtackDan.png");

	//サンダーの初期化
	Thunder thunder;
	Thunder thunderScreen;

	thunder.posA = { 95,670 };
	thunder.posB = { 490,670 };
	thunder.posC = { 875,670 };
	thunder.size = { 300,670 };
	thunder.gh = Novice::LoadTexture("./Resources/thunder.png");
	thunder.ghDan = Novice::LoadTexture("./Resources/thunderDan.png");

	//かめはめ波
	Kamehameha kamehameha;
	Kamehameha kamehamehaScreen;
	kamehameha.pos = { 0,350 };
	kamehameha.size = { 1300,350 };
	kamehameha.gh = Novice::LoadTexture("./Resources/kamehame.png");
	kamehameha.ghDan = Novice::LoadTexture("./Resources/kamehame1.png");



	//シーン
	int sceanNum = TITLE;
	int sceanPhase = BOSSPHASE;
	int playerMode = PLAYERMOVE;
	int bossMode = BOSSWAY;


	//スクリーン座標に変換
	playerScreen.pos.y = player.pos.y - 670;
	playerScreen.pos.y *= -1;
	playerTScreen.tmp.pos.y = playerT.tmp.pos.y - 670;
	playerTScreen.tmp.pos.y *= -1;

	//スクリーン座標に変換
	bossScreen.pos.y = boss.pos.y - 670;
	bossScreen.pos.y *= -1;

	floorLeftScreen.pos.y = floorLeft.pos.y - 670;
	floorLeftScreen.pos.y *= -1;

	floorRightScreen.pos.y = floorRight.pos.y - 670;
	floorRightScreen.pos.y *= -1;

	floorCenterScreen.pos.y = floorCenter.pos.y - 670;
	floorCenterScreen.pos.y *= -1;

	gyarikkuScreen.pos.y = gyarikku.pos.y - 670;
	gyarikkuScreen.pos.y *= -1;

	detheBallScreen.pos.y = detheBall.pos.y - 670;
	detheBallScreen.pos.y *= -1;

	thunderScreen.posA.y = thunder.posA.y - 670;
	thunderScreen.posA.y *= -1;
	thunderScreen.posB.y = thunder.posB.y - 670;
	thunderScreen.posB.y *= -1;
	thunderScreen.posC.y = thunder.posC.y - 670;
	thunderScreen.posC.y *= -1;

	kamehamehaScreen.pos.y = kamehameha.pos.y - 670;
	kamehamehaScreen.pos.y *= -1;

	int animanTimerW = 0;
	int animanTimerWW = animanTimerW / 20;

	int animanTimerM = 0;
	int animanTimerMM = animanTimerM / 10;

	int animanTimerBo = 0;
	int animanTimerBos = animanTimerBo / 20;
	int renda[2] = {
		 Novice::LoadTexture("./Resources/rennda.png"),
		  Novice::LoadTexture("./Resources/rennda1.png")
	};

	int titleTimer = 0;
	int titleTimerW = titleTimer / 20;

	int rendaTimer = 0;
	int rendaTimerW =rendaTimer / 20;

	int isPlayerMove = false;
	int playerDir = playerRight;
	int lastAtack = 0;
	int gyakamegh[5] = {
		Novice::LoadTexture("./Resources/gyakame.png"),
		 Novice::LoadTexture("./Resources/gyakame1.png"),
		  Novice::LoadTexture("./Resources/gyakame2.png"),
		   Novice::LoadTexture("./Resources/gyakame3.png"),
			Novice::LoadTexture("./Resources/gyakame4.png"), };
	//audio
	int soundH1 = Novice::LoadAudio("./Resources/title.mp3");
	int soundH2 = Novice::LoadAudio("./Resources/play.mp3");
	int soundH3 = Novice::LoadAudio("./Resources/over.mp3");
	int soundH4 = Novice::LoadAudio("./Resources/CLEAR.mp3");
	int voice1 = -1;
	int voice2 = -1;
	int voice3 = -1;
	int jump = Novice::LoadAudio("./Resources/jump.mp3");
	int php = Novice::LoadTexture("./Resources/php.png");
	int bhp = Novice::LoadTexture("./Resources/bhp.png");
	int kame = Novice::LoadAudio("./Resources/kame.mp3");
	int change = Novice::LoadAudio("./Resources/change.mp3");


	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///
		switch (sceanNum) {
		case TITLE: {
			titleTimer++;
			titleTimerW = titleTimer / 80;

			if (titleTimer >= 159) {
				titleTimer = 0;
			}


			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				sceanNum = BOSSPLAY;
				if (!Novice::IsPlayingAudio(change)) {
					Novice::PlayAudio(change, false, 1.0f);
				}
			}
			break;
		}
		case BOSSPLAY: {

			animanTimerW++;
			animanTimerWW = animanTimerW / 20;
			if (animanTimerW >= 39) {
				animanTimerW = 0;
			}
			animanTimerM++;
			animanTimerMM = animanTimerM / 10;
			if (animanTimerM >= 49) {
				animanTimerM = 0;
			}

			animanTimerBo++;
			animanTimerBos = animanTimerBo / 20;
			if (animanTimerBo >= 59) {
				animanTimerBo = 0;
			}
			rendaTimer++;
			rendaTimerW = rendaTimer / 10;
			if (rendaTimer >= 19) {
				rendaTimer = 0;
			}
			background.shake = 0;
			player.direction.x = 0;
			player.direction.y = 0;
			player.velocity.x = 5;

			player.accel.y = -0.6;


			player.velocity.y += player.accel.y;
			player.pos.y += player.velocity.y;





			//シェイク確認
			if (keys[DIK_Q]) {
				background.shake = rand() % 20 - 10;
			}



			if (player.pos.y <= 100) {
				player.pos.y = 100;
				player.jumpFlag = false;
			}
			if (player.pos.y < 100) {
				player.velocity.x = 0.0f;
			}

			isPlayerMove = false;
			if (keys[DIK_D]) {
				if (playerMode == PLAYERMOVE) {

					isPlayerMove = true;
				}
				playerDir = playerRight;
				player.direction.x += 1.0f;
				if (player.pos.x >= 1230) {
					player.velocity.x = 0;


				}
			}
			else if (keys[DIK_A] && playerMode == PLAYERMOVE) {
				player.direction.x -= 1.0f;
				if (playerMode == PLAYERMOVE) {

					isPlayerMove = true;
				}
				playerDir = playerLeft;
				if (player.pos.x <= 0) {
					player.velocity.x = 0;

				}
			}

			if (player.length != 0.0f) {
				player.length = sqrtf(player.direction.x * player.direction.x + player.direction.y * player.direction.y);
				player.direction.x = player.direction.x / player.length;
				player.direction.y = player.direction.y / player.length;
			}
			if (player.unitRadius != 0.0f) {
				float length = sqrtf(player.direction.x * player.direction.x + player.direction.y * player.direction.y);
				player.direction.x = player.direction.x / player.unitRadius;
				player.direction.y = player.direction.y / player.unitRadius;
			}

			player.pos.x += player.direction.x * player.velocity.x;
			player.pos.y += player.direction.y * player.velocity.y;

			playerScreen.pos.y = player.pos.y - 670;
			playerScreen.pos.y *= -1;
			switch (sceanPhase) {
			case BOSSPHASE: {
				/*------------------------床との当たり判定----------------------*/
		//左の床
				if (player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x && player.pos.y - player.size.y >= floorLeft.pos.y) {
					floorLeft.rideFlag = true;

				}
				else if ((player.pos.x + player.size.x < floorLeft.pos.x || player.pos.x > floorLeft.pos.x + floorLeft.size.x) && player.pos.y - player.size.y <= floorLeft.pos.y)
				{
					floorLeft.rideFlag = false;
				}

				if (floorLeft.rideFlag == true) {
					if (player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x && player.pos.y - player.size.y <= floorLeft.pos.y) {
						player.pos.y = floorLeft.pos.y + player.size.y;
						player.velocity.y = 0;
						player.jumpFlag = false;
					}

				}
				//右の床
				if (player.pos.x + player.size.x >= floorRight.pos.x && player.pos.x <= floorRight.pos.x + floorRight.size.x && player.pos.y - player.size.y >= floorRight.pos.y) {
					floorRight.rideFlag = true;

				}
				else if ((player.pos.x + player.size.x < floorRight.pos.x || player.pos.x > floorRight.pos.x + floorRight.size.x) && player.pos.y - player.size.y <= floorRight.pos.y)
				{
					floorRight.rideFlag = false;
				}

				if (floorRight.rideFlag == true) {
					if (player.pos.x + player.size.x >= floorRight.pos.x && player.pos.x <= floorRight.pos.x + floorRight.size.x && player.pos.y - player.size.y <= floorRight.pos.y) {
						player.pos.y = floorRight.pos.y + player.size.y;
						player.velocity.y = 0;
						player.jumpFlag = false;
					}

				}

				//中心の床
				if (player.pos.x + player.size.x >= floorCenter.pos.x && player.pos.x <= floorCenter.pos.x + floorCenter.size.x && player.pos.y - player.size.y >= floorCenter.pos.y) {
					floorCenter.rideFlag = true;

				}
				else if ((player.pos.x + player.size.x < floorCenter.pos.x || player.pos.x > floorCenter.pos.x + floorCenter.size.x) && player.pos.y - player.size.y <= floorCenter.pos.y)
				{
					floorCenter.rideFlag = false;
				}

				if (floorCenter.rideFlag == true) {
					if (player.pos.x + player.size.x >= floorCenter.pos.x && player.pos.x <= floorCenter.pos.x + floorCenter.size.x && player.pos.y - player.size.y <= floorCenter.pos.y) {
						player.pos.y = floorCenter.pos.y + player.size.y;
						player.velocity.y = 0;
						player.jumpFlag = false;
					}

				}
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0 && player.jumpFlag == false && (player.pos.y == 100 ||
					//下三つの条件は
					(player.pos.y == floorLeft.pos.y + player.size.y && player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x) ||
					(player.pos.y == floorRight.pos.y + player.size.y && player.pos.x + player.size.x >= floorRight.pos.x && player.pos.x <= floorRight.pos.x + floorRight.size.x) ||
					(player.pos.y == floorCenter.pos.y + player.size.y && player.pos.x + player.size.x >= floorCenter.pos.x && player.pos.x <= floorCenter.pos.x + floorCenter.size.x))) {

					player.direction.y += 1.0f;//ジャンプのための速度を追加
					player.jumpFlag = true;
					player.velocity.y = 17;
					if (!Novice::IsPlayingAudio(jump)) {
						Novice::PlayAudio(jump, false, 1.0f);
					}
				}


				if (bossAtackNum == 0) {

					bossAtackNumTmp++;
					bossAtackNum = bossAtackNumTmp;
					bossMode = BOSSWAY;
					sceanPhase = PLAYERPHASE;
					floorRight.rideFlag = false;
					floorLeft.rideFlag = false;
					floorCenter.rideFlag = false;
				}
				switch (bossMode) {
				case BOSSWAY: {
					bossModeTimer++;
					if (bossModeTimer >= 180) {
						bossModeTimer = 0;
						bossMode = rand() % 4 + 1;
					}
					break;
				}
				case THUNDER: {//雷攻撃の処理 担当なち
					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}

					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}
					atackTimer++;
					if (atackTimer >= 150 && atackFlag == false) {
						atackTimer = 0;
						atackFlag = true;
					}
					if (atackTimer >= 150 && atackFlag == true) {
						bossAtackNum--;
						atackTimer = 0;
						atackFlag = false;
						bossMode = BOSSWAY;
					}
					if (atackFlag == true && atackTimer >= 30) {


						if (thunder.posA.x < player.pos.x + player.size.x && player.pos.x < thunder.posA.x + thunder.size.x)
						{
							if ((thunder.posA.y > player.pos.y - player.size.y && player.pos.y < thunder.posA.y + thunder.size.y))
								player.HP--;
						}
					}
					if (atackFlag == true && atackTimer >= 60) {


						if (thunder.posB.x < player.pos.x + player.size.x && player.pos.x < thunder.posB.x + thunder.size.x)
						{
							if ((thunder.posB.y > player.pos.y - player.size.y && player.pos.y < thunder.posB.y + thunder.size.y))
								player.HP--;
						}
					}
					if (atackFlag == true && atackTimer >= 90) {


						if (thunder.posC.x < player.pos.x + player.size.x && player.pos.x < thunder.posC.x + thunder.size.x)
						{
							if ((thunder.posC.y > player.pos.y - player.size.y && player.pos.y < thunder.posC.y + thunder.size.y))
								player.HP--;
						}
					}


					break;
				}
				case DEATHBALL: {//デスボール攻撃の処理
					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}
					if (atackTimer <= 100 && atackFlag == false) {
						playerT.tmp.pos.x = player.pos.x;
						playerT.tmp.pos.y = player.pos.y;
					}
					detheBall.tergetFlag = false;
					atackTimer++;
					if (atackTimer >= 150 && atackFlag == false) {
						atackTimer = 0;
						atackFlag = true;

					}
					if (atackFlag == true) {

						detheBall.t += 0.0005f;
						detheBall.pos.x = (1.0f - detheBall.t) * detheBall.pos.x + detheBall.t * (playerT.tmp.pos.x - 275);
						detheBall.pos.y = (1.0f - detheBall.t) * detheBall.pos.y + detheBall.t * (playerT.tmp.pos.y + 250);
					}
					if (atackTimer >= 200 && atackFlag == true) {
						bossAtackNum--;
						atackTimer = 0;
						atackFlag = false;
						bossMode = BOSSWAY;
						detheBall.pos = { 1000,1100 };
						detheBall.t = 0;
					}
					if (atackFlag == true) {


						if (detheBall.pos.x < player.pos.x + player.size.x && player.pos.x < detheBall.pos.x + detheBall.size.x)
						{
							if ((detheBall.pos.y > player.pos.y - player.size.y && player.pos.y > detheBall.pos.y - detheBall.size.y))
								player.HP--;
						}
					}
					break;
				}
				case UPDOWNATACK: {//上下攻撃 担当だいき
					int color = WHITE;
					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}
					atackTimer++;
					if (atackTimer >= 150 && atackFlag == false) {
						atackTimer = 0;
						atackFlag = true;
					}
					if (atackTimer >= 150 && atackFlag == true) {
						bossAtackNum--;
						atackTimer = 0;
						atackFlag = false;
						bossMode = BOSSWAY;
					}
					if (atackFlag == true) {
						if (UpatackX < player.pos.x + player.size.x && player.pos.x < UpatackX + AtackWidth)
						{
							if ((UpatackY < player.pos.y + player.size.y && player.pos.y < UpatackY + AtackHeight))
								player.HP--;
						}

						if (DownatackX < player.pos.x + player.size.x && player.pos.x < DownatackX + AtackWidth)
						{
							if ((DownatackY < player.pos.y + player.size.y && player.pos.y < DownatackY + AtackHeight))
								player.HP--;
						}
					}
					break;
				}
				case GYARIKKUHOU: {
					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}
					atackTimer++;
					if (atackTimer >= 150 && atackFlag == false) {
						atackTimer = 0;
						atackFlag = true;
					}
					if (atackTimer >= 150 && atackFlag == true) {
						bossAtackNum--;
						atackTimer = 0;
						atackFlag = false;
						bossMode = BOSSWAY;
					}
					if (atackFlag == true) {

						if (gyarikku.pos.x < player.pos.x + player.size.x && player.pos.x < gyarikku.pos.x + gyarikku.size.x)
						{
							if ((gyarikku.pos.y > player.pos.y - player.size.y && player.pos.y < gyarikku.pos.y + gyarikku.size.y))
								player.HP--;
						}
					}

					break;
				}
				}

				if (player.HP <= 0) {
					player.HP = 0;
					sceanNum = GAMEOVER;
				}
				break;
			}


			case PLAYERPHASE: {
				if (keys[DIK_T]) {
					sceanPhase = BOSSPHASE;
				}
				switch (playerMode) {
				case PLAYERMOVE: {
					if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
						playerMode = playerAtackNum;
					}
					break;
				}

				case KAMEHAMEHA: {
					player.pos.x =-500;
					player.pos.y = 350;
					player.accel.y = 0;
					player.velocity.y = 0;
					atackTimer++;
					if (atackTimer >= 150 && atackFlag == false) {
						atackTimer = 0;
						atackFlag = true;
					}

					
					if (atackTimer >= 150 && atackFlag == true) {
						player.pos.x = 300;
						atackTimer = 0;
						atackFlag = false;
						playerMode = PLAYERMOVE;
						sceanPhase = BOSSPHASE;
						boss.HP -= 34;
						boss.pos.x = 800;
						
					}
					if (atackFlag == true) {
						boss.pos.x += 10;
					}

					
					break;
				}
				}

				break;
			}
			case LASTPHASE: {
				if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
					lastAtack++;

				}
				if (lastAtack >= 40) {
					boss.pos.x += 10;
				}
				if (boss.pos.x >= 2000) {
					boss.pos.x = 800;
					player.pos.x = 0;
					player.pos.y = 100;
					bossAtackNum = 1;
					playerDir = playerRight;
					sceanNum = GAMECLEAR;
					sceanPhase = BOSSPHASE;
					bossMode = BOSSWAY;
					playerMode = PLAYERMOVE;
					lastAtack = 0;

				}
				break;
			}
			}
			if (boss.HP <= 0) {
				player.pos = { -300,100 };
				boss.HP = 100;
				player.HP = 100;
				sceanPhase = LASTPHASE;
				boss.pos.x = 1100;

			}

			break;
		}
		case GAMECLEAR: {
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				sceanNum = TITLE;
				if (!Novice::IsPlayingAudio(change)) {
					Novice::PlayAudio(change, false, 1.0f);
				}
			}
			break;
		}
		case GAMEOVER: {
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE]) {
				sceanNum = TITLE;
				boss.pos.x = 800;
				player.HP = 100;
				boss.HP = 100;
				player.pos.x = 0;
				player.pos.y = 100;
				bossAtackNum = 1;
				playerDir = playerRight;
				sceanPhase = BOSSPHASE;
				bossMode = BOSSWAY;
				playerMode = PLAYERMOVE;
				lastAtack = 0;
				if (!Novice::IsPlayingAudio(change)) {
					Novice::PlayAudio(change, false, 1.0f);
				}
			}
			break;
		}
		}
		//スクリーン座標に変換
		detheBallScreen.pos.y = detheBall.pos.y - 670;
		detheBallScreen.pos.y *= -1;

		playerTScreen.tmp.pos.y = playerT.tmp.pos.y - 670;
		playerTScreen.tmp.pos.y *= -1;

		thunderScreen.posA.y = thunder.posA.y - 670;
		thunderScreen.posA.y *= -1;
		thunderScreen.posB.y = thunder.posB.y - 670;
		thunderScreen.posB.y *= -1;
		thunderScreen.posC.y = thunder.posC.y - 670;
		thunderScreen.posC.y *= -1;

		kamehamehaScreen.pos.y = kamehameha.pos.y - 670;
		kamehamehaScreen.pos.y *= -1;
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		switch (sceanNum) {
		case TITLE: {
			Novice::DrawSprite(
				0, 0,
				title[titleTimerW].gh,
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);
			Novice::StopAudio(voice3);
			if (Novice::IsPlayingAudio(voice1) == 0 || voice1 == -1) {
				voice1 = Novice::PlayAudio(soundH1, true,0.5f);
			}
			break;
		}
		case BOSSPLAY: {
			Novice::StopAudio(voice1);
			if (!Novice::IsPlayingAudio(voice2)  || voice2 == -1) {
				voice2 = Novice::PlayAudio(soundH2, true, 0.5f);
			}


			///背景
			Novice::DrawSprite(
				background.pos.x + background.shake, background.pos.y + background.shake,
				background.gh,
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);
			Novice::DrawBox(
				floor.pos.x + background.shake, floor.pos.y + background.shake,
				floor.size.x, floor.size.y,
				0.0f,
				BLACK,
				kFillModeSolid);

			/*------------------------------------------------------
							 自機の描画
			-------------------------------------------------------*/
			if (isPlayerMove == false && playerDir == playerRight) {
				Novice::DrawSprite(
					player.pos.x + background.shake, playerScreen.pos.y + background.shake,
					player.WayGh[animanTimerWW],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
			}
			if (isPlayerMove == true && playerDir == playerRight) {
				Novice::DrawSprite(
					player.pos.x + background.shake, playerScreen.pos.y + background.shake,
					player.moveR[animanTimerMM],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
			}


			if (isPlayerMove == true && playerDir == playerLeft) {
				Novice::DrawSprite(
					player.pos.x + background.shake, playerScreen.pos.y + background.shake,
					player.moveL[animanTimerMM],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);


			}
			if (isPlayerMove == false && playerDir == playerLeft) {
				Novice::DrawSprite(
					player.pos.x + background.shake, playerScreen.pos.y + background.shake,
					player.WayL[animanTimerWW],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
			}
			switch (sceanPhase) {
			case BOSSPHASE: {
				//床


				Novice::DrawBox(
					floorLeft.pos.x + background.shake, floorLeftScreen.pos.y + background.shake,
					floorLeft.size.x, floorLeft.size.y,
					0.0f,
					BLACK,
					kFillModeSolid);
				Novice::DrawBox(
					floorRight.pos.x + background.shake, floorRightScreen.pos.y + background.shake,
					floorRight.size.x, floorRight.size.y,
					0.0f,
					BLACK,
					kFillModeSolid);
				Novice::DrawBox(
					floorCenter.pos.x + background.shake, floorCenterScreen.pos.y + background.shake,
					floorRight.size.x, floorRight.size.y,
					0.0f,
					BLACK,
					kFillModeSolid);
				switch (bossMode) {
				case BOSSWAY: {

					break;
				}
				case THUNDER: {
					if (atackFlag == false) {
						Novice::DrawSprite(
							thunder.posA.x, thunderScreen.posA.y - 10,
							thunder.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
						Novice::DrawSprite(
							thunder.posB.x, thunderScreen.posB.y - 10,
							thunder.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
						Novice::DrawSprite(
							thunder.posC.x, thunderScreen.posC.y - 10,
							thunder.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}

					if (atackFlag == true && atackTimer >= 30) {
						Novice::DrawSprite(
							thunder.posA.x, thunderScreen.posA.y - 10,
							thunder.gh,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
						if (atackFlag == true && atackTimer >= 60) {
							Novice::DrawSprite(
								thunder.posB.x, thunderScreen.posB.y - 10,
								thunder.gh,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}
						if (atackFlag == true && atackTimer >= 90) {
							Novice::DrawSprite(
								thunder.posC.x, thunderScreen.posC.y - 10,
								thunder.gh,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}
					}


					break;
				}
				case DEATHBALL: {//デスボール攻撃の処理
					if (atackFlag == false) {
						Novice::DrawSprite(
							playerT.tmp.pos.x - 275, playerTScreen.tmp.pos.y - 250,
							detheBall.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}

					if (atackFlag == true) {
						Novice::DrawSprite(
							detheBall.pos.x, detheBallScreen.pos.y,
							detheBall.gh,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}

					break;
				}
				case UPDOWNATACK: {//上下攻撃 担当だいき
					if (atackFlag == false) {
						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								UpatackX - 10, UpatackY - 10,
								upDownAtackGhDan,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}

						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								DownatackX - 10, DownatackY,
								upDownAtackGhDan,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}
					}
					if (atackFlag == true) {
						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								UpatackX - 10, UpatackY - 10,
								upDownAtackGh,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}

						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								DownatackX - 10, DownatackY,
								upDownAtackGh,
								1.0f, 1.0f,
								0.0f,
								0xFFFFFFFF);
						}
					}



					break;
				}
				case GYARIKKUHOU: {
					if (atackFlag == false) {
						Novice::DrawSprite(
							gyarikku.pos.x + background.shake, gyarikkuScreen.pos.y + background.shake,
							gyarikku.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}
					//ギャリック法
					if (atackFlag == true) {
						Novice::DrawSprite(
							gyarikku.pos.x + background.shake, gyarikkuScreen.pos.y + background.shake,
							gyarikku.gh,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}


					break;
				}
				}
				//HPバー
				Novice::DrawSprite(
				playerHP.pos.x-10, playerHP.pos.y -10,
					php,
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				Novice::DrawSprite(
					bossHP.pos.x - 510, bossHP.pos.y - 10,
					bhp,
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				Novice::DrawQuad(
					playerHP.pos.x, playerHP.pos.y,
					playerHP.pos.x + player.HP * 5, playerHP.pos.y,
					playerHP.pos.x, playerHP.pos.y + playerHP.size.y,
					playerHP.pos.x + player.HP * 5, playerHP.pos.y + playerHP.size.y,
					500, 25,
					1.0f, 1.0f,
					playerHP.gh,
					0xFFFFFFFF
				);
				//HPバー
				Novice::DrawQuad(
					bossHP.pos.x, bossHP.pos.y,
					bossHP.pos.x - boss.HP * 5, bossHP.pos.y,
					bossHP.pos.x, bossHP.pos.y + bossHP.size.y,
					bossHP.pos.x - boss.HP * 5, bossHP.pos.y + bossHP.size.y,
					500, 25,
					1.0f, 1.0f,
					bossHP.gh,
					0xFFFFFFFF
				);
				break;
			}
			case PLAYERPHASE: {
				Novice::DrawSprite(
					boss.pos.x, bossScreen.pos.y,
					boss.WayGh[animanTimerBos],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				switch (playerMode) {
				case KAMEHAMEHA: {
					if (atackFlag == false) {
						Novice::DrawSprite(
							0, playerScreen.pos.y - 100,
							kamehameha.ghDan,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}
					if (atackFlag == true) {
						Novice::DrawSprite(
							0, playerScreen.pos.y - 100,
							kamehameha.gh,
							1.0f, 1.0f,
							0.0f,
							0xFFFFFFFF);
					}
					break;
				}

				}
				//HPバー
				Novice::DrawSprite(
					playerHP.pos.x - 10, playerHP.pos.y - 10,
					php,
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				Novice::DrawSprite(
					bossHP.pos.x - 510, bossHP.pos.y - 10,
					bhp,
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				Novice::DrawQuad(
					playerHP.pos.x, playerHP.pos.y,
					playerHP.pos.x + player.HP * 5, playerHP.pos.y,
					playerHP.pos.x, playerHP.pos.y + playerHP.size.y,
					playerHP.pos.x + player.HP * 5, playerHP.pos.y + playerHP.size.y,
					500, 25,
					1.0f, 1.0f,
					playerHP.gh,
					0xFFFFFFFF
				);
				//HPバー
				Novice::DrawQuad(
					bossHP.pos.x, bossHP.pos.y,
					bossHP.pos.x - boss.HP * 5, bossHP.pos.y,
					bossHP.pos.x, bossHP.pos.y + bossHP.size.y,
					bossHP.pos.x - boss.HP * 5, bossHP.pos.y + bossHP.size.y,
					500, 25,
					1.0f, 1.0f,
					bossHP.gh,
					0xFFFFFFFF
				);
				break;
			}
			case LASTPHASE: {
				Novice::DrawSprite(
					450, 100,
					renda[rendaTimerW],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				Novice::DrawSprite(
					boss.pos.x, bossScreen.pos.y,
					boss.WayGh[animanTimerBos],
					1.0f, 1.0f,
					0.0f,
					0xFFFFFFFF);
				player.pos.x = -500;
				player.pos.y = 350;
				player.accel.y = 0;
				player.velocity.y = 0;
				if (lastAtack <= 10&&lastAtack>=0) {
					Novice::DrawSprite(
						0, playerScreen.pos.y - 100,
						gyakamegh[0],
						1.0f, 1.0f,
						0.0f,
						0xFFFFFFFF);
				}
				if (lastAtack <= 20&&lastAtack>10) {
					Novice::DrawSprite(
						0, playerScreen.pos.y - 100,
						gyakamegh[1],
						1.0f, 1.0f,
						0.0f,
						0xFFFFFFFF);
				}
				if (lastAtack <= 30 && lastAtack > 20) {
					Novice::DrawSprite(
						0, playerScreen.pos.y - 100,
						gyakamegh[2],
						1.0f, 1.0f,
						0.0f,
						0xFFFFFFFF);
				}
				if (lastAtack < 40 && lastAtack > 30) {
					Novice::DrawSprite(
						0, playerScreen.pos.y - 100,
						gyakamegh[3],
						1.0f, 1.0f,
						0.0f,
						0xFFFFFFFF);
				}
				if (lastAtack>= 40 ) {
					Novice::DrawSprite(
						0, playerScreen.pos.y - 100,
						gyakamegh[4],
						1.0f, 1.0f,
						0.0f,
						0xFFFFFFFF);
				}
				break;
			}
			}


			





			/*Novice::ScreenPrintf(10, 10, "bossMode:%d", bossMode);
			Novice::ScreenPrintf(10, 30, "bossModeT:%d", bossModeTimer);

			Novice::ScreenPrintf(10, 50, "ban:%d", bossAtackNum);
			Novice::ScreenPrintf(10, 70, "bant:%d", animanTimerWW);*/

			break;
		}
		case GAMECLEAR: {
			Novice::StopAudio(voice2);
			if (Novice::IsPlayingAudio(voice3) == 0 || voice3 == -1) {
				voice3 = Novice::PlayAudio(soundH4, true, 0.5f);
			}
			Novice::DrawSprite(
				0, 0,
				gameClear.gh,
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);
			break;
		case GAMEOVER: {
			Novice::StopAudio(voice2);
			if (Novice::IsPlayingAudio(voice3) == 0 || voice3 == -1) {
				voice3 = Novice::PlayAudio(soundH3, true, 0.5f);
			}
			Novice::DrawSprite(
				0, 0,
				gameOver.gh,
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);
			break;
		}
		}
		}
		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
