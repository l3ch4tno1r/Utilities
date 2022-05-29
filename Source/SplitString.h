#pragma once

#include <string_view>
#include <iterator>

namespace LCN
{
	//////////////////////////
	//-- BasicSplitResult --//
	//////////////////////////

	template<typename _CharType, typename _Traits = std::char_traits<_CharType>>
	class BasicSplitResult
	{
	public:
		using StringType     = std::basic_string<_CharType, _Traits>;
		using StringViewType = std::basic_string_view<_CharType, _Traits>;

		class Iterator;

		Iterator begin() const { return { *this }; }
		Iterator end()   const { return { *this, true }; }

		size_t Count() const
		{
			Iterator it = this->begin(), end = this->end();
			size_t count{ 0 };

			for (; it != end; ++it, ++count);

			return count;
		}

		template<typename _CharType, typename _Traits, typename _StrType>
		friend BasicSplitResult<_CharType, _Traits> Split(const _CharType*, _StrType&&);

		template<typename _CharType, typename _Traits, typename _StrType>
		friend BasicSplitResult<_CharType, _Traits> Split(const std::basic_string<_CharType, _Traits>&, _StrType&&);

		template<typename _CharType, typename _Traits, typename _StrType>
		friend BasicSplitResult<_CharType, _Traits> Split(const std::basic_string_view<_CharType, _Traits>, _StrType&&);

	private:
		template<typename _StrArgType1, typename _StrArgType2>
		BasicSplitResult(_StrArgType1&& target, _StrArgType2&& delimiter) :
			m_Target{ std::forward<_StrArgType1>(target) },
			m_Delimiter{ std::forward<_StrArgType2>(delimiter) }
		{}

	private:
		StringViewType m_Target;
		StringViewType m_Delimiter;
	};

	//////////////////
	//-- Iterator --//
	//////////////////

	template<typename _CharType, typename _Traits>
	class BasicSplitResult<_CharType, _Traits>::Iterator
	{
	public:
		using SplitResultType = BasicSplitResult<_CharType, _Traits>;
		using StringType      = typename SplitResultType::StringType;
		using StringViewType  = typename SplitResultType::StringViewType;

		friend SplitResultType;
		
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type        = StringViewType;
		using difference_type   = int;
		using pointer           = void;
		using reference         = void;

	public:
		friend bool operator==(
			const Iterator& it1,
			const Iterator& it2)
		{
			return &it1.m_SplitResult == &it2.m_SplitResult && it1.m_Start == it2.m_Start;
		}

		friend bool operator!=(
			const Iterator& it1,
			const Iterator& it2)
		{
			return !(it1 == it2);
		}

		Iterator& operator++()
		{
			const StringViewType& target{ m_SplitResult.m_Target };
			const StringViewType& delimiter{ m_SplitResult.m_Delimiter };

			m_Start = std::min(m_End + delimiter.size(), target.size());
			m_End   = std::min(target.find(delimiter, m_Start), target.size());

			return *this;
		}

		Iterator operator++(int)
		{
			Iterator result{ *this };

			++(*this);

			return result;
		}

		StringViewType operator*() const
		{
			return { m_SplitResult.m_Target.data() + m_Start, m_End - m_Start };
		}

		Iterator(const Iterator& other) :
			m_SplitResult{ other.m_SplitResult },
			m_Start{ other.m_Start },
			m_End{ other.m_End }
		{}

	private:
		Iterator(const SplitResultType& splitResult) :
			m_SplitResult(splitResult),
			m_End(std::min(
				splitResult.m_Target.find(splitResult.m_Delimiter),
				splitResult.m_Target.size()))
		{}

		Iterator(const SplitResultType& splitResult, bool) :
			m_SplitResult{ splitResult },
			m_Start{ splitResult.m_Target.size() }
		{}

	private:
		const SplitResultType& m_SplitResult;

		size_t m_Start{ 0 }, m_End{ StringType::npos };
	};

	/////////////////////
	//-- Split proxy --//
	/////////////////////

	template<typename _CharType, typename _Traits = std::char_traits<_CharType>, typename _StrType>
	BasicSplitResult<_CharType, _Traits> Split(
		const _CharType* ptr,
		_StrType&& delimiter)
	{
		return { ptr, std::forward<_StrType>(delimiter) };
	}

	template<typename _CharType, typename _Traits, typename _StrType>
	BasicSplitResult<_CharType, _Traits> Split(
		const std::basic_string<_CharType, _Traits>& str,
		_StrType&& delimiter)
	{
		return { str, std::forward<_StrType>(delimiter) };
	}

	template<typename _CharType, typename _Traits, typename _StrType>
	BasicSplitResult<_CharType, _Traits> Split(
		const std::basic_string_view<_CharType, _Traits> strv,
		_StrType&& delimiter)
	{
		return { strv, std::forward<_StrType>(delimiter) };
	}


	///////////////////////////
	//-- Convenience using --//
	///////////////////////////

	using SplitStringResult    = BasicSplitResult<char>;
	using SplitWStringResult   = BasicSplitResult<wchar_t>;
	using Splitu8StringResult  = BasicSplitResult<char8_t>;
	using Splitu16StringResult = BasicSplitResult<char16_t>;
	using Splitu32StringResult = BasicSplitResult<char32_t>;
}