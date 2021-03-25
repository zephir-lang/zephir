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
use Zephir\AliasManager;

final class AliasManagerTest extends TestCase
{
    private AliasManager $testAliasMgr;

    protected function setUp(): void
    {
        $this->testAliasMgr = new AliasManager();
    }

    private function injectExpectedResult(array $expected): array
    {
        $testDataProvider = $this->baseTestSuiteProvider();

        $index = 0;
        foreach ($testDataProvider as $testName => $testSuite) {
            $testDataProvider[$testName][] = $expected[$index++];
        }

        return $testDataProvider;
    }

    public function baseTestSuiteProvider(): array
    {
        return [
            'with alias' => [
                [
                    'name' => 'Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ],
            ],
            'with alias and class name from root' => [
                [
                    'name' => '\\Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ],
            ],
            'without explicit alias' => [
                [
                    'name' => '\\Throwable',
                ],
            ],
            'without explicit alias and FQN' => [
                [
                    'name' => 'Zephir\\Compiler\\CompilerInterface',
                ],
            ],
        ];
    }

    public function aliasProvider(): array
    {
        $expected = [
            //   [ alias => name ]
            [
                'EventsManagerInterface' => 'Bug\\Events\\ManagerInterface',
            ],
            [
                'EventsManagerInterface' => '\\Bug\\Events\\ManagerInterface',
            ],
            [
                'Throwable' => '\Throwable',
            ],
            [
                'CompilerInterface' => 'Zephir\\Compiler\\CompilerInterface',
            ],
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @dataProvider aliasProvider
     *
     * @param array $useStatements
     * @param array $expected
     */
    public function testShouldProperAddStatements(array $useStatements, array $expected): void
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $className = $useStatements['name'];

        $parts = explode('\\', $className);
        $alias = $useStatements['alias'] ?? $parts[\count($parts) - 1];

        $this->assertTrue($this->testAliasMgr->isAlias($alias));
        $this->assertSame($expected, $this->testAliasMgr->getAliases());
        $this->assertSame($className, $this->testAliasMgr->getAlias($alias));
    }

    public function statementProvider(): array
    {
        $expected = [
            true, true, false, false,
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @dataProvider statementProvider
     *
     * @param array $useStatements
     * @param bool $expected
     */
    public function testShouldCheckAliasedStatement(array $useStatements, bool $expected): void
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $className = $useStatements['name'];

        $parts = explode('\\', $className);
        $alias = $useStatements['alias'] ?? $parts[\count($parts) - 1];

        $this->assertSame($expected, $this->testAliasMgr->isUseStatementAliased($alias));
        $this->assertSame($expected, $this->testAliasMgr->isAliasPresentFor($className));
    }

    public function classNameDataProvider(): array
    {
        $expected = [
            'EventsManagerInterface',
            '\Bug\Events\ManagerInterface',
            '\Throwable',
            'CompilerInterface',
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @dataProvider classNameDataProvider
     *
     * @param array $useStatements
     * @param string $expected
     */
    public function testShouldGetAliasForClassName(array $useStatements, string $expected): void
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $className = $useStatements['name'];

        $this->assertSame($expected, $this->testAliasMgr->getAliasForClassName($className));
    }

    public function testShouldCheckIfAliasPresentForClass(): void
    {
        $this->testAliasMgr->add([
            'aliases' => [
                [
                    'name' => 'One',
                    'alias' => 'One',
                ],
                [
                    'name' => 'Bug\\Events\\ManagerInterface',
                    'alias' => 'EventsManagerInterface',
                ],
                [
                    'name' => '\\Root\SomeNamespace\\SomeClassName',
                    'alias' => 'SomeClassName',
                ],
                [
                    'name' => 'AnotherClass',
                    'alias' => 'AnotherClass',
                ],
                [
                    'name' => 'Bug\\Storage\\FileSystem',
                ],
            ],
        ]);

        $this->assertTrue($this->testAliasMgr->isAliasPresentFor('Bug\\Events\\ManagerInterface'));
        $this->assertFalse($this->testAliasMgr->isAliasPresentFor('\\Root\SomeNamespace\\SomeClassName'));
        $this->assertFalse($this->testAliasMgr->isAliasPresentFor('AnotherClass'));
        $this->assertFalse($this->testAliasMgr->isAliasPresentFor('NonExistingClass'));
        $this->assertFalse($this->testAliasMgr->isAliasPresentFor('Bug\\Storage\\FileSystem'));
    }
}
