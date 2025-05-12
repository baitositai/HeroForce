#pragma once
class Timer
{
	//a
public:

	//���Ԃ̏���E����
	static constexpr int TIME_MAX = 59;
	static constexpr int TIME_MIN = 0;

	//�t�H���g�T�C�Y�����l
	static constexpr int TIME_FONT_SIZE = 50;

	//�^�C�}�[�w�i�T�C�Y
	static constexpr float TIMER_BACK_RATE = 0.8f;
	static constexpr int TIMER_BACK_SIZE_X = 298 * TIMER_BACK_RATE;
	static constexpr int TIMER_BACK_SIZE_Y = 168 * TIMER_BACK_RATE;

	//�����傫��
	static constexpr float NUM_RATE = 0.75f;

	//�����`�搔
	static constexpr int NUM_CNT = 2;

	//�����̕`��ʒu
	static constexpr int NUM_POS_X[NUM_CNT] = {
		Application::SCREEN_SIZE_X / 2 - 30,
		Application::SCREEN_SIZE_X / 2 + 30,
	};
	static constexpr int NUM_POS_Y = 64;

	void Update();
	void Draw();
	void Reset();	//�f�o�b�O�p
	void Release();
	void Destroy();

	const bool IsEnd(void) { return isEnd_; }

	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static Timer& GetInstance(void);

private:

	static Timer* instance_;

	int minute_;	//����
	int second_;	//�b��

	bool isEnd_;	//�^�C�}�[�I���̒m�点

	int cnt_;	//�J�E���^�[

	//int font_;	//�t�H���g�n���h��
	//int fontSize_;	//�t�H���g�̑傫��
	//int strWidth_;	//������̉��T�C�Y

	VECTOR pos_;	//�ʒu

	int imgTimerBack_;	//�w�i
	int *imgNumbers_;	//�����摜
	
	void ReduceTime(void);

	Timer(void);
	~Timer(void);
};

