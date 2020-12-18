#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
private:
	static T _instance;

public:
	static T getInstance();

	static void setInstance(T instance);
};

#endif
