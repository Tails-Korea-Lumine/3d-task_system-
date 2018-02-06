#pragma once
#pragma warning (disable:4996)

//------------------------------------------------------------
//�I�u�W�F�N�g�p�X�[�p�[�N���X
//----------------------------------------------------------

#include "GameEngine_Ver3_7.h"
#include "Task_Map3d.h"
#include "MyPG.h"

class BObject : public BTask
{	
//�ύX�s��--------------------------------------------------------------
public:
	typedef shared_ptr<BObject> SP;
	typedef weak_ptr<BObject> WP;


//�ύX�\---------------------------------------------------------------------
public:
	//�I�u�W�F�N�g���ʕϐ�
	ML::Vec3 pos;
	ML::Box3D hitBase;
	ML::Vec3 scale;
	float fallspeed;

	//�I�u�W�F�N�g���ʃ��\�b�h	
	virtual bool Object_Check_Bother();//���n���Ă��邩�ǂ�����Ԃ�
	virtual bool Hit_Check_toPlayer(ML::Box3D playerHItbox, ML::Vec3 pos);//�v���C���Ƃ̓����蔻���Ԃ�
	virtual ML::Vec2 Object_Position_Mass();//�}�X�P�ʂ̈ʒu��Ԃ�

	BObject() :
		pos(0, 0, 0),
		hitBase(0, 0, 0, 0, 0, 0),
		fallspeed(20)
	{

	}

	virtual ~BObject() {}
};