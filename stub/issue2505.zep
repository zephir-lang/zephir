namespace Stub;

class Issue2505
{
    public function makeSelf() -> <self>
    {
        return new self();
    }

    public static function makeSelfStatic() -> <self>
    {
        return new self();
    }

    public function makeStatic() -> <static>
    {
        return new static();
    }

    public static function makeStaticFromStatic() -> <static>
    {
        return new static();
    }
}
