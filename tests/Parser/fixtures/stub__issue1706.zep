
namespace Stub;

/**
 * Issue #1706: a switch that has a `default` clause and returns on every
 * path is exhaustive and must satisfy the return-type hint.
 */
class Issue1706
{
    public function testSwitch() -> int
    {
        var a;
        let a = 1;

        switch a {
            case 1:
            default:
                return 1;
            case 2:
                return 2;
        }
    }

    public function switchDefaultLast(int a) -> int
    {
        switch a {
            case 1:
                return 10;
            case 2:
                return 20;
            default:
                return 0;
        }
    }

    public function switchWithThrow(int a) -> string
    {
        switch a {
            case 1:
                return "one";
            default:
                throw new \Exception("bad");
        }
    }
}
