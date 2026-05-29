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

namespace Zephir\Test\Class\Method;

use PHPUnit\Framework\TestCase;
use Zephir\Class\Definition\Definition;
use Zephir\Class\Method\Method;
use Zephir\StatementsBlock;

/**
 * @see https://github.com/zephir-lang/zephir/issues/1849
 */
final class MethodIsGeneratorTest extends TestCase
{
    public function testReturnsFalseWhenNoStatementsBlockSet(): void
    {
        $method = new Method(
            new Definition('Stub', 'Counter'),
            ['public'],
            'noop'
        );

        $this->assertFalse($method->isGenerator());
    }

    public function testReturnsFalseForNonGeneratorBody(): void
    {
        $method = $this->methodWithBody([
            [
                'type' => 'return',
                'expr' => ['type' => 'int', 'value' => '1'],
                'file' => 't.zep',
                'line' => 1,
                'char' => 1,
            ],
        ]);

        $this->assertFalse($method->isGenerator());
    }

    public function testReturnsTrueForBodyContainingYield(): void
    {
        $method = $this->methodWithBody([
            [
                'type' => 'yield',
                'expr' => ['type' => 'int', 'value' => '1'],
                'file' => 't.zep',
                'line' => 2,
                'char' => 1,
            ],
        ]);

        $this->assertTrue($method->isGenerator());
    }

    public function testCachesResultAcrossCalls(): void
    {
        $statements = new StatementsBlock([
            ['type' => 'yield', 'file' => 't.zep', 'line' => 1, 'char' => 1],
        ]);

        $method = new Method(
            new Definition('Stub', 'Counter'),
            ['public'],
            'pump',
            null,
            $statements
        );

        $this->assertTrue($method->isGenerator());
        $this->assertTrue($method->isGenerator());
    }

    /**
     * @param array<int, array<string, mixed>> $statements
     */
    private function methodWithBody(array $statements): Method
    {
        return new Method(
            new Definition('Stub', 'Counter'),
            ['public'],
            'body',
            null,
            new StatementsBlock($statements)
        );
    }
}
