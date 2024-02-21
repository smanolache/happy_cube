#include "permutations.hh"

namespace utils {

Permutations::state::state(unsigned int n)
{
	c.reserve(n);
	for (unsigned int i = 0; i < n; ++i) {
		s.push(i);
		c.push_back(i);
	}
}

bool
Permutations::state::next() {
	unsigned int lo = c.size();
	while (!s.empty()) {
		--lo;
		unsigned int i = s.top();
		s.pop();

		if (i == lo)
			i = lo + 1;
		else {
			std::swap(c[lo], c[i]);
			++i;
		}

		if (i < c.size()) {
			std::swap(c[lo], c[i]);
			s.push(i);
			for (++lo; lo < c.size(); ++lo)
				s.push(lo);
			return true;
		}
	}
	return false;
}

std::ostream&
operator<<(std::ostream& os, const Permutations::state::value_type& v) {
	auto i = v.begin(), __li = v.end();
	if (__li != i) {
		os << *i;
		for (++i; __li != i; ++i)
			os << ' ' << *i;
	}
	return os;
}

}
