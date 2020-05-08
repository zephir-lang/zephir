/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Stubs;

use Psr\Http\Message\RequestInterface;

/**
 * Test Class Example Description
 *
 * <code>
 * $container = new Di();
 * $config = [
 *    'param' => 42
 * ];
 *
 * $example = new Issue_1778($request, $config);
 * </code>
 */
class Issue_1778
{
    const PROPERTY_EXAMPLE = "test_property";

    /** @var <RequestInterface> */
    private request;

    /** @var array */
    protected config;

    public function __construct(<RequestInterface> request, array config)
    {
        let this->request = request;
        let this->config = config;
    }

    /**
     * Some useful description
     *
     * @return var|null
     */
    public function getVar(string! key) -> var | null
    {
        return null;
    }
}
