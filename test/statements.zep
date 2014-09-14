namespace Test;

class Statements
{
    public function testElseIf(int num)
    {
        if num > 0 {
            return "more";
        } elseif num == 0 {
            return "equal";
        } elseif num == -1 {
            return "-1";
        } else {
            return "less";
        }
    }
}
