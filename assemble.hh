#pragma once

#include <vector>
#include "brick.hh"
#include <utility>

namespace happy_cube {

typedef std::reference_wrapper<const BrickB> BrickBRef;

class Solution: protected std::vector<BrickBRef> {
private:
	typedef std::vector<BrickBRef> base_type;

public:
	using base_type::base_type;

	using base_type::operator[];
	using base_type::begin;
	using base_type::end;
	using base_type::empty;
	using base_type::size;

	static Solution assemble(const Brick& b1, const Brick& b2, const Brick& b3,
				 const Brick& b4, const Brick& b5, const Brick& b6);

private:
	Solution(std::vector<BrickBRef>&& v) noexcept;
};

inline
Solution::Solution(std::vector<BrickBRef>&& v) noexcept
	: base_type(std::move(v))
{
}

}
