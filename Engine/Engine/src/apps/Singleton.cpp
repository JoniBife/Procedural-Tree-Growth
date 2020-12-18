#include "Singleton.h"

template <typename T>
T Singleton<T>::getInstance() {
	return _instance;
}

template <typename T>
void Singleton<T>::setInstance(T instance) {
	_instance = instance;
}