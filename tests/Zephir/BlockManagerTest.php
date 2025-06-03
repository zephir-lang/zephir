<?php

declare(strict_types=1);

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\BlockManager;

final class BlockManagerTest extends TestCase
{
    public function testBasics(): void
    {
        $testSubject = new BlockManager();

        $testSubject->add('test1');
        $testSubject->add('test2');
        $testSubject->add('test1');

        $this->assertFalse($testSubject->isEmpty());

        $this->assertEquals(
            [
                '5a105e8b9d40e1329780d62ea2265d8a' => 'test1',
                'ad0234829205b9033196ba818f7a872b' => 'test2',
            ],
            $testSubject->get(),
        );
    }
}
