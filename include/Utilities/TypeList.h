#pragma once

namespace LCN
{
	//////////////////
	//-- TypeList --//
	//////////////////

	template<typename _Head, typename ... _Tail>
	struct TypeList
	{
		using Head = _Head;
		using Tail = TypeList<_Tail...>;

		static constexpr size_t Count() { return Tail::Count() + 1; }
	};

	template<typename Single>
	struct TypeList<Single>
	{
		using Head = Single;
		using Tail = void;

		static constexpr size_t Count() { return 1; }
	};

	/////////////////
	//-- Extract --//
	/////////////////

	template<size_t Idx, typename typelist>
	struct Extract
	{
		using Result = typename Extract<Idx - 1, typename typelist::Tail>::Result;
	};

	template<typename typelist>
	struct Extract<0, typelist>
	{
		using Result = typename typelist::Head;
	};
}