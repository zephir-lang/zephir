
namespace Stub\Properties;

class GetObjectVars
{
    public    a = 1;
    protected b = 2;
    private   c = 3;

    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1245
     */
    public function issue1245() -> array
    {
        return get_object_vars(this);
    }
}
