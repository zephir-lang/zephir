/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Stubs;

use Stubs\Dependency;

/**
 * Test Class Example Description
 *
 * <code>
 * $container = new Di();
 * $config = [
 *    'param' => 42
 * ];
 *
 * $example = new Test($container, $config);
 * </code>
 */
class UseBlock
{
    const PROPERTY_EXAMPLE = "test_property";

    /** @var <Dependency> */
    private _container;

    /** @var array */
    protected config;

    public function __construct(<Dependency> container, array config)
    {
        let this->_container = container;
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
