#include "../../Application.h"
#include "TextManager.h"

TextManager* TextManager::instance_ = nullptr;

//�V���O���g����
void TextManager::CreateInstance()
{
	//�C���X�^���X��nullptr�Ȃ珉�������ꂽ�̂������������s��
	if (instance_ == nullptr)
	{
		instance_ = new TextManager();
	}
	instance_->Init();
}

TextManager& TextManager::GetInstance()
{
	if (instance_ == nullptr)
	{
		TextManager::CreateInstance();
	}
	return *instance_;
}

void TextManager::Init()
{
    //�e�L�X�g�Ǘ�
    std::string text = (Application::PATH_TEXT + "textData.txt").c_str();
    textDatas_ = LoadDialogues(text);

    //�t�H���g�֌W������
    InitFont();
}

std::string TextManager::TextLoad(TEXTS _text)
{
    std::string* txt = _Load(_text);
    if (txt == nullptr)
    {
        return "null";
    }
    std::string ret = *txt;
    return ret;
}

int TextManager::GetTextSize(const std::string _txt,const int _num) const
{
    int ret = -1;
    size_t bufferSize = std::snprintf(nullptr, 0, _txt.c_str(), _num) + 1; // +1�͏I�[�̃k�������p
    char* buffer = new char[bufferSize];

    return ret;
}

void TextManager::Destroy()
{
    // �t�H���g�o�^����
    for (int i = 0; i < static_cast<int>(FONT_TYPE::MAX); i++)
    {
        if (RemoveFontResourceEx(
            (Application::PATH_FONT + fontPath_[i]).c_str(),
            FR_PRIVATE,
            NULL)
            == -1)
        {
            return;
        }
    }
    delete instance_;
}

void TextManager::InitFont()
{
    //�t�H���g�p�X�ݒ�
    fontPath_[static_cast<int>(FONT_TYPE::NORMAR)] = "msgothic.ttc";
    fontPath_[static_cast<int>(FONT_TYPE::DOT)] = "FontDot.otf";
    fontPath_[static_cast<int>(FONT_TYPE::LOGO)] = "Corporate-Logo-Bold-ver3.otf";
    fontPath_[static_cast<int>(FONT_TYPE::BOKUTACHI)] = "bokutachi.otf";
    fontPath_[static_cast<int>(FONT_TYPE::HANAZOME)] = "PENGS.ttf";
    fontPath_[static_cast<int>(FONT_TYPE::BANANA)] = "YDWbananaslipplus.otf";
    fontPath_[static_cast<int>(FONT_TYPE::PENGS)] = "PENGS.ttf";
    fontPath_[static_cast<int>(FONT_TYPE::KAKUMEI)] = "NikkyouSans-mLKax.ttf";

    //�t�H���g���O�ݒ�
    fontName_[static_cast<int>(FONT_TYPE::NORMAR)] = "�l�r �S�V�b�N";
    fontName_[static_cast<int>(FONT_TYPE::DOT)] = "�x�X�g�e��DOT";
    fontName_[static_cast<int>(FONT_TYPE::LOGO)] = "�R�[�|���[�g�E���S ver3 Bold";
    fontName_[static_cast<int>(FONT_TYPE::BOKUTACHI)] = "�ڂ������̃S�V�b�N";
    fontName_[static_cast<int>(FONT_TYPE::HANAZOME)] = "�͂Ȃ��߃t�H���g";
    fontName_[static_cast<int>(FONT_TYPE::BANANA)] = "�x�c�v �o�i�i�X���b�v plus plus";
    fontName_[static_cast<int>(FONT_TYPE::PENGS)] = "PENGS";
    fontName_[static_cast<int>(FONT_TYPE::KAKUMEI)] = "Nikkyou Sans";

    // �t�H���g���V�X�e���ɓo�^
    for (int i = 0; i < static_cast<int>(FONT_TYPE::MAX); i++)
    {
        if (AddFontResourceEx(
            (Application::PATH_FONT + fontPath_[i]).c_str(),
            FR_PRIVATE,
            NULL)
            == -1)
        {
            return;
        }
    }
}

std::unordered_map<TextManager::TEXTS, std::string>
TextManager::LoadDialogues(const std::string& _filename)
{
    std::unordered_map<TEXTS, std::string> dialogues;
    std::ifstream file(_filename);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            size_t separatorPos = line.find(":");
            if (separatorPos != std::string::npos) 
            {
                int id = std::stoi(line.substr(0, separatorPos));
                std::string dialogue = line.substr(separatorPos + 1);

                // id��񋓌^�ɃL���X�g���ă}�b�v�ɒǉ�
                dialogues[static_cast<TEXTS>(id)] = dialogue;
            }
        }
        file.close();
    }
    else 
    {
        std::cerr << "�t�@�C�����J���܂���ł����B" << std::endl;
    }

    return dialogues;
}

std::string* TextManager::_Load(const TEXTS _text)
{
    std::string* ret = new std::string;
    if (textDatas_.find(_text) != textDatas_.end())
    {
        *ret = textDatas_.at(_text);
        return ret;
    }

    *ret = "�Z���t��������܂���B";

    return ret;
}

TextManager::TextManager()
{
}
