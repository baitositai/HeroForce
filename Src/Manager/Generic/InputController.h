#pragma once


class InputController
{
public:

	enum class KEY_ACTION
	{
		UP,
		DOWN,
		LEFT,
		RIGHT,
		DECIDE,
		CANCEL,
		SKILL_ONE,
		SKILL_TWO,
		
		MAX_ACTION
	};

	// �C���X�^���X�𖾎��I�ɐ���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static InputController& GetInstance(void);

	void Init(void);
	void Update(void);

	// ���\�[�X�̔j��
	void Destroy(void);

	void SetInput(void);

private:

	// �V���O���g���p�C���X�^���X
	static InputController* instance_;

	KEY_ACTION keyAction_;
};

