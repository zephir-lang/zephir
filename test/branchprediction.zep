
/**
 * OO operations
 */

namespace Test;

class BranchPrediction
{
	public function testLikely1()
	{
		if likely 1 == 1 {
			return true;
		} else {
			return false;
		}
	}

	public function testLikely2(var a)
	{
		if likely a === 1 {
			return true;
		} else {
			return false;
		}
	}

	public function testUnlikely1()
	{
		if likely 1 == 1 {
			return true;
		} else {
			return false;
		}
	}

	public function testUnlikely2(var a)
	{
		if likely a === 1 {
			return true;
		} else {
			return false;
		}
	}
}
