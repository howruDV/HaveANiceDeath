#pragma once

// =======================================
// singleton.h:
// 싱글톤으로 구현되는 class가 상속하는 parent class (template class)
// =======================================

template <typename T>
class CSingleton
{
private:
	static T* m_This;

public:
	static T* GetInst()
	{
		if (!m_This)
			m_This = new T;

		return m_This;
	}

	static void Destroy()
	{
		if (m_This)
		{
			delete m_This;
			m_This = nullptr;
		}
	}

public:
	CSingleton()
	{
		typedef void(*FUNC_TYPE)(void);
		atexit((FUNC_TYPE)(&CSingleton::Destroy)); //atexit: 프로그램 정상 종료 시 호출할 함수 등록
	}
	~CSingleton()
	{
	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr; //static 변수는 반드시 초기화 필요