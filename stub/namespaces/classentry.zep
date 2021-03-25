namespace Stub\Namespaces;

use Stub\Namespaces\A\B\Sub;

class ClassEntry
{
    public function setParam(<\Stub\Namespaces\A\B\Sub> param) -> <\Stub\Namespaces\A\B\Sub>
    {
        return param;
    }

    public function setParamImported(<Sub> param) -> <Sub>
    {
        return param;
    }
}
