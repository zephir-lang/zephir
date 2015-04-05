
namespace Test;

class Sort
{

	public fn quick(array arr)
	{
		int i, length, pivot, item;
		var left, right;

		let length = count(arr);

		if length <= 1 {
			return arr;
		}

		let pivot = (int) arr[0],
			left = [],
			right = [];

		for i in range(1, length - 1) {
			let item = (int) arr[i];
			if item < pivot {
				let left[] = item;
			} else {
				let right[] = item;
			}
		}

		// use recursion to now sort the left and right lists
		return array_merge(
			this->quick(left),
			[pivot],
			this->quick(right)
		);
	}
}