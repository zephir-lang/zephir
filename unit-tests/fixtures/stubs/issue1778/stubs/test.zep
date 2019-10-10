/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Stubs;

use Fixtures\Ide_Stubs\Interfaces\DiInterfaceExample;

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
class Test
{
    const PROPERTY_EXAMPLE = "test_property";

    /** @var <DiInterfaceExample> */
    private _container;

    /** @var array */
    protected config;

    public function __construct(<DiInterfaceExample> container, array config)
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
