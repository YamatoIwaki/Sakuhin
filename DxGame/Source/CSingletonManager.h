#pragma once

/*****************************************
*		シングルトンマネージャークラス
**/


template <class T>

class SingletonManager
{
public:

	static inline T* GetInstance()
	{
		static T instance;
		return &instance;
	}


protected:
	SingletonManager() {};
	virtual ~SingletonManager() {};

private:
	void operator = (const SingletonManager& obj)	{}
	SingletonManager(const SingletonManager &obj)	{}
};

