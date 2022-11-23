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
	RUSH,//連続攻撃
	SORD,//剣攻撃
	KAMEHAMEHA,//かめはめ波
};
enum bossMode {
	BOSSWAY,
	THUNDER,    //雷
	DEATHBALL,  //デスボール
	UPDOWNATACK,//上下攻撃
	GYARIKKUHOU //ギャリック法
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
	player.WayGh[0] = {
		Novice::LoadTexture("./Resources/player.png")
	};
	player.HP = 100;

	Tmp playerT;
	Tmp playerTScreen;


	playerT.tmp.pos.x = player.pos.x;
	playerT.tmp.pos.y = player.pos.y;
	/*-----------------------------------------------------------
	                     ボス戦の処理
	----------------------------------------------------------*/
	//ボス攻撃の選択
	int bossModeTimer = 0;
	int atackFlag = false;
	int atackTimer = 0;
	int bossAtackNum = 1;
	int bossAtackNumTmp = bossAtackNum;






	/*--------------床----------------------------*/

	//LEFT
	Floatingflooar floorLeft;
	Floatingflooar floorLeftScreen;
	floorLeft.pos = { 100,200 };
	floorLeft.size = { 250,10 };
	floorLeft.rideFlag = false;
	floorLeft.fallFlag = false;

	//LEFT
	Floatingflooar floorRight;
	Floatingflooar floorRightScreen;
	floorRight.pos = { 930,200 };
	floorRight.size = { 250,10 };
	floorRight.rideFlag = false;
	floorRight.fallFlag = false;

