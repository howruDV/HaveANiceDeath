#pragma once

// =======================================
// singleton.h:
// �̱������� �����Ǵ� class�� ����ϴ� parent class (template class)
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
		atexit((FUNC_TYPE)(&CSingleton::Destroy)); //atexit: ���α׷� ���� ���� �� ȣ���� �Լ� ���
	}
	~CSingleton()
	{
	}
};

template<typename T>
T* CSingleton<T>::m_This = nullptr; //static ������ �ݵ�� �ʱ�ȭ �ʿ