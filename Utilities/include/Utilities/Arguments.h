#pragma once

#include <string_view>
#include <span>
#include <ranges>
#include <charconv>

namespace LCN::Utilities
{
    #define _CharType char

    namespace Details
    {
        inline
        std::basic_string_view<_CharType>
        ConstCharPtrToStringView(
            const _CharType* raw)
        {
            return { raw };
        }

        inline
        auto
        CreateArgRange(
            const int argc,
            const _CharType** argv)
        {
            return std::span{ argv, (size_t)argc }
                | std::ranges::views::transform(ConstCharPtrToStringView);
        }

        using ArgRangeType = decltype(CreateArgRange(0, nullptr));
    }

    //////////////////////////////////
    //-- Argument value extractor --//
    //////////////////////////////////

    class ArgValue
    {
    public:
        using CharType       = _CharType;
        using StringViewType = std::basic_string_view<_CharType>;

        ArgValue() = default;

        ArgValue(const StringViewType argv)
            : m_Argv{ argv }
        {}

        operator const StringViewType() const
        {
            return m_Argv;
        }

        template<typename _StrType>
        requires std::same_as<CharType, typename _StrType::value_type>
        void
        operator>>(
            _StrType& str)
        {
            str = m_Argv;
        }

        template<typename _Arithmetic>
        requires std::is_arithmetic_v<_Arithmetic>
        [[nodiscard]]
        std::errc
        operator>>(
            _Arithmetic& value)
        {
            return std::from_chars(
                m_Argv.begin(),
                m_Argv.end(),
                value).ec;
        }

    private:
        const StringViewType m_Argv;
    };

    ///////////////////
    //-- Arguments --//
    ///////////////////

    struct Arguments : Details::ArgRangeType
    {
        using CharType       = _CharType;
        using StringViewType = std::basic_string_view<_CharType>;

        Arguments(const int argc, CharType** argv)
            : Details::ArgRangeType{ Details::CreateArgRange(argc, const_cast<const CharType**>(argv)) }
        {}

        Arguments(const int argc, const CharType** argv)
            : Details::ArgRangeType{ Details::CreateArgRange(argc, argv) }
        {}

        const StringViewType
        operator[](
            const std::size_t idx) const
        {
            return Details::ArgRangeType::operator[](idx);
        }

        ArgValue
        operator[](
            const StringViewType key) const
        {
            auto it = std::ranges::find(*this, key);

            if(this->end() == it)
                return {};
            
            return ++it == this->end()
                ? ArgValue{}
                : ArgValue{ *it };
        }
    };
}