#include "combinations.hh"

namespace utils {

Combinations::state::state(unsigned int n__, unsigned int k__)
	: n((assert(n__ >= k__), n__))
	, k(k__)
{
	c.reserve(k);
	for (unsigned int i = 0; i < k; ++i)
		c.push_back(i);
}

bool
Combinations::state::next() noexcept {
	for (unsigned int i = k; i > 0;) {
		--i;
		unsigned int l = c[i];
		if (k - i + l < n) {
			++l;
			for (unsigned int j = 0; j < k - i; ++j)
				c[i + j] = l + j;
			return true;
		}
	}
	return false;
}

std::ostream&
operator<<(std::ostream& os, const Combinations::state::value_type& v) {
	auto i = v.begin(), __li = v.end();
	if (__li != i) {
		os << *i;
		for (++i; __li != i; ++i)
			os << ' ' << *i;
	}
	return os;
}

}
