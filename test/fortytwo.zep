
/**
 * FortyTwo
 */

namespace Test;

class FortyTwo
{

	public function proof() -> void
	{
		var box, side;
		int i, j;

		let box = [
			[10, 24,  8], [ 8, 15, 19], [19, 17,  6],
			[ 6, 16, 20], [20, 13,  9], [9,  11, 22],
			[22,  2, 18], [18,  4, 20], [5,  21, 16],
			[16,  3, 23], [23,  7, 12], [12, 25,  5],
			[24,  7, 11], [11,  27, 4], [11, 27,  4]
		];
		for side in box {
			let j = 0;
			for i in range(0, 2) {
				let j += side[i];
			}
			if j != 42 {
				throw new Exception("not true");
			}
		}
	}
}
