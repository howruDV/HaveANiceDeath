#include "pch.h"
#include "func.h"

#include <random>

int Random(int min, int max)
{
	int ret = 0;
	std::random_device rd;  // true 랜덤 수 발생기
	std::mt19937 gen(rd()); // mersenne_twister_engine 알고리즘 사용
	std::uniform_int_distribution<int> dis(min, max); // 분포 설정

	return dis(gen); // 랜덤 정수 반환
}
