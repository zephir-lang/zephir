
/**
 * Class with constructor
 */

namespace Test\Oo;

class OoNativeImplements implements
							\Countable,
							\Iterator,
							\OuterIterator,
							\RecursiveIterator,
							\SeekableIterator,
							\IteratorAggregate
{

	/* Countable */

	public function count() -> int
	{
	}

	/* Iterator */

	public function current()
	{
	}

	public function key()
	{
	}

	public function next()
	{
	}

	public function rewind()
	{
	}

	public function valid() -> boolean
	{
	}

	/* OuterIterator */

	public function getInnerIterator()
	{
	}

	/* RecursiveIterator */

	public function getChildren() -> <\RecursiveIterator>
	{
	}
	public function hasChildren() -> boolean
	{
	}

	/* SeekableIterator */

	public function seek (int position)
	{
	}

	/* IteratorAggregate */
	public function getIterator() -> <\Traversable>
	{
	}

	/* ArrayAccess*/
	public function offsetExists (offset) -> boolean
	{
	}

	public function offsetGet (offset)
	{
	}

	public function offsetSet (offset, value)
	{
	}

	public function offsetUnset (offset)
	{
	}

	/* Serializable */
	public function serialize() -> string
	{
	}

	public function unserialize(string serialized)
	{
	}
}