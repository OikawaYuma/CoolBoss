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
	player.accel = { 0,-0.5 };
	player.WayGh[0] = {
		Novice::LoadTexture("./Resources/player.png")
	};

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




	//スクリーン座標に変換
	playerScreen.pos.y = player.pos.y - 670;
	playerScreen.pos.y *= -1;

	floorLeftScreen.pos.y = floorLeft.pos.y - 670;
	floorLeftScreen.pos.y *= -1;

	floorRightScreen.pos.y = floorRight.pos.y - 670;
	floorRightScreen.pos.y *= -1;

	floorCenterScreen.pos.y = floorCenter.pos.y - 670;
	floorCenterScreen.pos.y *= -1;

	StageSprite background;
	background.pos = { -10,-10 };
	background.size = { 1300,740 };

	StageSprite floor;
	floor.pos = { -10,670 };
	floor.size = { 1300,50 };

	//シーン
	int sceanNum = TITLE;
	int sceanPhase = PLAYERPHASE;
	int playerMode = PLAYERMOVE;
	int bossMode = THUNDER;





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
			player.direction.x = 0;
			player.direction.y = 0;
			player.velocity.x = 5;

			player.accel.y = -0.5;
				if (keys[DIK_SPACE] && player.jumpFlag == false &&( player.pos.y == 100||
				(player.pos.y == floorLeft.pos.y + player.size.y&& player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x)||
				(player.pos.y == floorRight.pos.y + player.size.y && player.pos.x + player.size.x >= floorRight.pos.x && player.pos.x <= floorRight.pos.x + floorRight.size.x)||
				(player.pos.y == floorCenter.pos.y + player.size.y && player.pos.x + player.size.x >= floorCenter.pos.x && player.pos.x <= floorCenter.pos.x + floorCenter.size.x))) {

				player.direction.y += 1.0f;//ジャンプのための速度を追加
				player.jumpFlag = true;
				player.velocity.y = 15;

			}

			player.velocity.y += player.accel.y;
			player.pos.y += player.velocity.y;
			
			/*------------------------床との当たり判定----------------------*/
			//左の床
			if (player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x && player.pos.y - player.size.y >= floorLeft.pos.y) {
				floorLeft.rideFlag = true;

			}
			else if((player.pos.x + player.size.x < floorLeft.pos.x || player.pos.x > floorLeft.pos.x + floorLeft.size.x )&& player.pos.y - player.size.y <= floorLeft.pos.y) 
			{ floorLeft.rideFlag = false; }
			


			if (floorLeft.rideFlag == true) {
				if (player.pos.x + player.size.x >= floorLeft.pos.x && player.pos.x <= floorLeft.pos.x + floorLeft.size.x && player.pos.y - player.size.y <= floorLeft.pos.y ) {
					player.pos.y  = floorLeft.pos.y + player.size.y;
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
				switch (bossMode) {
				case THUNDER: {//雷攻撃の処理 担当なち


					break;
				}
				case DEATHBALL: {//デスボール攻撃の処理
					break;
				}
				case UPDOWNATACK: {//上下攻撃 担当だいき


					break;
				}
				case GYARIKKUHOU: {
					break;
				}
				}
				break;
			}


			case PLAYERPHASE: {
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



			////背景
			/*Novice::DrawBox(
				background.pos.x, background.pos.y,
				background.size.x, background.size.y,
				0.0f,
				0xFFFFFFFF,
				kFillModeSolid);*/
				//床
			Novice::DrawBox(
				floor.pos.x, floor.pos.y,
				floor.size.x, floor.size.y,
				0.0f,
				BLACK,
				kFillModeSolid);

			Novice::DrawBox(
				floorLeft.pos.x, floorLeftScreen.pos.y,
				floorLeft.size.x, floorLeft.size.y,
				0.0f,
				BLACK,
				kFillModeSolid);
			Novice::DrawBox(
				floorRight.pos.x, floorRightScreen.pos.y,
				floorRight.size.x, floorRight.size.y,
				0.0f,
				BLACK,
				kFillModeSolid);
			Novice::DrawBox(
				floorCenter.pos.x, floorCenterScreen.pos.y,
				floorRight.size.x, floorRight.size.y,
				0.0f,
				BLACK,
				kFillModeSolid);
			/*------------------------------------------------------
							 自機の描画
			-------------------------------------------------------*/
			Novice::DrawSprite(
				player.pos.x, playerScreen.pos.y,
				player.WayGh[0],
				1.0f, 1.0f,
				0.0f,
				0xFFFFFFFF);

			Novice::ScreenPrintf(10, 10, "player.pos.y:%f", player.pos.y);
			Novice::ScreenPrintf(300, 10, ":%f", player.size.y+floorLeft.pos.y);
			Novice::ScreenPrintf(10, 40, "player.jumpFlag:%d", player.jumpFlag);

			Novice::ScreenPrintf(10, 70, "player.jumpFlag:%f", player.pos.x);
			Novice::ScreenPrintf(10, 100, "player.jumpFlag:%f", floorLeft.pos.x);
			Novice::ScreenPrintf(10, 120, "ride:%d", floorLeft.rideFlag);
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
