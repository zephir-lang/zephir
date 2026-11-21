namespace Stub\Bench;

/**
 * The `parent::` call-site workload of the inline cache benchmark (#2790).
 */
class SiteChild extends \Stub\Bench
{
    public function siteParentLoop(long n) -> long
    {
        long i = 0, sum = 0;

        while i < n {
            let sum += this->siteParentOnce();
            let i++;
        }

        return sum;
    }

    public function siteParentOnce() -> long
    {
        return parent::returnConst();
    }
}
