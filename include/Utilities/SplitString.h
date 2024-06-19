#pragma once

#include <string_view>
#include <ranges>

namespace LCN
{
    /////////////////////
    //-- Declaration --//
    /////////////////////

    template<typename _StringViewType>
    class BasicSplitResult;

    template<
        typename _CharType,
        typename _CharTraits>
    class BasicSplitResult<std::basic_string_view<_CharType, _CharTraits>> : public std::ranges::view_base
    {
    public:
        using StringViewType = std::basic_string_view<_CharType, _CharTraits>;

    public:
        BasicSplitResult(const StringViewType view, const StringViewType delim)
            : m_view{ view }
            , m_delimiter{ delim }
        {}

    public:

        class Iterator
        {
        public:
            using StringViewType  = std::basic_string_view<_CharType, _CharTraits>;
            using difference_type = std::ptrdiff_t;
            using value_type      = StringViewType;
        
        public:
            Iterator() = default;

            Iterator(const Iterator& other) = default;

            Iterator(const StringViewType view, const StringViewType delim)
                : m_view{ view }
                , m_delimiter{ delim }
                , m_offset{ 0 }
                , m_size{ view.find(delim) }
            {}

        public:
            Iterator& operator++()
            {
                m_offset += m_size + m_delimiter.size();

                if(m_offset > m_view.size())
                    return *this = Iterator{};            

                auto substring = m_view.substr(m_offset);

                m_size = std::min(
                    substring.size(),
                    substring.find(m_delimiter));

                return *this;
            }

            Iterator operator++(int)
            {
                Iterator result{ *this };

                ++(*this);

                return result;
            }

            const bool operator==(const Iterator& other) const
            {
                return
                    m_offset == other.m_offset &&
                    m_size   == other.m_size;
            }

            const StringViewType operator*() const { return m_view.substr(m_offset, m_size); }
        
        private:
            StringViewType m_view;
            StringViewType m_delimiter;

            size_t m_offset{ StringViewType::npos }, m_size{ StringViewType::npos };
        };

        using iterator = Iterator;

        const size_t Count() const
        {
            return std::distance(this->begin(), this->end());
        }

        Iterator begin() const
        {
            return { m_view, m_delimiter };
        }

        Iterator end() const
        {
            return {};
        }

    private:
        StringViewType m_view;
        StringViewType m_delimiter;
    };

    /////////////////////
    //-- Convenience --//
    /////////////////////

    using SplitResult  = BasicSplitResult<std::string_view>;
    using WSplitResult = BasicSplitResult<std::wstring_view>;

    SplitResult
    Split(
        const std::string_view view,
        const std::string_view delim)
    {
        std::basic_string_view sv{ "" };
        return { view, delim };
    }

    WSplitResult
    Split(
        const std::wstring_view view,
        const std::wstring_view delim)
    {
        return { view, delim };
    }
}