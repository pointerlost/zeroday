#pragma once
#include <iostream>
#include <typeinfo>
#include <type_traits>

namespace DEBUG
{

	template<typename T>
	inline void DebugForEngineObjectPointers(const T& obj)
	{
		using Type = std::decay_t<T>;

		if constexpr (std::is_pointer_v<Type>) // raw pointer debugging
		{
			if (obj == nullptr)
			{
				std::cerr << "Raw pointer is nullptr. Type: "
					<< typeid(std::remove_pointer_t<Type>).name() << "*\n";
			}
		}
		else if constexpr (requires {obj.get(); }) // smart pointer debugging
		{
			if (obj.get() == nullptr)
			{
				std::cerr << "Smart pointer is nullptr. Type: "
					<< typeid(typename Type::element_type).name() << "*\n";
			}
		}
		else
		{
			std::cout << "Non-pointer type passed to DebugForEngineObjectPointers. Type: "
				<< typeid(Type).name() << "\n";
		}
	}
}