namespace Stub;

/**
 * Has no constructor, so it is also a Zephir class to build without one.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2839
 */
class Issue2839
{
    public function build(string className, array parameters)
    {
        return create_instance_params(className, parameters);
    }
}
