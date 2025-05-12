#pragma once

#include"../../../JobManagerBase.h"
class Knight;
class PlKnight :public JobManagerBase
{
public:
	//�X�L��2�̖h��A�j���[�V�����̎~�߃t���[��
	static constexpr float GUARD_ANIM_STEP = 10.0f;
	PlKnight(const SceneManager::CNTL _cntl);
	PlKnight(const InputManager::JOYPAD_NO _padNum);
	~PlKnight(void) = default;

	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	//�U������
	void AtkInput(void)override;

	//�X�L��1����
	void SkillOneInput(void)override;

	//�X�L��2����
	void SkillTwoInput(void)override;

	//�X�L��������
	void SkillOneInit(void);
	void SkillTwoInit(void);
};