namespace Stub\Flow;

class SwitchFlow
{
	public function testSwitch1() -> int
    {
        var a;
        let a = 10;
        switch a { }
        return a;
    }

    public function testSwitch2() -> bool
    {
        var a;
        let a = 10;
        switch a {
            case 10:
                return true;
        }
        return false;
    }

    public function testSwitch3() -> bool
    {
        var a;
        let a = 10;
        switch a {
            case 5:
                return true;
        }
        return false;
    }

    public function testSwitch4() -> int
    {
        var a;
        let a = 10;
        switch a {
            case 5:
                return 1;
            default:
                return 2;
        }
        return 3;
    }

    public function testSwitch5() -> int
    {
        var a;
        let a = 10;
        switch a {
            case 10:
                return 1;
            default:
                return 2;
        }
        return 3;
    }

    public function testSwitch6() -> int
    {
        var a;
        let a = 10;
        switch a {
            case 10:
            case 4:
                return 1;
        }
        return 2;
    }

    public function testSwitch7() -> int
    {
        var a;
        let a = 4;
        switch a {
            case 10:
            case 4:
                return 1;
        }
        return 2;
    }

    public function testSwitch8() -> int
    {
        var a;
        let a = 5;
        switch a {
            case 10:
            case 4:
                return 1;
        }
        return 2;
    }

    public function testSwitch9() -> int
    {
        var a;
        let a = 5;
        switch a {
            case 3:
            case 4:
                return 0;
            case 10:
            case 4:
                return 1;
        }
        return 2;
    }

    public function testSwitch10() -> int
    {
        var a;
        let a = 5;
        switch a {

            case 3:
            case 4:
                return 0;

            case 10:
            case 4:
                return 1;
        }
        return 2;
    }

    public function testSwitch11()
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

    public function testSwitch12(var1, var2)
    {
        switch true {
            case var1 > var2 && var2 > 5:
                return 1;
            case var1 < var2 && var1 < 5:
                return 2;
            default:
                return 0;
        }
    }

    public function testSwitch13(long a)
    {
        switch mt_rand(1, 2) {
            case 100:
                return 1;
            default:
                return 0;
        }
    }

    public function testSwitch14(result_type = 1)
    {
        var ret;

        switch (result_type) {
            case 1:
            case MYSQL_ASSOC:
            case MYSQLI_ASSOC:
                let ret = "aaa";
                break;

            default:
                let ret = "bbb";
                break;
        }

        return ret;
    }

    /**
     * Issue #1704: PHP fall-through. A clause without `break` runs on into the
     * next clause's body, and on into `default`.
     */
    public function testSwitch15(int a) -> string
    {
        string r = "";

        switch a {
            case 1:
                let r .= "a";
            case 2:
                let r .= "b";
            default:
                let r .= "d";
        }

        return r;
    }

    /**
     * Issue #1704: a `default` clause in the middle falls through into the
     * `case` written after it.
     */
    public function testSwitch16(int a) -> string
    {
        string r = "";

        switch a {
            case 1:
                let r .= "one";
                break;
            default:
                let r .= "def";
            case 2:
                let r .= "two";
        }

        return r;
    }

    /**
     * Issue #1704: `continue` inside a `switch` is equivalent to `break`, as
     * in PHP - the loop body after the `switch` still runs.
     */
    public function testSwitch17() -> string
    {
        string r = "";
        var i;

        for i in 0..3 {
            switch i {
                case 1:
                    continue;
                default:
                    break;
            }

            let r .= (string) i;
        }

        return r;
    }

    /**
     * Issue #1704: a `break` inside a loop nested in a clause breaks the loop,
     * not the `switch`.
     */
    public function testSwitch18() -> string
    {
        string r = "";
        int n = 0;

        switch 1 {
            case 1:
                while true {
                    let n++;

                    if n > 2 {
                        break;
                    }
                }

                let r .= "w";
                let r .= (string) n;
        }

        return r;
    }

    /**
     * Issue #1704: an empty clause falls through into the next body, which
     * then falls through into `default`.
     */
    public function testSwitch19(int a) -> string
    {
        string r = "";

        switch a {
            case 1:
            case 2:
                let r .= "x";
            default:
                let r .= "y";
        }

        return r;
    }
}
