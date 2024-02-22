#pragma once

#include <array>
#include <vector>
#include <ostream>
#include <cassert>
#if !defined(__cpp_impl_three_way_comparison) || __cpp_impl_three_way_comparison < 201907L
#include "compare.hpp"
#define ORD GP::impl
#else
#define ORD std
#endif

namespace happy_cube {

class Side: protected std::array<bool, 5> {
private:
	typedef std::array<bool, 5> base_type;

public:
	using base_type::base_type;
	using base_type::front;
	using base_type::back;
	using base_type::operator[];

	Side(base_type&&) noexcept;
	Side& operator=(base_type&&) noexcept;

	Side flip() const noexcept;

	bool valid() const noexcept;

	bool match(const Side&) const noexcept;

	static bool corner(bool, bool, bool) noexcept;

	bool operator<(const Side&) const noexcept;
	bool operator>(const Side&) const noexcept;
	bool operator<=(const Side&) const noexcept;
	bool operator>=(const Side&) const noexcept;
	bool operator==(const Side&) const noexcept;
	bool operator!=(const Side&) const noexcept;
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	std::partial_ordering operator<=>(const Side&) const noexcept;
#else
	ORD::partial_ordering cmp(const Side&) const noexcept;
#endif
	
friend std::ostream& operator<<(std::ostream&, const Side&);
};

class BrickB {
protected:
	Side top_, right_, bottom_, left_;

public:
	BrickB(const std::vector<unsigned int>&);

	BrickB t(unsigned int) const;

	const Side& top() const noexcept;
	const Side& right() const noexcept;
	const Side& bottom() const noexcept;
	const Side& left() const noexcept;

	bool operator<(const BrickB&) const noexcept;
	bool operator>(const BrickB&) const noexcept;
	bool operator<=(const BrickB&) const noexcept;
	bool operator>=(const BrickB&) const noexcept;
	bool operator==(const BrickB&) const noexcept;
	bool operator!=(const BrickB&) const noexcept;
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	std::partial_ordering operator<=>(const BrickB&) const noexcept;
#else
	ORD::partial_ordering cmp(const BrickB&) const noexcept;
#endif

	bool valid() const noexcept;

protected:
	template<typename S1, typename S2, typename S3, typename S4>
	BrickB(S1&&, S2&&, S3&&, S4&&);
};

class Brick: public BrickB {
private:
	typedef BrickB base_type;

protected:
	bool flipping;
	std::vector<BrickB> variants_;

public:
	Brick(const std::vector<unsigned int>&);
	Brick(const BrickB&);
	Brick(BrickB&&);

	const BrickB& brick(unsigned int) const noexcept;
	bool worth_flipping() const noexcept;

	bool operator<(const Brick&) const noexcept;
	bool operator>(const Brick&) const noexcept;
	bool operator<=(const Brick&) const noexcept;
	bool operator>=(const Brick&) const noexcept;
	bool operator==(const Brick&) const noexcept;
	bool operator!=(const Brick&) const noexcept;
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	std::partial_ordering operator<=>(const Brick&) const noexcept;
#else
	ORD::partial_ordering cmp(const Brick&) const noexcept;
#endif