	Floatingflooar floorCenter;
	Floatingflooar floorCenterScreen;
	floorCenter.pos = { 515,400 };
	floorCenter.size = { 250,10 };
	floorCenter.rideFlag = false;
	floorCenter.fallFlag = false;
	//ステージ背景
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
	detheBall.pos = {1000,1100};
	detheBall.size = { 600,600 };
	detheBall.t = 0;
	detheBall.tergetFlag = true;
	detheBall.gh = Novice::LoadTexture("./Resources/detheBallDan.png");
	//上下攻撃
	int isUpDownAtack = 0;
	int UpatackX = 0;
	int UpatackY = 0;
	int DownatackX = 0;
	int DownatackY = 480;
	int AtackWidth = 1280;
	int AtackHeight = 280;
	int upDownAtackGh = Novice::LoadTexture("./Resources/upDownAtack.png");

	

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
			if (keys[DIK_SPACE] && preKeys[DIK_SPACE] == 0) {
				sceanNum = BOSSPLAY;
			}
			break;
		}
		case BOSSPLAY: {
			
			
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

			if (keys[DIK_D]) {
				player.direction.x += 1.0f;
				if (player.pos.x >= 1230) {
					player.velocity.x = 0;

				}
			}
			else if (keys[DIK_A]) {
				player.direction.x -= 1.0f;
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
				if (keys[DIK_SPACE] &&preKeys[DIK_SPACE] ==0&& player.jumpFlag == false && (player.pos.y == 100 ||
					//下三つの条件は
					(player.pos.y == floorLeft.pos.y + player.size.y && player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x) ||
					(player.pos.y == floorRight.pos.y + player.size.y && player.pos.x + player.size.x >= floorRight.pos.x && player.pos.x <= floorRight.pos.x + floorRight.size.x) ||
					(player.pos.y == floorCenter.pos.y + player.size.y && player.pos.x + player.size.x >= floorCenter.pos.x && player.pos.x <= floorCenter.pos.x + floorCenter.size.x))) {

					player.direction.y += 1.0f;//ジャンプのための速度を追加
					player.jumpFlag = true;
					player.velocity.y = 17;

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

					break;
				}
				case DEATHBALL: {//デスボール攻撃の処理
					if (keys[DIK_R]) {
						bossMode = BOSSWAY;
					}
					if (atackTimer<=100&& atackFlag ==false) {
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
						detheBall.pos.x = (1.0f - detheBall.t) * detheBall.pos.x + detheBall.t * (playerT.tmp.pos.x-275 );
						detheBall.pos.y = (1.0f - detheBall.t) * detheBall.pos.y + detheBall.t * (playerT.tmp.pos.y +250);
					}
					if (atackTimer >=  200&& atackFlag == true) {
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
					if (atackTimer >= 150&&atackFlag == false) {
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
							if ((gyarikku.pos.y > player.pos.y-player.size.y && player.pos.y < gyarikku.pos.y + gyarikku.size.y))
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
				break;
			}
			case LASTPHASE: {
				break;
			}
			}


			break;
		}
		case GAMECLEAR: {
			break;
		}
		case GAMEOVER: {
			break;
		}
		}
		//スクリーン座標に変換
		detheBallScreen.pos.y = detheBall.pos.y - 670;
		detheBallScreen.pos.y *= -1;

		playerTScreen.tmp.pos.y = playerT.tmp.pos.y - 670;
		playerTScreen.tmp.pos.y *= -1;
		///
		/// ↑更新処理ここまで
		///

		///
		/// ↓描画処理ここから
		///
		switch (sceanNum) {
		case TITLE: {
			break;
		}
		case BOSSPLAY: {

			
			


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
				case THUNDER: {//雷攻撃の処理 担当なち


					break;
				}
				case DEATHBALL: {//デスボール攻撃の処理
					
						Novice::DrawSprite(
							playerT.tmp.pos.x-275, playerTScreen.tmp.pos.y-250,
							detheBall.gh,
							1.0f,1.0f,
							0.0f,
							0xFFFFFFFF);
					
					if (atackFlag == true) {
						Novice::DrawBox(detheBall.pos.x, detheBallScreen.pos.y, detheBall.size.x, detheBall.size.y, 0.0f, GREEN, kFillModeSolid);
					}

					break;
				}
				case UPDOWNATACK: {//上下攻撃 担当だいき
					if (atackFlag == false) {
						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								UpatackX-10, UpatackY-10,
								upDownAtackGh,
								1.0f,1.0f,
								0.0f,
								0xFFFFFFFF);
						}

						if (isUpDownAtack == 0)
						{
							Novice::DrawSprite(
								DownatackX-10, DownatackY,
								upDownAtackGh,
								1.0f,1.0f,
								0.0f,
								0xFFFFFFFF);
						}
					}
					if (atackFlag == true) {
						if (isUpDownAtack == 0)
						{
							Novice::DrawBox(UpatackX, UpatackY, AtackWidth, AtackHeight, 0.0f, RED, kFillModeSolid);
						}

						if (isUpDownAtack == 0)
						{
							Novice::DrawBox(DownatackX, DownatackY, AtackWidth, AtackHeight, 0.0f, RED, kFillModeSolid);
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
				
				break;
			}
			}
			
			/*------------------------------------------------------
							 自機の描画
			-------------------------------------------------------*/
			Novice::DrawSprite(
				player.pos.x+background.shake, playerScreen.pos.y+background.shake ,
				player.WayGh[0],
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);
			//HPバー
			Novice::DrawQuad(
				playerHP.pos.x, playerHP.pos.y,
				player.HP * 5, playerHP.pos.y,
				playerHP.pos.x, playerHP.pos.y + playerHP.size.y,
				player.HP * 5, playerHP.pos.y + playerHP.size.y,
				500, 25,
				1.0f, 1.0f,
				playerHP.gh,
				0xFFFFFFFF
			);



			Novice::ScreenPrintf(10, 10, "bossMode:%d", bossMode);
			Novice::ScreenPrintf(10, 30, "bossModeT:%d", bossModeTimer);

			Novice::ScreenPrintf(10, 50, "ban:%d", bossAtackNum);
			Novice::ScreenPrintf(10, 70, "bant:%d", bossAtackNumTmp);
			
			break;
		}
		case GAMECLEAR: {
			break;
		case GAMEOVER: {
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
