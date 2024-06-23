#pragma once

#include <type_traits>
#include <numeric>

namespace LCN::Units
{
	/////////////////////////////
	//-- Forward declaration --//
	/////////////////////////////

	template<typename _To, typename _From>
	constexpr _To UnitCast(const _From&);

	////////////////////
	//-- class Unit --//
	////////////////////

	template<
		typename _MeasurementType,
		typename _Type,
		typename _Ratio,
		std::enable_if_t<std::is_arithmetic_v<_Type>, bool> = true>
	class Unit
	{
	public:
		using Type      = _MeasurementType;
		using ValueType = _Type;
		using RatioType = _Ratio;

	public:
		constexpr Unit() = default;

		template<typename _Type2>
		constexpr Unit(const _Type2& value) :
			m_Measure{ static_cast<_Type>(value) }
		{}

		template<typename _Type2, typename _Ratio2>
		constexpr Unit(const Unit<_MeasurementType, _Type2, _Ratio2>& other) :
			m_Measure{ UnitCast<Unit>(other).Count() }
		{}

		constexpr _Type Count() const { return m_Measure; }

		constexpr Unit operator+() const { return Unit(*this); }

		constexpr Unit operator-() const { return Unit(-m_Measure); }

		constexpr Unit& operator++()
		{
			++m_Measure;
			return *this;
		}

		constexpr Unit operator++(int) { return Unit(m_Measure++); }

		constexpr Unit& operator--()
		{
			--m_Measure;
			return *this;
		}

		constexpr Unit operator--(int) { return Unit(m_Measure--); }

		constexpr Unit& operator+=(const Unit& other)
		{
			m_Measure += other.m_Measure;
			return *this;
		}

		constexpr Unit& operator-=(const Unit& other)
		{
			m_Measure -= other.m_Measure;
			return *this;
		}

		constexpr Unit& operator*=(const ValueType& factor)
		{
			m_Measure *= factor;
			return *this;
		}

		constexpr Unit& operator/=(const ValueType& factor)
		{
			m_Measure /= factor;
			return *this;
		}

	private:
		_Type m_Measure{ _Type(0) };
	};

#pragma region Utilities

	///////////////////
	//-- Utilities --//
	///////////////////

	template<typename, typename>
	struct CommonType;

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2,
		typename _Ratio2>
	struct CommonType<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	{
		using Type = LCN::Units::Unit<
			_MeasurementType,
			std::common_type_t<_Type1, _Type2>,
			std::ratio<
				std::gcd(_Ratio1::num, _Ratio2::num),
				std::lcm(_Ratio1::den, _Ratio2::den)>>;
	};

	template<
		typename _To,
		typename _MeasurementType,
		typename _Type,
		typename _Ratio2>
	constexpr
	std::enable_if_t<std::is_same_v<_MeasurementType, typename _To::Type>, _To>
	UnitCast(
		const Unit<_MeasurementType, _Type, _Ratio2>& unit)
	{
		using ToValueType = typename _To::ValueType;
		
		using FromRatio = _Ratio2;
		using ToRatio   = typename _To::RatioType;

		using NewRatio = std::ratio_divide<FromRatio, ToRatio>;

		return _To{ static_cast<ToValueType>(NewRatio::num * unit.Count()) / NewRatio::den };
	}

#pragma endregion

#pragma region Arithmetic operators

	//////////////////////////////
	//-- Arithmetic operators --//
	//////////////////////////////

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	auto
	operator+(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		using ReturnType = typename CommonType<
			Unit<_MeasurementType, _Type1, _Ratio1>,
			Unit<_MeasurementType, _Type2, _Ratio2>>::Type;

		return ReturnType{ ReturnType{ left }.Count() + ReturnType{ right }.Count() };
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	std::common_type_t<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	operator-(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		using ReturnType = typename std::common_type<Unit<_MeasurementType, _Type1, _Ratio1>, Unit<_MeasurementType, _Type2, _Ratio2>>::type;

		return ReturnType(ReturnType(left).Count() - ReturnType(right).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Type2,
		typename _Ratio2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio2>
	operator*(
		const _Type1& factor,
		const Unit<_MeasurementType, _Type2, _Ratio2>& measure)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio2>;

		return ReturnType(factor * ReturnType(measure).Count());
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	inline
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator*(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		return factor * measure;
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator/(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& factor)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() / factor);
	}

	template<
		typename _MeasurementType,
		typename _Type1,
		typename _Ratio1,
		typename _Type2>
	constexpr
	Unit<_MeasurementType, std::common_type_t<_Type1, _Type2>, _Ratio1>
	operator%(
		const Unit<_MeasurementType, _Type1, _Ratio1>& measure,
		const _Type2& divider)
	{
		using CommonType = std::common_type_t<_Type1, _Type2>;
		using ReturnType = Unit<_MeasurementType, CommonType, _Ratio1>;

		return ReturnType(ReturnType(measure).Count() % divider);
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	std::common_type_t<
		Unit<_MeasurementType, _Type1, _Ratio1>,
		Unit<_MeasurementType, _Type2, _Ratio2>>
	operator%(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		using ReturnType = std::common_type_t<
			Unit<_MeasurementType, _Type1, _Ratio1>,
			Unit<_MeasurementType, _Type2, _Ratio2>>;

		return ReturnType(ReturnType(left).Count() % ReturnType(right).Count());
	}

#pragma endregion

#pragma region Comparison operators

	//////////////////////////////
	//-- Comparison operators --//
	//////////////////////////////

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator==(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		using _CommonType = typename CommonType<
			Unit<_MeasurementType, _Type1, _Ratio1>,
			Unit<_MeasurementType, _Type2, _Ratio2>>::Type;

		return _CommonType{ left }.Count() == _CommonType{ right }.Count();
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator!=(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		return !(left == right);
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator<(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		using CommonType = std::common_type_t<
			Unit<_MeasurementType, _Type1, _Ratio1>,
			Unit<_MeasurementType, _Type2, _Ratio2>>;

		return CommonType(left).Count() < CommonType(right).Count();
	}	

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator<=(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		return !(right < left);
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator>(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		return right < left;
	}

	template<
		typename _MeasurementType,
		typename _Type1, typename _Ratio1,
		typename _Type2, typename _Ratio2>
	constexpr
	bool
	operator>=(
		const Unit<_MeasurementType, _Type1, _Ratio1>& left,
		const Unit<_MeasurementType, _Type2, _Ratio2>& right)
	{
		return !(left < right);
	}

#pragma endregion
}