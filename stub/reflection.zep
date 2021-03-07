
namespace Stub;

use ReflectionClass;
use ReflectionFunction;
use ReflectionParameter;

class Reflection
{
    private reflectionParameter = null;

    public function getReflectionClass() -> <ReflectionClass>
    {
        var r;
        let r = new ReflectionClass(this);

        return r;
    }

    public function getReflectionFunction() -> <ReflectionFunction>
    {
        var r;
        let r = new ReflectionFunction("abs");

        return r;
    }

    public function setReflectionParameter(<ReflectionParameter> parameter) -> <ReflectionParameter>
    {
        let this->reflectionParameter = parameter;

        return this->reflectionParameter;
    }

    public function getReflectionParameter() -> <ReflectionParameter> | null
    {
        return this->reflectionParameter;
    }
}
