#include "assemble.hh"
#include <algorithm>
#include <utility>
#include <list>
#include <iostream>
#if !defined(__cpp_impl_three_way_comparison) || __cpp_impl_three_way_comparison < 201907L
#include "compare.hpp"
#define ORD GP::impl
#else
#define ORD std
#endif

namespace happy_cube {

typedef std::reference_wrapper<const Brick> BrickRef;
typedef std::vector<BrickRef> Bricks;

typedef std::vector<BrickBRef> BrickBs;

class BO {
private:
	unsigned int brick_;
	unsigned int orientation_;

public:
	BO(unsigned int brick__, unsigned int orientation__) noexcept;

	unsigned int brick() const noexcept;
	unsigned int orientation() const noexcept;

	bool operator<(const BO& other) const noexcept;
	bool operator>(const BO& other) const noexcept;
	bool operator<=(const BO& other) const noexcept;
	bool operator>=(const BO& other) const noexcept;
	bool operator==(const BO& other) const noexcept;
	bool operator!=(const BO& other) const noexcept;
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	std::partial_ordering operator<=>(const BO&) const noexcept;
#else
	ORD::partial_ordering cmp(const BO&) const noexcept;
#endif
};

inline
BO::BO(unsigned int brick__, unsigned int orientation__) noexcept
	: brick_(brick__)
	, orientation_(orientation__)
{
}

inline unsigned int
BO::brick() const noexcept {
	return brick_;
}

inline unsigned int
BO::orientation() const noexcept {
	return orientation_;
}

#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
inline std::partial_ordering
BO::operator<=>(const BO& other) const noexcept {
#else
inline ORD::partial_ordering
BO::cmp(const BO& other) const noexcept {
#endif
	if (brick() < other.brick())
		return ORD::partial_ordering::less;
	if (brick() > other.brick())
		return ORD::partial_ordering::greater;
	if (orientation() < other.orientation())
		return ORD::partial_ordering::less;
	return orientation() > other.orientation() ?
		ORD::partial_ordering::greater : ORD::partial_ordering::equivalent;
}

inline bool
BO::operator<(const BO& other) const noexcept {
#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
	return (*this <=> other) < 0;
#else
	return cmp(other) < 0;
#endif
}

inline bool
BO::operator>(const BO& other) const noexcept {
	return other < *this;
}

inline bool
BO::operator<=(const BO& other) const noexcept {
	return !(other < *this);
}

inline bool
BO::operator>=(const BO& other) const noexcept {
	return !(*this < other);
}

inline bool
BO::operator==(const BO& other) const noexcept {
	return brick() == other.brick() && orientation() == other.orientation();
}

inline bool
BO::operator!=(const BO& other) const noexcept {
	return !(*this == other);
}

typedef std::list<BO> BOs;

class Algorithm {
private:
	const Bricks& bricks;
	std::vector<BO> solution;

	// the brick/orientations that were already tried
	BOs utop, uright, ubottom, uleft;

	// the brick/orientations that can still be used
	BOs available;

public:
	Algorithm(const Bricks&, unsigned int orientation);

	bool assemble();
	std::vector<BO>&& result() &&;

private:
	bool top();
	bool right();
	bool bottom();
	bool left();
	bool lid();

	bool fits_top(const BO&) const;
	bool fits_right(const BO&) const;
	bool fits_bottom(const BO&) const;
	bool fits_left(const BO&) const;
	bool fits_lid(const BO&) const;

	const BrickB& brick(const BO&) const noexcept;

	void undo(BOs&);
};

Algorithm::Algorithm(const Bricks& bricks__, unsigned int orientation)
	: bricks(bricks__)
{
	solution.emplace_back(0, orientation);
	for (unsigned int i = 1; i < bricks.size(); ++i)
		for (unsigned int j = 0; j < bricks[i].get().degree(); ++j)
			available.emplace_back(i, j);
}

Solution
Solution::assemble(const Brick& b1, const Brick& b2, const Brick& b3,
		   const Brick& b4, const Brick& b5, const Brick& b6) {
	Bricks bricks;
	bricks.reserve(6);
	bricks.push_back(std::cref(b1));
	bricks.push_back(std::cref(b2));
	bricks.push_back(std::cref(b3));
	bricks.push_back(std::cref(b4));
	bricks.push_back(std::cref(b5));
	bricks.push_back(std::cref(b6));
	std::sort(bricks.begin(), bricks.end(),
		  [](const Brick& b1, const Brick& b2) {
			  return b1 < b2;
		  });

	const Brick& foundation = bricks.front();
	Algorithm alg(bricks, 0);
	if (alg.assemble()) {
		Solution s;
		std::vector<BO> solution(std::move(alg).result());
		for (const BO& e: solution)
			s.emplace_back(std::cref(bricks[e.brick()].get().brick(e.orientation())));
		return s;
	}

	return Solution();
}


// so: if top() returned true => we remove all orientations
//        of the chosen brick from available

bool
Algorithm::assemble() {
	// available contains all bricks and orientations except
	// those of the foundation
	// i.e. 5 bricks and their orientations
	while (top()) {
		// available contains all bricks and orientations except
		// those of the foundation and of the top brick
		// i.e. 4 bricks and their orientations
		while (right()) {
			// available contains all bricks and orientations except
			// those of the foundation, the top, and the right brick
			// i.e. 3 bricks and their orientations
			while (bottom()) {
				// available contains all bricks and
				// orientations except those of the foundation,
				// the top, the right, and the bottom brick
				// i.e. 2 bricks and their orientations
				while (left()) {
					// available contains all bricks and
					// orientations except those of the
					// foundation, the top, the right, the
					// bottom, and the left brick
					// i.e. 1 brick and its orientations
					if (lid())
						return true;
					undo(uleft);
					// available contains all bricks and
					// orientations except those of the
					// foundation, the top, the right, and
					// the bottom brick, and except those
					// brick-orientation pairs already
					// tried to be placed at left
				}
				// available contains all bricks and
				// orientations except those of the foundation,
				// the top, the right, and the bottom brick
				// i.e. 2 bricks and their orientations
				undo(ubottom);
				// available contains all bricks and
				// orientations except those of the
				// foundation, the top, and the right brick,
				// and except those brick-orientation pairs
				// already tried to be placed at the bottom
			}
			// available contains all bricks and orientations except
			// those of the foundation, the top, and the right brick
			// i.e. 3 bricks and their orientations
			undo(uright);
			// available contains all bricks and orientations except
			// those of the foundation and the top brick, and except
			// those brick-orientation pairs already tried to be
			// placed on the right
		}
		// available contains all bricks and orientations except
		// those of the foundation and of the top brick
		// i.e. 4 bricks and their orientations
		undo(utop);
		// available contains all bricks and orientations except
		// those of the foundation and except those brick-orientation
		// pairs already tried to be placed at the top
	}
	return false;
}

bool
Algorithm::top() {
	for (const BO& e: available)
		if (fits_top(e)) {
			solution.push_back(e);
			unsigned int b = e.brick();
			// all orientations of the chosen brick are not
			// available any more
			available.remove_if(
				[b](const BO& e) {
					return e.brick() == b;
				});
			std::copy_if(utop.begin(), utop.end(),
				     std::back_inserter(available),
				     [b](const BO& e) {
					     return e.brick() != b;
				     });
			return true;
		}
	available.splice(available.end(), utop, utop.begin(), utop.end());
	return false;
}

bool
Algorithm::right() {
	for (const BO& e: available)
		if (fits_right(e)) {
			solution.push_back(e);
			unsigned int b = e.brick();
			// all orientations of the chosen brick are not
			// available any more
			available.remove_if(
				[b](const BO& e) {
					return e.brick() == b;
				});
			std::copy_if(uright.begin(), uright.end(),
				     std::back_inserter(available),
				     [b](const BO& e) {
					     return e.brick() != b;
				     });
			return true;
		}
	available.splice(available.end(), uright, uright.begin(), uright.end());
	return false;
}

bool
Algorithm::bottom() {
	for (const BO& e: available)
		if (fits_bottom(e)) {
			solution.push_back(e);
			unsigned int b = e.brick();
			// all orientations of the chosen brick are not
			// available any more
			available.remove_if(
				[b](const BO& e) {
					return e.brick() == b;
				});
			std::copy_if(ubottom.begin(), ubottom.end(),
				     std::back_inserter(available),
				     [b](const BO& e) {
					     return e.brick() != b;
				     });
			return true;
		}
	available.splice(available.end(), ubottom, ubottom.begin(), ubottom.end());
	return false;
}

bool
Algorithm::left() {
	for (const BO& e: available)
		if (fits_left(e)) {
			solution.push_back(e);
			unsigned int b = e.brick();
			// all orientations of the chosen brick are not
			// available any more
			available.remove_if(
				[b](const BO& e) {
					return e.brick() == b;
				});
			std::copy_if(uleft.begin(), uleft.end(),
				     std::back_inserter(available),
				     [b](const BO& e) {
					     return e.brick() != b;
				     });
			return true;
		}
	available.splice(available.end(), uleft, uleft.begin(), uleft.end());
	return false;
}

bool
Algorithm::lid() {
	for (const BO& e: available)
		if (fits_lid(e)) {
			solution.push_back(e);
			unsigned int b = e.brick();
			// all orientations of the chosen brick are not
			// available any more
			available.remove_if(
				[b](const BO& e) {
					return e.brick() == b;
				});
			assert(available.empty());
			return true;
		}
	return false;
}

void
Algorithm::undo(BOs& unavailable) {
	const BO& crt = solution.back();
	// mark the BO as unavailable
	unavailable.push_back(crt);
	// add all orientations of the current brick, except those in the unavailable list,
	// to the available list
	for (unsigned int i = 0; i < bricks[crt.brick()].get().degree(); ++i) {
		auto k = std::find_if(unavailable.begin(), unavailable.end(),
				      [b = crt.brick(), i](const BO& e) {
					      return e.brick() == b &&
						      e.orientation() == i;
				      });
		if (unavailable.end() == k)
			available.emplace_back(crt.brick(), i);
	}
	available.remove_if([&unavailable](const BO& e) {
		return unavailable.end() != std::find(unavailable.begin(), unavailable.end(), e);
	});
	// remove the element from the solution
	solution.pop_back();
}

bool
Algorithm::fits_top(const BO& e) const {
	const BrickB& foundation = brick(solution[0]);
	const BrickB& to_fit = brick(e);
	return foundation.top().match(to_fit.bottom().flip());
}

bool
Algorithm::fits_right(const BO& e) const {
	const BrickB& foundation = brick(solution[0]);
	const BrickB& to_fit = brick(e);
	if (foundation.right().match(to_fit.bottom().flip())) {
		const BrickB& t = brick(solution[1]);
		if (t.right().match(to_fit.left().flip())) {
			// check that the corner foundation-top-right is filled
			if (Side::corner(foundation.right()[0], t.right()[4], to_fit.left()[0]))
				return true;
		}
	}
	return false;
}

bool
Algorithm::fits_bottom(const BO& e) const {
	const BrickB& foundation = brick(solution[0]);
	const BrickB& to_fit = brick(e);
	if (foundation.bottom().match(to_fit.bottom().flip())) {
		const BrickB& r = brick(solution[2]);
		if (r.right().match(to_fit.left().flip())) {
			// check that the corner foundation-right-bottom is filled
			if (Side::corner(foundation.bottom()[0], r.right()[4], to_fit.left()[0]))
				return true;
		}
	}
	return false;
}

bool
Algorithm::fits_left(const BO& e) const {
	const BrickB& foundation = brick(solution[0]);
	const BrickB& to_fit = brick(e);
	if (foundation.left().match(to_fit.bottom().flip())) {
		const BrickB& b = brick(solution[3]);
		if (b.right().match(to_fit.left().flip())) {
			// check that the corner foundation-bottom-left is filled
			if (Side::corner(foundation.left()[0], b.right()[4], to_fit.left()[0])) {
				const BrickB& t = brick(solution[1]);
				if (t.left().match(to_fit.right().flip())) {
					// check that the corner foundation-left-top is filled
					if (Side::corner(foundation.top()[0], to_fit.right()[4], t.left()[0]))
						return true;
				}
			}
		}
	}
	return false;
}

bool
Algorithm::fits_lid(const BO& e) const {
	const BrickB& to_fit = brick(e);
	const BrickB& t = brick(solution[1]);
	const BrickB& r = brick(solution[2]);
	const BrickB& b = brick(solution[3]);
	const BrickB& l = brick(solution[4]);
	return t.top().match(to_fit.top()) &&
		r.top().match(to_fit.right()) &&
		b.top().match(to_fit.bottom()) &&
		l.top().match(to_fit.left()) &&
		Side::corner(t.top()[0], l.top()[4], to_fit.top()[0]) &&
		Side::corner(r.top()[0], t.top()[4], to_fit.right()[0]) &&
		Side::corner(b.top()[0], r.top()[4], to_fit.bottom()[0]) &&
		Side::corner(l.top()[0], t.top()[4], to_fit.left()[0]);
}

const BrickB&
Algorithm::brick(const BO& e) const noexcept {
	return bricks[e.brick()].get().brick(e.orientation());
}

std::vector<BO>&&
Algorithm::result() && {
	return std::move(solution);
}

}
