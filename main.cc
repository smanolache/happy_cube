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
	// std::vector<Brick> bricks = generate_bricks();
	// for (const Brick& b: bricks)
	// 	std::cout << b << std::endl;

	// 'Watt' from the paper
	// const Brick b1(std::vector<unsigned int>{0, 2, 4, 7, 8, 10, 13, 15});
	// const Brick b2(std::vector<unsigned int>{0, 1, 3, 4, 6, 9, 11, 14, 15});
	// const Brick b3(std::vector<unsigned int>{0, 2, 6, 8, 10, 11, 12, 13, 15});
	// const Brick b4(std::vector<unsigned int>{0, 2, 4, 7, 8, 9, 11, 12, 13, 15});
	// const Brick b5(std::vector<unsigned int>{0, 2, 5, 7, 8, 9, 11, 12, 14});
	// const Brick b6(std::vector<unsigned int>{2, 4, 5, 7, 10, 13, 15});

	// 'violet' from the game
	const Brick b1(std::vector<unsigned int>{0, 1,       4, 5, 6,       9,     11, 12, 13        });
	const Brick b2(std::vector<unsigned int>{   1,    3,       6, 7, 8, 9,         12, 13, 14    });
	const Brick b3(std::vector<unsigned int>{0,    2,    4, 5, 6,          10, 11, 12,     14    });
	const Brick b4(std::vector<unsigned int>{0,    2,    4,    6,       9, 10,         13,     15});
	const Brick b5(std::vector<unsigned int>{      2, 3, 4,    6,    8, 9,         12, 13        });
	const Brick b6(std::vector<unsigned int>{0, 1,          5,    7, 8, 9,         12, 13    , 15});

	std::cout << "Input:" << std::endl;
	std::cout << b1 << std::endl;
	std::cout << b2 << std::endl;
	std::cout << b3 << std::endl;
	std::cout << b4 << std::endl;
	std::cout << b5 << std::endl;
	std::cout << b6 << std::endl;
	Solution s = Solution::assemble(b1, b2, b3, b4, b5, b6);

	std::cout << std::endl << "Output:" << std::endl;
	for (const BrickB& b: s)
		std::cout << b << std::endl;

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
