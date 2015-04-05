
namespace Test;

/**
 * Test\Fannkuch
 *
 * Fannkuch Redux in Zephir
 *
 * @see http://disciple-devel.blogspot.mx/2010/11/shootout-fannkuch-redux.html
 */
class Fannkuch
{

	public function process(int n)
	{
		var perm, perm0, perm1, count, temp;
		int maxFlipsCount = 0;
		int permCount = 0;
		int checksum = 0;
		int i, j, flipsCount, k, r, k2;

		let perm = [],
			perm1 = [],
			count = [];

		for i in range(0, n) {
			let perm1[i] = i;
		}

		let r = n;
		loop {

			/* ??? */
			if false {
				break;
			}

			while r != 1 {
				let count[r - 1] = r, r--;
			}

			for i in range(0, n) {
				let perm[i] = perm1[i];
			}

			let flipsCount = 0, k = 0;

			while perm[0] != 0 {
				let k = (int) perm[0], k2 = ((k + 1) / 2) - 1;
				for i in range(0, k2) {
					let temp = perm[i], perm[i] = perm[k - i], perm[k - i] = temp;
				}
				let flipsCount++;
			}

			if maxFlipsCount < flipsCount {
				let maxFlipsCount = flipsCount;
			}

			if permCount % 2 == 0 {
				let checksum += flipsCount;
			} else {
				let checksum -= flipsCount;
			}

			/* Use incremental change to generate another permutation */
			loop {

				if r == n {
					//echo checksum, '\n';
					//echo "Pfannkuchen(", n, ") = ", maxFlipsCount, '\n';
					return [checksum, n, maxFlipsCount];
				}

				let perm0 = perm1[0], i = 0;
				while i < r {
					let j = i + 1,
						perm1[i] = perm1[j],
						i = j;
				}

				let perm1[r] = perm0, count[r] = count[r] - 1;
				if count[r] > 0 {
					break;
				}

				let r++;
			}
			let permCount++;
		}
	}
}
