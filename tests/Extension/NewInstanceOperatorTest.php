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

namespace Extension;

use PHPUnit\Framework\TestCase;
use Stub\Operator;

final class NewInstanceOperatorTest extends TestCase
{
    public Operator $test;

    protected array $autoloadMap = [
        'Fixture\ThrowException' => ZEPHIRPATH.'/tests/fixtures/throw-exception.php',
        'Fixture\EmptyClass' => ZEPHIRPATH.'/tests/fixtures/class-empty.php',
    ];

    protected function setUp(): void
    {
        spl_autoload_register([$this, 'autoload']);
        $this->test = new Operator();
    }

    protected function tearDown(): void
    {
        spl_autoload_unregister([$this, 'autoload']);
    }

    public function autoload(string $className): void
    {
        if (isset($this->autoloadMap[$className])) {
            include $this->autoloadMap[$className];
        }
    }

    public function testException(): void
    {
        $this->expectException(\Exception::class);
        $this->test->testNewInstanceOperator('Fixture\ThrowException');
    }

    public function testNewInstance(): void
    {
        $object = $this->test->testNewInstanceOperator('Fixture\EmptyClass');

        $this->assertInstanceOf('Fixture\EmptyClass', $object);
    }
}
