
/**
 * Exercises the cached object-property read/write path
 * (zephir_read_property_cached / zephir_update_property_zval_cached).
 * Backs PropertyCacheTest.
 *
 * @see https://github.com/zephir-lang/zephir/issues/1884 (property access)
 */

namespace Stub;

class PropertyCache
{
	public pub = 1;
	protected prot = 2;
	private priv = 3;

	public function getPub()
	{
		return this->pub;
	}

	public function setPub(var value)
	{
		let this->pub = value;
	}

	public function getProt()
	{
		return this->prot;
	}

	public function setProt(var value)
	{
		let this->prot = value;
	}

	public function getPriv()
	{
		return this->priv;
	}

	public function setPriv(var value)
	{
		let this->priv = value;
	}

	/**
	 * Compound read-modify-write on a property (read + write emitters).
	 */
	public function incPub()
	{
		let this->pub += 1;
		return this->pub;
	}

	/**
	 * Same property read at two sites in one method: first is the cache
	 * miss/fill, second is the cache hit (Stage 2).
	 */
	public function sumPubTwice()
	{
		return this->pub + this->pub;
	}

	/**
	 * Value-type transition at one write site: array then scalar.
	 */
	public function swapArrayScalar()
	{
		let this->pub = [1, 2, 3];
		let this->pub = 42;
		return this->pub;
	}

	/**
	 * Reads a property in both arms of a branch and after it: the name slot
	 * must be declared at function scope so every arm can reference it.
	 */
	public function branchedRead(bool flag)
	{
		var out = 0;
		if flag {
			let out = this->pub;
		} else {
			let out = this->prot;
		}
		return out + this->priv;
	}

	/**
	 * Tight loop over a property read (cache-hit path under load).
	 */
	public function readLoop(int n)
	{
		var total = 0, i = 0;
		for i in range(1, n) {
			let total += this->pub;
		}
		return total;
	}

	/**
	 * Property value CHANGES on every iteration: a cached write then a cached
	 * read of the same property in the SAME iteration. The slot caches the
	 * offset, never the value, so the read must observe the just-written value
	 * each time. A stale cached value would break the running total.
	 * Returns 1 + 2 + ... + n.
	 */
	public function mutateInLoop(int n)
	{
		var i = 0, total = 0;
		for i in range(1, n) {
			let this->pub = i;
			let total += this->pub;
		}
		return total;
	}

	/**
	 * Compound read-modify-write of one property inside a loop: each iteration
	 * emits BOTH a cached read and a cached write against the same slot while
	 * the value keeps changing. Returns the final value (1 + 2 + ... + n).
	 */
	public function accumulateInLoop(int n)
	{
		var i = 0;
		let this->pub = 0;
		for i in range(1, n) {
			let this->pub += i;
		}
		return this->pub;
	}
}
