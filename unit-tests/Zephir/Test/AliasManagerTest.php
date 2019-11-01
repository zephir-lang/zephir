<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view the LICENSE
 * file that was distributed with this source code.
 */

namespace Zephir\Test;

use PHPUnit\Framework\TestCase;
use Zephir\AliasManager;

class AliasManagerTest extends TestCase
{
    /** @var \Zephir\AliasManager */
    private $testAliasMgr;

    public function setUp()
    {
        $this->testAliasMgr = new AliasManager();
    }

    private function injectExpectedResult(array $expected): array
    {
        $testDataProvider = $this->baseTestSuiteProvider();

        $index = 0;

        foreach ($testDataProvider as $testName => $testSuite) {
            array_push($testDataProvider[$testName], $expected[$index++]);
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
                    'name' => 'Throwable',
                    'alias' => 'Throwable',
                ],
            ],
        ];
    }

    public function aliasDataProvider(): array
    {
        $expected = [
            [
                'EventsManagerInterface' => 'Bug\\Events\\ManagerInterface',
            ],
            [
                'EventsManagerInterface' => '\\Bug\\Events\\ManagerInterface',
            ],
            [
                'Throwable' => 'Throwable',
            ],
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @test
     * @dataProvider aliasDataProvider
     */
    public function shouldProperAddStatements(array $useStatements, array $expected)
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $alias = $useStatements['alias'];
        $className = $useStatements['name'];

        $this->assertTrue($this->testAliasMgr->isAlias($alias));
        $this->assertSame($this->testAliasMgr->getAliases(), $expected);
        $this->assertSame($this->testAliasMgr->getAlias($alias), $className);
    }

    public function statementDataProvider(): array
    {
        $expected = [
            true, true, false,
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @test
     * @dataProvider statementDataProvider
     */
    public function shouldCheckAliasedStatement(array $useStatements, bool $expected)
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $alias = $useStatements['alias'];
        $className = $useStatements['name'];

        $this->assertSame($this->testAliasMgr->isUseStatementAliased($alias), $expected);
        $this->assertSame($this->testAliasMgr->isAliasPresentFor($className), $expected);
    }

    public function classNameDataProvider(): array
    {
        $expected = [
            'EventsManagerInterface',
            '\Bug\Events\ManagerInterface',
            'Throwable',
        ];

        return $this->injectExpectedResult($expected);
    }

    /**
     * @test
     * @dataProvider classNameDataProvider
     */
    public function shouldGetAliasForClassName(array $useStatements, string $expected)
    {
        $this->testAliasMgr->add([
            'aliases' => [$useStatements],
        ]);

        $className = $useStatements['name'];

        $this->assertSame($this->testAliasMgr->getAliasForClassName($className), $expected);
    }
}
