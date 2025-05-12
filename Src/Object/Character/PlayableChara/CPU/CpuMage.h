#pragma once
#include "../../../JobManagerBase.h"
class CpuMage :
    public JobManagerBase
{
public:
	CpuMage(void);
	~CpuMage(void);
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

