#pragma once

#include <string_view>

namespace LCN
{
	template<typename _Type>
	struct NameOfType
	{
		static constexpr std::string_view Value()
		{
			std::string_view name{ __FUNCSIG__ };

			size_t begin = name.find("NameOfType") + 11;
			size_t end = name.find(">::", begin);

			return { name.data() + begin, end - begin };
		}
	};

	template<typename _Type>
	constexpr std::string_view NameOf = NameOfType<_Type>::Value();
}