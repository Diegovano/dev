#pragma once

template <class _SType>
class stack {
private:
	_SType* arr;
	_SType* lastElem;
	size_t occSize;
public:
	stack() {
		if (sizeof(_SType) > 0xD000) {
			std::cerr << "The size of your type is too large to make this container have a use.";
			system("pause");
			return;
		}
		arr = new _SType[0xFA000 / sizeof(_SType)];
		lastElem = arr;
	}
	bool empty();
	size_t size();
	void pushBack(_SType ent);
	void popBack();
	const _SType operator[](const size_t ent);
};
