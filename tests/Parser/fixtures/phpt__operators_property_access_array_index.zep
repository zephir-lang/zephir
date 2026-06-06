namespace Debug;

class Container
{
    public items;

    public function set()
    {
        let this->items[0] = "hello";
        let this->items["key"] = "world";
    }
}