#pragma once

#include <cassert>
#include <vector>
#include <ostream>

namespace utils {

class Combinations {
private:
	struct state {
		struct value_type: protected std::vector<unsigned int> {
			typedef std::vector<unsigned int> base_type;

			using base_type::base_type;
			using base_type::reserve;
			using base_type::push_back;
			using base_type::operator[];
			using base_type::begin;
			using base_type::end;

			const std::vector<unsigned int>& elements() const noexcept;
		};
		typedef value_type const& reference;

		unsigned int n, k;
		value_type c;

		state(unsigned int n, unsigned int k);

		reference crt() const noexcept;
		bool next() noexcept;
	};

friend std::ostream& operator<<(std::ostream&, const Combinations::state::value_type&);

	state s;

public:
	class iterator {
	private:
		state *s;

	public:
		typedef state::value_type value_type;
		typedef value_type const& reference;
		typedef value_type const *pointer;

	public:
		iterator() noexcept;

		reference operator*() const noexcept;
		pointer operator->() const noexcept;
		iterator& operator++() noexcept;
		iterator operator++(int);

		bool operator==(const iterator&) const noexcept;
		bool operator!=(const iterator&) const noexcept;

	private:
		iterator(state&) noexcept;

	friend class Combinations;
	};

	Combinations(unsigned int n, unsigned int k);
	Combinations(const Combinations&) = delete;

	iterator begin() noexcept;
	iterator end() const noexcept;
};

inline Combinations::state::reference
Combinations::state::crt() const noexcept {
	return c;
}

inline const std::vector<unsigned int>&
Combinations::state::value_type::elements() const noexcept {
	return *this;
}

inline
Combinations::iterator::iterator() noexcept
	: s(nullptr)
{
}

inline
Combinations::iterator::iterator(state& s__) noexcept
	: s(&s__)
{
}

inline Combinations::iterator::reference
Combinations::iterator::operator*() const noexcept {
	assert(nullptr != s);
	return s->crt();
}

inline Combinations::iterator::pointer
Combinations::iterator::operator->() const noexcept {
	return s ? &s->crt() : nullptr;
}

inline Combinations::iterator&
Combinations::iterator::operator++() noexcept {
	if (s && !s->next())
		s = nullptr;
	return *this;
}

inline Combinations::iterator
Combinations::iterator::operator++(int) {
	iterator r(*this);
	++*this;
	return r;
}

inline bool
Combinations::iterator::operator==(const iterator& other) const noexcept {
	return s == other.s;
}

inline bool
Combinations::iterator::operator!=(const iterator& other) const noexcept {
	return !(*this == other);
}

inline
Combinations::Combinations(unsigned int n, unsigned int k)
	: s(n, k)
{
}

inline Combinations::iterator
Combinations::begin() noexcept {
	return iterator(s);
}

inline Combinations::iterator
Combinations::end() const noexcept {
	return iterator();
}

extern std::ostream& operator<<(std::ostream&, const Combinations::state::value_type&);

}
