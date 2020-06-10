#include <iostream>

class CSingleton {
private:
	static CSingleton* singleton;

	CSingleton()
	{
		std::cout << "The Singleton object has been created." << std::endl;
	}

public:
	CSingleton(CSingleton&) = delete;
	CSingleton(CSingleton&&) = delete;
	~CSingleton()
	{	
		std::cout << "The Singleton object has been destroyed." << std::endl;
	}

	static CSingleton& getSingleton()
	{
		if (singleton == nullptr)
		{
			singleton = new CSingleton;
		}
		else
		{
			std::cout << "The Singleton object has been referenced." << std::endl;
		}
		
		return *singleton;
	}

	static void deleteSingleton()
	{
		if (singleton != nullptr)
		{
			delete singleton;
			singleton = nullptr;
		}
	}
};

CSingleton *CSingleton::singleton{ nullptr };

int main()
{
	CSingleton& first{ CSingleton::getSingleton() };
	CSingleton& second{ CSingleton::getSingleton() };
	CSingleton& third{ CSingleton::getSingleton() };
	//CSingleton fourth(third); would generate an error, as the copy constructor is deleted
	//CSingleton fifth(std::move(third)); would generate an error, as the move constructor is deleted
	first.deleteSingleton();
	CSingleton::deleteSingleton();
}
