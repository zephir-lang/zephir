
namespace Stub\Invokes;

abstract class AbstractInvokerComplex extends AbstractProtected
{
    public function __toString()
    {
        return this->renderArrayElementsComplex(this->text);
    }

    public function __invoke() -> <AbstractInvoker>
    {
        return this;
    }
}
