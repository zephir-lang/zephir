
namespace Stub;

use ReflectionClass;
use ReflectionFunction;

class Reflection
{
    public function getReflectionClass() -> <ReflectionClass>
    {
        return new ReflectionClass();
    }

    public function getReflectionFunction() -> <ReflectionFunction>
    {
        return new ReflectionFunction();
    }
}
