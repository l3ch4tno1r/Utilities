#pragma once

namespace LCN
{
	template<std::integral>
	class BitRef;

	template<std::integral _Int>
	class BitwiseEditor
	{
	public:
		using ValType = _Int;
		using RefType = ValType&;

		using BitRefType      = BitRef<std::remove_const_t<_Int>>;
		using ConstBitRefType = BitRef<const std::remove_const_t<_Int>>;

		constexpr
		BitwiseEditor(RefType target)
			: m_Target{ target }
		{}

		BitRefType operator[](uint32_t);

	private:
		RefType m_Target;
	};

	template<std::integral _Int>
	class BitRef
	{
	public:
		using ValType = _Int;
		using RefType = ValType&;

	private:
		constexpr
		BitRef(RefType target, const uint32_t idx)
			: m_Target{ target }
			, m_Idx{ idx }
		{}

		template<std::integral _Int2>
		friend BitwiseEditor<_Int2>;

	private:
		RefType m_Target;
		const uint32_t m_Idx;
	};

	template<std::integral _Int>
	typename BitwiseEditor<_Int>::BitRefType
	BitwiseEditor<_Int>::operator[](
		uint32_t idx)
	{
		return { this->m_Target, idx };
	}
}