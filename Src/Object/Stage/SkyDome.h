#pragma once
#pragma once
#include <map>
#include <functional>
#include "../../Object/Common/Transform.h"

class SkyDome
{
public:

	static constexpr float ROT_SPEED = 0.05f;

	static constexpr float SCALE = 100.0f;
	static constexpr VECTOR SCALES = { SCALE, SCALE, SCALE };

	// ���
	enum class STATE
	{
		NONE,
		STAY,
		FOLLOW
	};

	// �R���X�g���N�^
	SkyDome();

	// �f�X�g���N�^
	~SkyDome(void);

	//������
	void Init(void);

	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//���
	void Release(void);

	//���g�̃g�����X�t�H�[����Ԃ�
	const Transform& GetTransform() const;

private:	
	
	// ���
	STATE state_;

	// ���f������̊�{���
	Transform transform_;

	// ��ԊǗ�(��ԑJ�ڎ���������)
	std::map<STATE, std::function<void(void)>> stateChanges_;

	// ��ԊǗ�(�X�V�X�e�b�v)
	std::function<void(void)> stateUpdate_;
};

