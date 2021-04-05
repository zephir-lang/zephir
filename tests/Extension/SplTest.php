<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

declare(strict_types=1);

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Spl;

final class SplTest extends TestCase
{
    /**
     * @issue https://github.com/zephir-lang/zephir/issues/1212
     */
    public function testIssue1212(): void
    {
        $this->assertInstanceOf(\SplFileObject::class, Spl::issue1212(__DIR__ . '/../fixtures/class-empty.php'));
    }
}
