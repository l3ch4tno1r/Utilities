#pragma once

namespace LCN
{
	template<std::integral _Int>
	class Bitwise
	{
	public:
		Bitwise(_Int& target)
			: m_target{ target }
		{}

	public:
		class BitRef
		{
		public:
			void
			operator=(
				const bool value) const
			{
				static_assert(not std::is_const_v<_Int>, "Cannot edit a const value.");

				_Int mask = 1 << m_idx;

				m_target = (m_target & ~mask) | (value << m_idx);
			}

			operator const bool () const
			{
				return (1 << m_idx) & m_target;
			}
		
		private:
			_Int& m_target;
			const std::size_t m_idx;
		
		private:
			BitRef(_Int& target, const std::size_t idx)
				: m_target{ target }
				, m_idx{ idx }
			{}

			friend Bitwise;
		};

		BitRef
		operator[](
			const size_t idx) const
		{
			return { m_target, idx };
		}

	private:
		_Int& m_target;
	};
}