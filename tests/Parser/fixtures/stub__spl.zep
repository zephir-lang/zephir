namespace Stub;

class Spl
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1212
     */
    public static function issue1212(string file)
    {
        return new \SplFileObject(file);
    }
}
