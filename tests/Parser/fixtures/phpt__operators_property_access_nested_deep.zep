namespace Debug;

class Chain
{
    public a;
    public b;
    public c;

    public function make()
    {
        let this->a = new Chain();
        let this->a->b = new Chain();
        let this->a->b->c = 42;
    }
}