#pragma once

#include <string_view>
#include <type_traits>

#include "Utilities/Source/TypeList.h"

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

		template<typename _Integral>
		static constexpr
		std::enable_if_t<std::is_unsigned_v<_Integral>, char>
		_Value(_Integral _I, size_t Idx)
		{
			size_t idx = Size() - Idx - 1;
			size_t div = 1;

			for (size_t i = 0; i < idx; ++i)
				div *= 10;

			return (_I / div) % 10 + '0';
		}

		template<typename _Integral>
		static constexpr
		std::enable_if_t<std::is_signed_v<_Integral>, char>
		_Value(_Integral _I, size_t Idx)
		{
			using _UIntegral = std::make_unsigned_t<_Integral>;

			if (Idx == 0 && _I < 0)
				return '-';

			_UIntegral _J = (_I < 0 ? -_I : _I);

			return _Value(_J, Idx);
		}

		static constexpr char Value(size_t Idx)
		{
			return _Value(I, Idx);
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

	////////////////////
	//-- StringView --//
	////////////////////

	template<typename _Derived>
	struct StringView
	{
		static constexpr std::string_view GetView() { return _Derived::GetView(); }

		static constexpr size_t Size() { return GetView().size(); }

		static constexpr char Value(size_t Idx) { return GetView()[Idx]; }
	};

	/////////////////////
	//-- JoinLiteral --//
	/////////////////////

	template<typename _Separator, typename _GeneratorList>
	struct JoinLiteral;

	template<typename _Separator, typename _HeadGen, typename ... _TailGen>
	struct JoinLiteral<_Separator, TypeList<_HeadGen, _TailGen...>>
	{
		using TypeListType   = TypeList<_HeadGen, _TailGen...>;
		using ConcatType     = ConcatLiterals<_HeadGen, _TailGen...>;
		using SubJoinLiteral = JoinLiteral<_Separator, typename TypeListType::Tail>;

		static constexpr size_t Size() { return ConcatType::Size() + _Separator::Size() * (TypeListType::Count() - 1); }

		static constexpr char Value(size_t Idx)
		{
			if (Idx < _HeadGen::Size())
				return _HeadGen::Value(Idx);

			Idx -= _HeadGen::Size();

			if (Idx < _Separator::Size())
				return _Separator::Value(Idx);

			Idx -= _Separator::Size();

			return SubJoinLiteral::Value(Idx);
		}
	};

	template<typename _Separator, typename _SingleType>
	struct JoinLiteral<_Separator, TypeList<_SingleType>>
	{
		static constexpr size_t Size() { return _SingleType::Size(); }

		static constexpr char Value(size_t Idx) { return _SingleType::Value(Idx); }
	};
}