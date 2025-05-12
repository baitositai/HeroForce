#pragma once

#include"../../../JobManagerBase.h"

class PlMage :public JobManagerBase
{
public:
	PlMage(const SceneManager::CNTL _cntl);
	PlMage(const InputManager::JOYPAD_NO _padNum);

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
};
