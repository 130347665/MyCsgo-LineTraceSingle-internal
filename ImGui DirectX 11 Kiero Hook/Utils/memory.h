#pragma once


namespace Memory
{
	template<class T>
	T Read(uintptr_t ptr)
	{
		T buff{};
		if ((ptr > 0x1000000 - 0x8) && (ptr < 0x7FFFFFF00000 + 0x8)) {
			if (IsBadReadPtr((void*)ptr, sizeof(T)) == 0) {

				buff = *reinterpret_cast<T*>(ptr);
			}
		}
		return buff;
	}
	template<typename type>
	bool Write(uintptr_t address, const type& value)
	{
		if (reinterpret_cast<type*>(address) != nullptr)
		{
			*reinterpret_cast<type*>(address) = value;
			return true;
		}

		return false;
	}
};