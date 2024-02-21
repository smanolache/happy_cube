#include "assemble.hh"
#include <algorithm>
#include <utility>
#include <list>

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

typedef std::list<BO> BOs;

class Algorithm {
private:
	const Bricks& bricks;
	BOs solution;

	// the brick/orientations that were already tried
	BOs unavailable;

	// the brick/orientations that can still be used
	BOs available;

public:
	Algorithm(const Bricks&, unsigned int orientation);

	bool assemble();
	BOs&& result() &&;

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

	void undo();
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
		BOs solution(std::move(alg).result());
		for (const BO& e: solution)
			s.emplace_back(std::cref(bricks[e.brick()].get().brick(e.orientation())));
		return s;
	}

	if (foundation.worth_flipping()) {
		// flip the foundation
		BrickB foundation2(foundation.t(6));
		unsigned int orientation = 0;
		for (; orientation < foundation.degree(); ++orientation)
			if (foundation2 == foundation.brick(orientation))
				break;
		Algorithm alg(bricks, orientation);
		if (alg.assemble()) {
			Solution s;
			BOs solution(std::move(alg).result());
			for (const BO& e: solution)
				s.emplace_back(std::cref(bricks[e.brick()].get().brick(e.orientation())));
			return s;
		}
	}

	return Solution();
}

bool
Algorithm::assemble() {
	while (top()) {
		while (right()) {
			while (bottom()) {
				while (left()) {
					if (lid())
						return true;
					undo();
				}
				undo();
			}
			undo();
		}
		undo();
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
			unavailable.clear();
			return true;
		}
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
			unavailable.clear();
			return true;
		}
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
			unavailable.clear();
			return true;
		}
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
			unavailable.clear();
			return true;
		}
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
			unavailable.clear();
			return true;
		}
	return false;
}

void
Algorithm::undo() {
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
	// remove the element from the solution
	solution.pop_back();
}

bool
Algorithm::fits_top(const BO& e) const {
	return true;
}

bool
Algorithm::fits_right(const BO& e) const {
	return true;
}

bool
Algorithm::fits_bottom(const BO& e) const {
	return true;
}

bool
Algorithm::fits_left(const BO& e) const {
	return true;
}

bool
Algorithm::fits_lid(const BO& e) const {
	return false;
}

BOs&&
Algorithm::result() && {
	return std::move(solution);
}

}
