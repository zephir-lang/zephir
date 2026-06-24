
namespace Stub\Invokes;

abstract class AbstractInvoker extends AbstractProtected
{
    public function __toString()
    {
        return this->renderArrayElements(this->text);
    }

    public function __invoke() -> <AbstractInvoker>
    {
        return this;
    }
}
