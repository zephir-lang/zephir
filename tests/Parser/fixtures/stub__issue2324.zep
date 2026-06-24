namespace Stub;

class Issue2324
{
    public function __construct()
    {
    }

    public static function makeStatic()
    {
        var obj;
        let obj = new static();
        return get_class(obj);
    }

    public static function makeSelf()
    {
        var obj;
        let obj = new self();
        return get_class(obj);
    }

    public function makeStaticInstance()
    {
        var obj;
        let obj = new static();
        return get_class(obj);
    }
}
