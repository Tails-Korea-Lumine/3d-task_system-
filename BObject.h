#pragma once
#pragma warning (disable:4996)

//------------------------------------------------------------
//オブジェクト用スーパークラス
//----------------------------------------------------------

#include "GameEngine_Ver3_7.h"
#include "Task_Map3d.h"
#include "MyPG.h"

class BObject : public BTask
{	
//変更不可--------------------------------------------------------------
public:
	typedef shared_ptr<BObject> SP;
	typedef weak_ptr<BObject> WP;


//変更可能---------------------------------------------------------------------
public:
	//オブジェクト共通変数
	ML::Vec3 pos;
	ML::Box3D hitBase;
	ML::Vec3 scale;
	float fallspeed;

	//オブジェクト共通メソッド	
	virtual bool Object_Check_Bother();//着地しているかどうかを返す
	virtual bool Hit_Check_toPlayer(ML::Box3D playerHItbox, ML::Vec3 pos);//プレイヤとの当たり判定を返す
	virtual ML::Vec2 Object_Position_Mass();//マス単位の位置を返す

	BObject() :
		pos(0, 0, 0),
		hitBase(0, 0, 0, 0, 0, 0),
		fallspeed(20)
	{

	}

	virtual ~BObject() {}
};