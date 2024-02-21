#include "combinations.hh"
#include "brick.hh"
#include <iostream>
#include <set>
#include <algorithm>
#include "assemble.hh"

using happy_cube::Brick;
using happy_cube::BrickB;
using happy_cube::Solution;

static std::vector<Brick> generate_bricks();

int
main(int argc, char *argv[]) {
	std::vector<Brick> bricks = generate_bricks();
	for (const Brick& b: bricks)
		std::cout << b << std::endl;

	Solution s = Solution::assemble(bricks[0], bricks[1], bricks[2],
					bricks[3], bricks[4], bricks[5]);
	return 0;
}

static std::vector<Brick>
generate_bricks() {
	typedef std::set<BrickB> BricksSet;
	BricksSet aux;
	for (unsigned int i = 1; i < 16; ++i) {
		utils::Combinations c(16, i);
		for (const auto& x: c) {
			BrickB b(x.elements());
			if (b.valid()) {
				BricksSet::iterator e = aux.end();
				unsigned int k = 0;
				for (; k < 8; ++k)
					if (e != aux.find(b.t(k)))
						break;
				if (8 == k)
					aux.emplace(std::move(b));
			}
		}
	}

	typedef std::vector<Brick> Bricks;
	Bricks bricks;
	std::move(aux.begin(), aux.end(), std::back_inserter(bricks));
	std::sort(bricks.begin(), bricks.end());

	return bricks;
}
