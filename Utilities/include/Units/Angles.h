#pragma once

#include <ratio>

#include "Details/Units.h"

namespace LCN::Units::Angle
{
	class Angle;

	using DegToRadRatio = std::ratio<18000000000, 314159265>;

	template<typename _Type, std::enable_if_t<std::is_floating_point_v<_Type>, bool> = true>
	using Radian = Unit<Angle, _Type, DegToRadRatio>;

	template<typename _Type, std::enable_if_t<std::is_floating_point_v<_Type>, bool> = true>
	using Round = Unit<Angle, _Type, std::ratio<360, 1>>;

	template<typename _Type> using Degree    = Unit<Angle, _Type, std::ratio<1>>;
	template<typename _Type> using ArcMinute = Unit<Angle, _Type, std::ratio<1, 60>>;
	template<typename _Type> using ArcSecond = Unit<Angle, _Type, std::ratio<1, 3600>>;

	using RadianLD    = Radian<long double>;
	using RoundLD     = Round<long double>;
	using DegreeLD    = Degree<long double>;
	using ArcMinuteLD = ArcMinute<long double>;
	using ArcSecondLD = ArcSecond<long double>;
	
	using DegreeLL    = Degree<long long>;
	using ArcMinuteLL = ArcMinute<long long>;
	using ArcSecondLL = ArcSecond<long long>;

	namespace Literals
	{
		constexpr RadianLD operator"" _rad(long double value) noexcept { return RadianLD{ value }; }

		constexpr RoundLD operator"" _rnd(long double value) noexcept { return RoundLD{ value }; }

		constexpr DegreeLD operator"" _deg(long double value) noexcept { return DegreeLD{ value }; }

		constexpr ArcMinuteLD operator"" _amin(long double value) noexcept { return ArcMinuteLD{ value }; }

		constexpr ArcSecondLD operator"" _asec(long double value) noexcept { return ArcSecondLD{ value }; }


		constexpr DegreeLL operator"" _deg(unsigned long long value) noexcept { return DegreeLL{ value }; }

		constexpr ArcMinuteLL operator"" _amin(unsigned long long value) noexcept { return ArcMinuteLL{ value }; }

		constexpr ArcSecondLL operator"" _asec(unsigned long long value) noexcept { return ArcSecondLL{ value }; }
	}
}

namespace LCN
{
	namespace UnitsAngle = Units::Angle;
}

/*
namespace std
{
	template<
		typename _Type,
		typename _Ratio>
	constexpr
	long double
	cos(
		const LCN::Units::Unit<LCN::UnitsAngle::Angle, _Type, _Ratio> angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::UnitsAngle::Angle, long double, _Ratio>;

		return std::cos(LCN::UnitsAngle::RadianLD(UnitLD(angle)).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
	constexpr
	long double
	sin(
		const LCN::Units::Unit<LCN::UnitsAngle::Angle, _Type, _Ratio> angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::UnitsAngle::Angle, long double, _Ratio>;

		return std::sin(LCN::UnitsAngle::RadianLD(UnitLD(angle)).Count());
	}

	template<
		typename _Type,
		typename _Ratio>
	constexpr
	long double
	tan(
		const LCN::Units::Unit<LCN::UnitsAngle::Angle, _Type, _Ratio> angle)
	{
		using UnitLD = LCN::Units::Unit<LCN::UnitsAngle::Angle, long double, _Ratio>;

		return std::tan(LCN::UnitsAngle::RadianLD(UnitLD(angle)).Count());
	}
}
*/