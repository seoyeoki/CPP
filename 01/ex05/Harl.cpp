#include "Harl.hpp"
#include <iostream>

Harl::Harl(void)
{
}

Harl::~Harl(void)
{
}

void Harl::debug(void)
{
	std::cout << "비행기에 다른 사람들이 너무 시끄럽네요." << std::endl;
	std::cout << "앞자리 사람은 아까부터 코를 골고, 옆자리 사람은 스마트폰 화면을 톡톡톡 터치하는 소리가 계속 거슬리고," << std::endl;
	std::cout << "뒷자리 사람은 기침을 쉬지 않고 해요. 이어폰을 꽂아도 소용이 없어요." << std::endl;
	std::cout << "제가 머리 위 에어컨 바람을 켜놓긴 했는데, 좀 춥지 않아요?" << std::endl;
	std::cout << "바람이 따뜻하면 좋을 거 같은데, 이건 왜 차가운 바람밖에 안 나오죠?" << std::endl;
	std::cout << "담요도 얇고, 발밑은 더 추워요." << std::endl;
}

void Harl::info(void)
{
	std::cout << "화장실이 너무 더러워요." << std::endl;
	std::cout << "세면대에 누가 오물을 묻혀놨는데, 아무도 치우지 않았어요." << std::endl;
	std::cout << "휴지는 바닥에 널려 있고, 물비누는 텅 비어 있고, 문 잠금장치는 반쯤 고장 나 있어요." << std::endl;
	std::cout << "아, 그리고 기내식이 너무 비려요. 생선 요리를 시켰는데 냄새가 온 좌석에 퍼졌어요." << std::endl;
	std::cout << "빵은 딱딱하고, 샐러드는 시들었고, 디저트는 미지근하게 녹아 있었어요." << std::endl;
}

void Harl::warning(void)
{
	std::cout << "비행기가 너무 흔들리는 거 같아요." << std::endl;
	std::cout << "아까부터 계속 덜컹거리는데, 이게 정상인 거 맞아요? 방금은 컵이 튀어 올랐어요." << std::endl;
	std::cout << "그런데 비행시간 내내 안전벨트를 해야 하는 게 말이 돼요?" << std::endl;
	std::cout << "벨트 때문에 허리는 아프고 잠도 못 자는데, 그렇다고 풀면 승무원이 와서 다시 채우라고 하잖아요." << std::endl;
	std::cout << "안내방송에서는 괜찮다고 하는데, 솔직히 하나도 안 괜찮아 보여요." << std::endl;
}

void Harl::error(void)
{
	std::cout << "기장 나와!!!!! 기장 나와!!!!! 기장 나와!!!!!" << std::endl;
	std::cout << "소음도, 냄새도, 흔들림도 더는 못 참겠어요! 당장 기장 불러주세요!!!!!" << std::endl;
	std::cout << "기장 나와!!!!! 기장 나와!!!!! 기장 나와!!!!!" << std::endl;
}

void Harl::complain(std::string level)
{
	const std::string levels[4] = { "DEBUG", "INFO", "WARNING", "ERROR" };
	void (Harl::*functions[4])(void) = { &Harl::debug, &Harl::info, &Harl::warning, &Harl::error };

	for (int i = 0; i < 4; i++)
	{
		if (level == levels[i])
		{
			(this->*functions[i])();
			return ;
		}
	}
}
