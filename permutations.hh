#pragma once

#include <cassert>
#include <vector>
#include <ostream>
#include <stack>

namespace utils {

class Permutations {
private:
	struct state {
		typedef std::stack<unsigned int> stack;
		struct value_type: protected std::vector<unsigned int> {
			typedef std::vector<unsigned int> base_type;

			using base_type::base_type;
			using base_type::reserve;
			using base_type::push_back;
			using base_type::operator[];
			using base_type::begin;
			using base_type::end;
			using base_type::size;

			const std::vector<unsigned int>& elements() const noexcept;
		};
		typedef value_type const& reference;

		value_type c;
		stack s;

		state(unsigned int n);

		reference crt() const noexcept;
		bool next();
	};

friend std::ostream& operator<<(std::ostream&, const Permutations::state::value_type&);

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
		iterator& operator++();
		iterator operator++(int);

		bool operator==(const iterator&) const noexcept;
		bool operator!=(const iterator&) const noexcept;

	private:
		iterator(state&) noexcept;

	friend class Permutations;
	};

	Permutations(unsigned int n);
	Permutations(const Permutations&) = delete;

	iterator begin() noexcept;
	iterator end() const noexcept;
};

inline Permutations::state::reference
Permutations::state::crt() const noexcept {
	return c;
}

inline const std::vector<unsigned int>&
Permutations::state::value_type::elements() const noexcept {
	return *this;
}

inline
Permutations::iterator::iterator() noexcept
	: s(nullptr)
{
}

inline
Permutations::iterator::iterator(state& s__) noexcept
	: s(&s__)
{
}

inline Permutations::iterator::reference
Permutations::iterator::operator*() const noexcept {
	assert(nullptr != s);
	return s->crt();
}

inline Permutations::iterator::pointer
Permutations::iterator::operator->() const noexcept {
	return s ? &s->crt() : nullptr;
}

inline Permutations::iterator&
Permutations::iterator::operator++() {
	if (s && !s->next())
		s = nullptr;
	return *this;
}

inline Permutations::iterator
Permutations::iterator::operator++(int) {
	iterator r(*this);
	++*this;
	return r;
}

inline bool
Permutations::iterator::operator==(const iterator& other) const noexcept {
	return s == other.s;
}

inline bool
Permutations::iterator::operator!=(const iterator& other) const noexcept {
	return !(*this == other);
}

inline
Permutations::Permutations(unsigned int n)
	: s(n)
{
}

inline Permutations::iterator
Permutations::begin() noexcept {
	return iterator(s);
}

inline Permutations::iterator
Permutations::end() const noexcept {
	return iterator();
}

extern std::ostream& operator<<(std::ostream&, const Permutations::state::value_type&);

}
