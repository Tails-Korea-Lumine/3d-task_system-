#pragma once
#pragma warning(disable:4996)

//-------------------------------------------------
//キャラクタのスーパークラス
//-----------------------------------------------------------------------

#include "GameEngine_Ver3_7.h"


class BChara : public BTask
{
//変更不可--------------------------------------------------------------
public:
	typedef shared_ptr<BChara> SP;
	typedef weak_ptr<BChara> WP;


//変更可能--------------------------------------------------------------------

public:
	enum Angle4
	{
		Up = 270,
		Down = 90,
		Left = 180,
		Right = 0,
	};
	//共通変数
	ML::Vec3 pos;//現在位置
	ML::Box3D hitBase;//あたり判定用の矩形
	ML::Vec3 angle;//現在角度

	bool playing_Effect_Flag;
	int timeCnt;
	
	//共通メソッド
	virtual void Change_Angle(Angle4& playerAngle, bool Rotation_is_true_Angle4_is_false);//方向転換
	virtual void Chara_CheckMove(ML::Vec3& est_);//めり込まない処理	

	//生成消滅
	BChara() :
		pos(0, 0, 0),
		hitBase(0, 0, 0, 0, 0, 0),
		angle(0, 0, 0),
		playing_Effect_Flag(false),
		timeCnt(0)
	{

	}

	virtual ~BChara()
	{

	}
};

