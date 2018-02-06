#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//文字の表示板
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "easing.h"

namespace Board
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("ボード");	//グループ名
	const  string  defName("NoName");	//タスク名
	//-------------------------------------------------------------------
	class  Resource
	{
		bool  Initialize();
		bool  Finalize();
		Resource();
	public:
		~Resource();
		typedef  shared_ptr<Resource>	SP;
		typedef  weak_ptr<Resource>		WP;
		static   WP  instance;
		static  Resource::SP  Create();
		//共有する変数はここに追加する
		string ready_R_ImgName;
		string ready_E_ImgName;
		string ready_A_ImgName;
		string ready_D_ImgName;
		string ready_Y_ImgName;
		string ui_Score_ImgName;
		string ui_Time_ImgName;
		string ui_speed_ImgName;
		string ui_Tail_ImgName;
		string number_ImgName;
		string mps_ImgName;
		string gameOver_Img;
	};
	//-------------------------------------------------------------------
	class  Object : public  BTask
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize();
		bool  B_Finalize();
		bool  Initialize();	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		//変数
		//絵で表現する文字
		ML::Vec2 ready[5];
		ML::Vec2 ui_Score;
		ML::Vec2 ui_Time;
		ML::Vec2 ui_speed;
		ML::Vec2 ui_Tail;
		ML::Vec2 ui_GameOver;
		ML::Box2D src_Number[11];

		int timeCnt;
		int score;
		bool show_GameOver;
		//メソッド
		void Appear_Score_Board();
		void Draw_Score_Board();
		void UpDate_Ready();
		void Draw_Ready();
		void Draw_Tail_Size();
		void Draw_Time();
		void Draw_Score(int score);
		void Draw_speed();
		void Draw_GameOver();
		void UpDate_GameOver();
	};	
}