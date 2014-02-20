namespace Test;

class References
{
    public function intChange(int &a, int b)
    {
        let a = b;
        return b;
    }

    public function stringChange(string &a, string b)
    {
        let a = b;
        return b;
    }

    public function boolChange(bool &a, bool b)
    {
        let a = b;
        return b;
    }

    public function arrayChange(var &a, string key, var b)
    {
        let a[key] = b;
        return b;
    }
}