	unsigned int degree() const noexcept;

private:
	std::vector<BrickB> variants(bool&) const;
};

inline Side&
Side::operator=(base_type&& v) noexcept {
	base_type::operator=(std::move(v));
	return *this;
}

inline
Side::Side(base_type&& v) noexcept
	: base_type(std::move(v))
{
}

inline bool
Side::match(const Side& other) const noexcept {
	return (operator[](1) ^ other[1]) &&
		(operator[](2) ^ other[2]) &&
		(operator[](3) ^ other[3]) &&
		!(operator[](0) && other[0]) &&
		!(operator[](4) && other[4]);
}

inline bool
Side::corner(bool a, bool b, bool c) noexcept {
	return (!a && (b ^ c)) || (a && !b && !c);
}

#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
inline std::partial_ordering
Side::operator<=>(const Side& other) const noexcept {
	return static_cast<const base_type&>(*this) <=>
		static_cast<const base_type&>(other);
}
#else
inline ORD::partial_ordering
Side::cmp(const Side& other) const noexcept {
	const_iterator i = begin(), __li = end(),
		j = other.begin(), __lj = other.end();
	for (; __li != i && __lj != j; ++i, ++j) {
		if (*i < *j)
			return ORD::partial_ordering::less;
		if (*i > *j)
			return ORD::partial_ordering::greater;
	}
	if (__li != i)
		return ORD::partial_ordering::greater;
	return __lj != j ? ORD::partial_ordering::less : ORD::partial_ordering::equivalent;
	return ORD::partial_ordering::equivalent;
}
#endif

inline bool
Side::operator<(const Side& other) const noexcept {
	return static_cast<const base_type&>(*this) <
		static_cast<const base_type&>(other);
}

inline bool
Side::operator>(const Side& other) const noexcept {
	return other < *this;
}

inline bool
Side::operator<=(const Side& other) const noexcept {
	return !(other < *this);
}

inline bool
Side::operator>=(const Side& other) const noexcept {
	return !(*this < other);
}

inline bool
Side::operator==(const Side& other) const noexcept {
	return static_cast<const base_type&>(*this) ==
		static_cast<const base_type&>(other);
}

inline bool
Side::operator!=(const Side& other) const noexcept {
	return !(*this == other);
}

inline const Side&
BrickB::top() const noexcept {
	return top_;
}

inline const Side&
BrickB::right() const noexcept {
	return right_;
}

inline const Side&
BrickB::bottom() const noexcept {
	return bottom_;
}

inline const Side&
BrickB::left() const noexcept {
	return left_;
}

inline unsigned int
Brick::degree() const noexcept {
	return variants_.size() + 1;
}

inline BrickB
BrickB::t(unsigned int n) const {
	switch (n) {
	case 7: // flip on the main diagonal
		return BrickB(left().flip(), bottom().flip(),
			      right().flip(), top().flip());
	case 6: // flip vertically
		return BrickB(bottom().flip(), right().flip(),
			      top().flip(), left().flip());
	case 5: // flip on the anti diagonal
		return BrickB(right().flip(), top().flip(),
			      left().flip(), bottom().flip());
	case 4: // flip horizontally
		return BrickB(top().flip(), left().flip(),
			      bottom().flip(), right().flip());
	case 3: // rotate 90° counter-clockwise
		return BrickB(right(), bottom(), left(), top());
	case 2: // rotate 180°
		return BrickB(bottom(), left(), top(), right());
	case 1: // rotate 90° clockwise
		return BrickB(left(), top(), right(), bottom());
	case 0:
	default:
		return *this;
	}
}

#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
inline std::partial_ordering
BrickB::operator<=>(const BrickB& other) const noexcept {
	auto c = top() <=> other.top();
	if (c < 0)
		return std::partial_ordering::less;
	if (c > 0)
		return std::partial_ordering::greater;

	c = right() <=> other.right();
	if (c < 0)
		return std::partial_ordering::less;
	if (c > 0)
		return std::partial_ordering::greater;

	c = bottom() <=> other.bottom();
	if (c < 0)
		return std::partial_ordering::less;
	if (c > 0)
		return std::partial_ordering::greater;

	c = left() <=> other.left();
	if (c < 0)
		return std::partial_ordering::less;
	if (c > 0)
		return std::partial_ordering::greater;

	return std::partial_ordering::equivalent;
}
#else
inline ORD::partial_ordering
BrickB::cmp(const BrickB& other) const noexcept {
	auto c = top().cmp(other.top());
	if (c < 0)
		return ORD::partial_ordering::less;
	if (c > 0)
		return ORD::partial_ordering::greater;

	c = right().cmp(other.right());
	if (c < 0)
		return ORD::partial_ordering::less;
	if (c > 0)
		return ORD::partial_ordering::greater;

	c = bottom().cmp(other.bottom());
	if (c < 0)
		return ORD::partial_ordering::less;
	if (c > 0)
		return ORD::partial_ordering::greater;

	c = left().cmp(other.left());
	if (c < 0)
		return ORD::partial_ordering::less;
	if (c > 0)
		return ORD::partial_ordering::greater;

	return ORD::partial_ordering::equivalent;
}
#endif

inline bool
BrickB::operator<(const BrickB& other) const noexcept {
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	return (*this <=> other) < 0;
#else
	return cmp(other) < 0;
#endif
}

inline bool
BrickB::operator>(const BrickB& other) const noexcept {
	return other < *this;
}

inline bool
BrickB::operator<=(const BrickB& other) const noexcept {
	return !(other < *this);
}

inline bool
BrickB::operator>=(const BrickB& other) const noexcept {
	return !(*this < other);
}

inline bool
BrickB::operator==(const BrickB& other) const noexcept {
	return top() == other.top() && right() == other.right() &&
		bottom() == other.bottom() && left() == other.left();
}

inline bool
BrickB::operator!=(const BrickB& other) const noexcept {
	return !(*this == other);
}

inline bool
BrickB::valid() const noexcept {
	return top().valid() && right().valid() &&
		bottom().valid() && left().valid() &&
		!(top()[0]    && !top()[1]    && !left()[3]) &&
		!(right()[0]  && !right()[1]  && !top()[3]) &&
		!(bottom()[0] && !bottom()[1] && !right()[3]) &&
		!(left()[0]   && !left()[1]   && !bottom()[3]);
}

template<typename S1, typename S2, typename S3, typename S4>
inline
BrickB::BrickB(S1&& top__, S2&& right__, S3&& bottom__, S4&& left__)
	: top_(std::forward<S1>(top__))
	, right_(std::forward<S2>(right__))
	, bottom_(std::forward<S3>(bottom__))
	, left_(std::forward<S4>(left__))
{
}

inline
Brick::Brick(const std::vector<unsigned int>& v)
	: BrickB(v)
	, flipping(false)
	, variants_(variants(flipping))
{
}

inline
Brick::Brick(const BrickB& b)
	: BrickB(b)
	, flipping(false)
	, variants_(variants(flipping))
{
}

inline
Brick::Brick(BrickB&& b)
	: BrickB(std::move(b))
	, flipping(false)
	, variants_(variants(flipping))
{
}

#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
inline std::partial_ordering
Brick::operator<=>(const Brick& other) const noexcept {
	if (degree() < other.degree())
		return std::partial_ordering::less;
	if (degree() > other.degree())
		return std::partial_ordering::greater;
	return *this <=> static_cast<const BrickB&>(other);
}
#else
inline ORD::partial_ordering
Brick::cmp(const Brick& other) const noexcept {
	if (degree() < other.degree())
		return ORD::partial_ordering::less;
	if (degree() > other.degree())
		return ORD::partial_ordering::greater;
	return base_type::cmp(other);
}
#endif

inline bool
Brick::operator<(const Brick& other) const noexcept {
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	return (*this <=> other) < 0;
#else
	return cmp(other) < 0;
#endif
}

inline bool
Brick::operator>(const Brick& other) const noexcept {
	return other < *this;
}

inline bool
Brick::operator<=(const Brick& other) const noexcept {
	return !(other < *this);
}

inline bool
Brick::operator>=(const Brick& other) const noexcept {
	return !(*this < other);
}

inline bool
Brick::operator==(const Brick& other) const noexcept {
	return degree() == other.degree() && base_type::operator==(other);
}

inline bool
Brick::operator!=(const Brick& other) const noexcept {
	return !(*this == other);
}

inline const BrickB&
Brick::brick(unsigned int n) const noexcept {
	assert(n < degree());
	return 0 == n ? *this : variants_[n - 1];
}

inline bool
Brick::worth_flipping() const noexcept {
	return flipping;
}

extern std::ostream& operator<<(std::ostream&, const Side&);

inline std::ostream&
operator<<(std::ostream& os, const Brick& b) {
	os << '(' << b.top() << ", " << b.right() << ", " << b.bottom()
	   << ", " << b.left() << "): " << b.degree() << "; "
	   << b.worth_flipping();
	return os;
}

}
