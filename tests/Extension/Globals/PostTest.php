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

namespace Extension\Globals;

use PHPUnit\Framework\TestCase;
use Stub\Globals\Post;

final class PostTest extends TestCase
{
    /**
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function testShouldNotTriggerAnyErrorIfPostIsUndefined(): void
    {
        $tester = new Post();

        unset($_POST);

        $this->assertFalse(isset($_POST));
        $this->assertFalse($tester->hasValue('issue-1623'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function testShouldReturnFalseIfVariableIsUndefined(): void
    {
        $tester = new Post();

        $_POST = [];

        $this->assertTrue(isset($_POST));
        $this->assertFalse($tester->hasValue('issue-1623'));
    }

    /**
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function testShouldReturnTrueIfVariableIsDefined(): void
    {
        $tester = new Post();

        $_POST = ['issue-1623' => true];

        $this->assertTrue(isset($_POST));
        $this->assertTrue($tester->hasValue('issue-1623'));
    }
}
