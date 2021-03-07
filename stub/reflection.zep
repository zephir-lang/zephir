
namespace Stub;

use ReflectionClass;
use ReflectionFunction;

class Reflection
{
    public function getReflectionClass() -> <ReflectionClass>
    {
        var r;
        let r = new ReflectionClass();

        return r;
    }

    public function getReflectionFunction() -> <ReflectionFunction>
    {
        var r;
        let r = new ReflectionFunction();

        return r;
    }
}
