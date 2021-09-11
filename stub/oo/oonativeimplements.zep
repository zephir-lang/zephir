
/**
 * Class with constructor
 */

namespace Stub\Oo;

class OoNativeImplements implements
							\Countable,
							\Iterator,
							\OuterIterator,
							\RecursiveIterator,
							\SeekableIterator
							// \IteratorAggregate
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

	public function next() -> void
	{
	}

	public function rewind() -> void
	{
	}

	public function valid() -> boolean
	{
	}

	/* OuterIterator */

	public function getInnerIterator() -> <\Iterator>|null
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

	public function seek(int position) -> void
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
