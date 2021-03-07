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
use Stub\Pregmatch;

final class PregmatchTest extends TestCase
{
    private Pregmatch $test;

    protected function setUp(): void
    {
        $this->test = new Pregmatch();
    }

    public function testPregMatch(): void
    {
        $this->assertSame(1, $this->test->testWithoutReturnAndMatches());
        $this->assertSame(['def'], $this->test->testWithoutReturns());
        $this->assertSame(1, $this->test->testWithoutMatches());
        $this->assertSame(1, $this->test->testPregMatchAll());
        $this->assertSame(1, $this->test->testPregMatchFallback());
        $this->assertSame(1, $this->test->testPregMatch2Params('#asd#', 'asd'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/287
     */
    public function testCollectMatches(): void
    {
        $this->assertSame(['asd'], $this->test->testPregMatchSaveMatches('asd', '#asd#'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/144
     */
    public function testPregMatchAllFlags(): void
    {
        $arr = $this->test->testMatchAllInZep();
        $this->assertSame($arr[0], [['test1', 'test2'], ['test1', 'test2']]);
        $this->assertSame($arr[1], [['test1', 'test1'], ['test2', 'test2']]);
    }
}
