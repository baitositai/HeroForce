#pragma once

#include"../../../JobManagerBase.h"
class AxeMan;
class PlAxe :public JobManagerBase
{
public:
	//�X�L���P�̒��������Ă��Ȃ��������̍U������
	static constexpr float SKILL_ONE_START_NOCHARGE = 2.0f;


	PlAxe(const SceneManager::CNTL _cntl);
	PlAxe(const InputManager::JOYPAD_NO _padNum);
	~PlAxe(void) = default;


	void Init(void)override;
	void Update(void)override;
	void Draw(void)override;
	void Release(void)override;

private:
	void InitSkill(PlayerBase::ATK_ACT _act);

	//�U������
	void AtkInput(void)override;

	//�X�L��1����
	void SkillOneInput(void)override;

	//�X�L��2����
	void SkillTwoInput(void)override;
};
