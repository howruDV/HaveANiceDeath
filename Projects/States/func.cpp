#include "pch.h"
#include "func.h"

#include <random>

int Random(int min, int max)
{
	int ret = 0;
	std::random_device rd;  // true ���� �� �߻���
	std::mt19937 gen(rd()); // mersenne_twister_engine �˰��� ���
	std::uniform_int_distribution<int> dis(min, max); // ���� ����

	return dis(gen); // ���� ���� ��ȯ
}
