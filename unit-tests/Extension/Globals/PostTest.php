<?php

namespace Extension\Globals;

use Test\Globals\Post;
use PHPUnit_Framework_TestCase;

class PostTest extends PHPUnit_Framework_TestCase
{
    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function shouldNotTriggerAnyErrorIfPostIsUndefined()
    {
        $tester = new Post();

        unset($_POST);

        $this->assertFalse(isset($_POST));
        $this->assertFalse($tester->hasValue('issue-1623'));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function shouldReturnFalseIfVariableIsUndefined()
    {
        $tester = new Post();

        $_POST = [];

        $this->assertTrue(isset($_POST));
        $this->assertFalse($tester->hasValue('issue-1623'));
    }

    /**
     * @test
     * @issue https://github.com/phalcon/zephir/issues/1623
     */
    public function shouldReturnTrueIfVariableIsDefined()
    {
        $tester = new Post();

        $_POST = ['issue-1623' => true];

        $this->assertTrue(isset($_POST));
        $this->assertTrue($tester->hasValue('issue-1623'));
    }
}
