
namespace Stub;

/**
 * Issue #1704: a `break` written after a `return` inside a `case` is dead code
 * that PHP accepts without emitting any diagnostic. It must not raise an
 * `Unreachable code` warning, and it must not stop the `switch` from
 * satisfying the method's return-type hint.
 */
class Issue1704
{
    /**
     * The snippet reported in the issue.
     */
    public function runmode(int value) -> string
    {
        switch value {
            case 123:
                return "abc";
                break;
            default:
                throw new \Exception("Invalid application runmode.");
        }
    }

    /**
     * Every clause returns and carries the same dead `break` padding.
     */
    public function padded(int value) -> string
    {
        switch value {
            case 1:
                return "one";
                break;
            case 2:
                return "two";
                break;
            default:
                return "many";
                break;
        }
    }

    /**
     * A dead `break` inside a loop is equally silent in PHP.
     */
    public function breakAfterReturnInLoop(int limit) -> int
    {
        int i = 0;

        while i < limit {
            return i;
            break;
        }

        return -1;
    }

    /**
     * As is a dead `continue`.
     */
    public function continueAfterReturnInLoop(int limit) -> int
    {
        int i = 0;

        while i < limit {
            return i;
            continue;
        }

        return -1;
    }
}
