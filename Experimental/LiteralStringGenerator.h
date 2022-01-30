#pragma once

#include <string_view>
#include <type_traits>

namespace LCN
{
	/////////////////
	//-- Helpers --//
	/////////////////

	using ConstCharRef = const char(&)[];

	template<typename _Generator, size_t Size, size_t Idx = 0, char ... c>
	struct LiteralHelper : LiteralHelper<_Generator, Size, Idx + 1, c..., _Generator::Value(Idx)>
	{
		using BaseType = LiteralHelper<_Generator, Size, Idx + 1, c..., _Generator::Value(Idx)>;
	};

	template<typename _Generator, size_t Size, char ... c>
	struct LiteralHelper<_Generator, Size, Size, c...>
	{
		static constexpr char Literal[Size] = { c... };

		static constexpr std::string_view View()
		{
			return { Literal, Size };
		}
	};

	template<typename _Generator>
	constexpr std::string_view Literal = LiteralHelper<_Generator, _Generator::Size()>::View();

	///////////////////////////
	//-- RepeatCharLiteral --//
	///////////////////////////

	template<char c, size_t N>
	struct RepeatCharLiteral
	{
		static constexpr size_t Size() { return N; }

		static constexpr char Value(size_t) { return c; }
	};

	///////////////////////////
	//-- ToLower / ToUpper --//
	///////////////////////////

	template<typename _Generator>
	struct ToLower
	{
		static constexpr size_t Size() { return _Generator::Size(); }

		static constexpr char Value(size_t Idx)
		{
			char c = _Generator::Value(Idx);

			return 'A' <= c && c <= 'Z' ? c - 'A' + 'a' : c;
		}
	};

	template<typename _Generator>
	struct ToUpper
	{
		static constexpr size_t Size() { return _Generator::Size(); }

		static constexpr char Value(size_t Idx)
		{
			char c = _Generator::Value(Idx);

			return 'a' <= c && c <= 'z' ? c - 'a' + 'A' : c;
		}
	};

	////////////////////////////////////
	//-- Integral to string literal --//
	////////////////////////////////////

	template<typename _Type, _Type, typename _Enable = void>
	struct ToString;

	template<typename _Integral, _Integral I>
	struct ToString<_Integral, I, std::enable_if_t<std::is_integral_v<_Integral>>>
	{
		static constexpr size_t Size()
		{
			if (I == 0)
				return 1;

			_Integral result = 0;

			for (_Integral i = I; i != 0; i /= 10)
				++result;

			return result + (I < 0);
		}

		static constexpr char Value(size_t Idx)
		{
			size_t idx = Size() - Idx - 1;
			size_t div = 1;

			for (size_t i = 0; i < idx; ++i)
				div *= 10;

			return (I / div) % 10 + '0';
		}
	};

	////////////////////////
	//-- ConcatLiterals --//
	////////////////////////

	template<typename _GenHead, typename ... _GenTail>
	struct ConcatLiterals
	{
		using Head = _GenHead;
		using Tail = ConcatLiterals<_GenTail...>;

		static constexpr size_t Size() { return Head::Size() + Tail::Size(); }

		static constexpr char Value(size_t Idx)
		{
			constexpr size_t size = Head::Size();

			if (Idx < size)
				return Head::Value(Idx);

			return Tail::Value(Idx - size);
		}
	};

	template<typename _Generator>
	struct ConcatLiterals<_Generator>
	{
		static constexpr size_t Size() { return _Generator::Size(); }

		static constexpr char Value(size_t Idx) { return _Generator::Value(Idx); }
	};
}