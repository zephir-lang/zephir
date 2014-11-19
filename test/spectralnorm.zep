
namespace Test;

/**
 * SpectralNorm
 *
 * @see http://mathworld.wolfram.com/SpectralNorm.html
 */
class SpectralNorm
{
	private inline function Ax(i, j)
	{
		return 1/(((i + j) * (i + j + 1) / 2) + i + 1);
	}

	private function Au(int n, <\SplFixedArray> u, <\SplFixedArray> v)
	{
		int t, i, j;
		for i in range(0, n - 1) {
			let t = 0;
			for j in range(0, n - 1) {
				let t += this->Ax(i, j) * u->offsetGet(j);
			}
			v->offsetSet(i, t);
		}
	}

	private function Atu(int n, <\SplFixedArray> u, <\SplFixedArray> v)
	{
		int t, i, j;
		for i in range(0, n - 1) {
			let t = 0;
			for j in range(0, n - 1) {
				let t += this->Ax(j, i) * u->offsetGet(j);
			}
			v->offsetSet(i, t);
		}
	}

	private function AtAu(n, u, v, w)
	{
		this->Au(n, u, w);
		this->Atu(n, w, v);
	}

	public function process(int n)
	{
		int i, vv = 0, vBv = 0;
		var u, v, w;

		let u = new \SplFixedArray(n), v = new \SplFixedArray(n), w = new \SplFixedArray(n);

		for i in range(0, n - 1) {
			u->offsetSet(i, 1);
			v->offsetSet(i, 1);
			w->offsetSet(i, 1);
		}

		for i in range(0, 9) {
			this->AtAu(n, u, v, w);
			this->AtAu(n, v, u, w);
		}

		for i in range(0, n - 1) {
			let vBv += u->offsetGet(i) * v->offsetGet(i);
			let vv  += v->offsetGet(i) * v->offsetGet(i);
		}

		return sqrt(vBv / vv);
	}
}
