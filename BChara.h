#pragma once
#pragma warning(disable:4996)

//-------------------------------------------------
//�L�����N�^�̃X�[�p�[�N���X
//-----------------------------------------------------------------------

#include "GameEngine_Ver3_7.h"


class BChara : public BTask
{
//�ύX�s��--------------------------------------------------------------
public:
	typedef shared_ptr<BChara> SP;
	typedef weak_ptr<BChara> WP;


//�ύX�\--------------------------------------------------------------------

public:
	enum Angle4
	{
		Up = 270,
		Down = 90,
		Left = 180,
		Right = 0,
	};
	//���ʕϐ�
	ML::Vec3 pos;//���݈ʒu
	ML::Box3D hitBase;//�����蔻��p�̋�`
	ML::Vec3 angle;//���݊p�x

	bool playing_Effect_Flag;
	int timeCnt;
	
	//���ʃ��\�b�h
	virtual void Change_Angle(Angle4& playerAngle, bool Rotation_is_true_Angle4_is_false);//�����]��
	virtual void Chara_CheckMove(ML::Vec3& est_);//�߂荞�܂Ȃ�����	

	//��������
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

