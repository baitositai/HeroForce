#pragma once
#include "../../../JobManagerBase.h"
class CpuAxe :
    public JobManagerBase
{
public:
	CpuAxe(void);
	~CpuAxe(void);
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

