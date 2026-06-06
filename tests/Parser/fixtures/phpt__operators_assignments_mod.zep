namespace Imagic;

class Test
{
    public function mod(int degrees)
    {
        let degrees %= 360;
        return degrees;
    }
}