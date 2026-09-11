namespace Test;

use Test\Marker as M;

#[Marker]
class Plain {}

#[Marker(1, -2, 1.5, "s", true, false, null, [1, "k": "v"], flag: true)]
abstract class WithArgs {}

#[A]
#[B(2)]
final class Stacked {}

#[A, B(3)]
class Grouped {}

/**
 * Doc first, then attributes.
 */
#[M]
class Documented {}

#[Marker]
interface Contract {}

#[Marker]
trait Helper {}

#[Marker]
function topLevel() {}

class Bare {}