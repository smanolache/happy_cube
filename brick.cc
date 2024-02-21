#include "brick.hh"

namespace happy_cube {

Side
Side::flip() const {
	base_type r;

	r.reserve(size());
	for (base_type::const_reverse_iterator i = rbegin(), __li = rend(); __li != i; ++i)
		r.push_back(4 - *i);

	return Side(std::move(r));
}

BrickB::BrickB(const std::vector<unsigned int>& v) {
	std::vector<unsigned int>::const_iterator i = v.begin(), __li = v.end();
	if (__li != i) {
		bool last = 0 == *i;
		std::vector<unsigned int> t, r, b, l;
		if (*i < 4) {
			t.push_back(*i);
			for (++i; __li != i && *i < 4; ++i)
				t.push_back(*i);
		}
		// i == __li or i != __li and *i >= 4
		if (__li != i) {
			if (*i == 4)
				t.push_back(*i);
			if (*i < 8) {
				r.push_back(*i - 4);
				for (++i; __li != i && *i < 8; ++i)
					r.push_back(*i - 4);
			}
		}
		// i == __li or i != __li and *i >= 8
		if (__li != i) {
			if (*i == 8)
				r.push_back(*i - 4);
			if (*i < 12) {
				b.push_back(*i - 8);
				for (++i; __li != i && *i < 12; ++i)
					b.push_back(*i - 8);
			}
		}
		// i == __li or i != __li and *i >= 12
		if (__li != i) {
			if (*i == 12)
				b.push_back(*i - 8);
			l.push_back(*i - 12);
			for (++i; __li != i; ++i)
				l.push_back(*i - 12);
		}
		if (last)
			l.push_back(4);

		top_ = std::move(t);
		right_ = std::move(r);
		bottom_ = std::move(b);
		left_ = std::move(l);
	}
}

std::vector<BrickB>
Brick::variants(bool& flipping) const {
	std::set<BrickB> bricks{*this};
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

	std::vector<BrickB> v;
	v.reserve(bricks.size() - 1);
	std::move(++bricks.begin(), bricks.end(), std::back_inserter(v));
	return v;
}

std::ostream&
operator<<(std::ostream& os, const Side& s) {
	os << '(';
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
