/**
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Stubs;

use Throwable;
use Stubs\Exception;

/**
 * Generated Stub should have `extends Exception` without slash
 * because we using import for this class with FQN
 */
class Issue_1907 extends Exception implements Throwable
{
    public function __construct() {}
}
