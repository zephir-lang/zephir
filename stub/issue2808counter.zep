namespace Stub;

/**
 * The same operations on `this`, whose accesses use the inline cache, with
 * declared typed properties.
 *
 * @see https://github.com/zephir-lang/zephir/issues/2808
 */
class Issue2808Counter
{
    public int counter = 0;

    public string text = "";

    public plain = 0;

    public function increment()
    {
        let this->counter++;
    }

    public function decrement()
    {
        let this->counter--;
    }

    public function addCounter(var v)
    {
        let this->counter += v;
    }

    public function appendText(var v)
    {
        let this->text .= v;
    }

    public function addPlain(var v)
    {
        let this->plain += v;
    }

    public function appendPlain(var v)
    {
        let this->plain .= v;
    }

    public function incrementPlain()
    {
        let this->plain++;
    }
}
