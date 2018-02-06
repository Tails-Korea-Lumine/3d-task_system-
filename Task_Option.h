#pragma warning(disable:4996)
#pragma once
//-------------------------------------------------------------------
//しっぽ
//-------------------------------------------------------------------
#include "GameEngine_Ver3_7.h"
#include "BObject.h"

namespace Tail
{
	//タスクに割り当てるグループ名と固有名
	const  string  defGroupName("オプション");	//グループ名
	const  string  defName("しっぽ");	//タスク名
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
		string meshName;
	};
	//-------------------------------------------------------------------
	class  Object : public  BObject
	{
	//変更不可◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆◆
	public:
		virtual  ~Object();
		typedef  shared_ptr<Object>		SP;
		typedef  weak_ptr<Object>		WP;
		//生成窓口 引数はtrueでタスクシステムへ自動登録
		static  Object::SP  Create(bool flagGameEnginePushBack_, ML::Vec3 vec , int m);
		Resource::SP	res;
	private:
		Object();
		bool  B_Initialize(ML::Vec3 vec, int m);
		bool  B_Finalize();
		bool  Initialize(ML::Vec3 vec, int m);	//「初期化」タスク生成時に１回だけ行う処理
		void  UpDate();		//「実行」１フレーム毎に行う処理
		void  Render2D_AF();	//「2D描画」１フレーム毎に行う処理
		bool  Finalize();		//「終了」タスク消滅時に１回だけ行う処理
		void  Render3D_L0();
		//変更可◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇◇
	public:
		//追加したい変数・メソッドはここに追加する
		int cnt;//1フレーム後に消滅するためのカウンタ
		int tailnum;
	};
}