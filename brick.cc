#include "brick.hh"
#include <set>

namespace happy_cube {

bool
Side::valid() const noexcept {
	if (operator[](0)) {
		for (std::size_t i = 1; i < size(); ++i)
			if (!operator[](i))
				return true;
		return false;
	}
	for (std::size_t i = 1; i < size(); ++i)
		if (operator[](i))
			return true;
	return false;
}

Side
Side::flip() const noexcept {
	base_type r;
	r[0] = operator[](4);
	r[1] = operator[](3);
	r[2] = operator[](2);
	r[3] = operator[](1);
	r[4] = operator[](0);
	return Side(std::move(r));
}

BrickB::BrickB(const std::vector<unsigned int>& v) {
	std::vector<unsigned int>::const_iterator i = v.begin(), __li = v.end();
	if (__li != i) {
		bool last = 0 == *i;
		std::array<bool, 5> t{true, true, true, true, true},
			r{true, true, true, true, true},
			b{true, true, true, true, true},
			l{true, true, true, true, true};
		if (*i < 4) {
			t[*i] = false;
			for (++i; __li != i && *i < 4; ++i)
				t[*i] = false;
		}
		// i == __li or i != __li and *i >= 4
		if (__li != i) {
			if (*i == 4)
				t[4] = false;
			if (*i < 8) {
				r[*i - 4] = false;
				for (++i; __li != i && *i < 8; ++i)
					r[*i - 4] = false;
			}
		}
		// i == __li or i != __li and *i >= 8
		if (__li != i) {
			if (*i == 8)
				r[4] = false;
			if (*i < 12) {
				b[*i - 8] = false;
				for (++i; __li != i && *i < 12; ++i)
					b[*i - 8] = false;
			}
		}
		// i == __li or i != __li and *i >= 12
		if (__li != i) {
			if (*i == 12)
				b[4] = false;
			l[*i - 12] = false;
			for (++i; __li != i; ++i)
				l[*i - 12] = false;
		}
		if (last)
			l[4] = false;

		top_ = std::move(t);
		right_ = std::move(r);
		bottom_ = std::move(b);
		left_ = std::move(l);
	}
}

std::vector<BrickB>
Brick::variants(bool& flipping) const {
	std::set<BrickB> bricks;
	auto crt = bricks.insert(*this).first;

	unsigned int k = 1;
	// rotations without flipping
	for (; k < 4; ++k)
		bricks.emplace(t(k));
	// flip horizontally
	if (bricks.emplace(t(k)).second)
		// new configuration because of flipping
		flipping = true;
	// other flips
	for (++k; k < 8; ++k)
		bricks.emplace(t(k));

	bricks.erase(crt);

	std::vector<BrickB> v;
	v.reserve(bricks.size());
	std::move(bricks.begin(), bricks.end(), std::back_inserter(v));
	return v;
}

std::ostream&
operator<<(std::ostream& os, const Side& s) {
	os << std::boolalpha << '(';
	Side::const_iterator i = s.begin(), __li = s.end();
	if (__li != i) {
		os << *i;
		for (++i; __li != i; ++i)
			os << ", " << *i;
	}
	os << ')';
	return os;
}

}
