<?php

namespace Zephir\Stubs;

class DocBlockTest extends \PHPUnit_Framework_TestCase
{
    public function testInitialState()
    {
        $docBlock = new DocBlock('');

        $this->assertSame('', (string)$docBlock);
    }

    public function testDescription()
    {
        $doc = <<<DOC
/**
 * Hello world
 * I am test.
 */
DOC;
        $expected = <<<DOC
    /**
     * Hello world
     * I am test.
     */
DOC;

        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string)$docBlock);
    }

    public function testPhpDocs()
    {
        $doc = <<<DOC
/**
 * @param string \$foo
 * @return string
 */
DOC;
        $expected = <<<DOC
    /**
     * @param string \$foo
     * @return string
     */
DOC;
        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string)$docBlock);
    }

    public function testPhpDocWithDescription()
    {
        $doc = <<<DOC
/**
 * Hello world
 * @return string
 */
DOC;
        $expected = <<<DOC
    /**
     * Hello world
     *
     * @return string
     */
DOC;

        $docBlock = new DocBlock($doc);

        $this->assertSame($expected, (string)$docBlock);
    }
}
