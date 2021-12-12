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
        'Fixture\ThrowException' => 'tests/fixtures/throw-exception.php',
        'Fixture\EmptyClass' => 'tests/fixtures/class-empty.php',
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
        if (version_compare(PHP_VERSION, '8.1.0', '>=')) {
            try {
                if (isset($this->autoloadMap[$className])) {
                    include $this->autoloadMap[$className];
                }
            } catch (\Throwable $exception) {
                // Ignore, as since PHP8.1 all exceptions are thrown during file include.
            }
        } else {
            if (isset($this->autoloadMap[$className])) {
                include $this->autoloadMap[$className];
            }
        }
    }

    public function testException(): void
    {
        if (version_compare(PHP_VERSION, '8.1.0', '<')) {
            $this->expectException(\Exception::class);
            $this->test->testNewInstanceOperator('Fixture\ThrowException');
        } else {
            $this->markTestSkipped('spl_autoload_register() now can only register with valid class inside included file.');
        }
    }

    public function testNewInstance(): void
    {
        $object = $this->test->testNewInstanceOperator('Fixture\EmptyClass');

        $this->assertInstanceOf('Fixture\EmptyClass', $object);
    }
}
