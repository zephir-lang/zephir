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
use Zephir\Documentation\Annotation;
use Zephir\Documentation\Docblock;

class DocblockTest extends TestCase
{
    /** @var Docblock */
    private $docblock;

    public function setUp()
    {
        $this->docblock = new Docblock();
    }

    /** @test */
    public function shouldGetAllProperties()
    {
        $annotation1 = new Annotation('see', 'https://github.com/phalcon/zephir Zephir homepage');
        $annotation2 = new Annotation('link', 'https://github.com/phalcon/cphalcon Phalcon homepage');

        $this->docblock->setAnnotations([
            $annotation1,
            $annotation2,
        ]);
        $this->docblock->setDescription('Example description for test docblock.');
        $this->docblock->setSummary('See LICENSE.txt for more details.');

        $this->assertArraySubset(
            [
                $annotation1,
                $annotation2,
            ],
            $this->docblock->getAnnotations()
        );

        $this->assertSame([$annotation1], $this->docblock->getAnnotationsByType('see'));
        $this->assertSame([$annotation2], $this->docblock->getAnnotationsByType('link'));
        $this->assertNotSame([$annotation1], $this->docblock->getAnnotationsByType('link'));

        $this->assertCount(2, $this->docblock->getAnnotations());

        $this->assertSame('Example description for test docblock.', $this->docblock->getDescription());
        $this->assertSame('See LICENSE.txt for more details.', $this->docblock->getSummary());
    }

    /** @test */
    public function shouldAddAnnotations()
    {
        $annotation1 = new Annotation('see', 'https://github.com/phalcon/zephir Zephir homepage');
        $annotation2 = new Annotation('link', 'https://github.com/phalcon/cphalcon Phalcon homepage');
        $annotation3 = new Annotation('readme', 'https://github.com/zephir/README.md Readme');

        $this->docblock->setAnnotations([
            $annotation1,
            $annotation2,
        ]);

        $this->assertCount(2, $this->docblock->getAnnotations());
        $this->docblock->addAnnotation($annotation3);
        $this->assertCount(3, $this->docblock->getAnnotations());
    }

    /** @test */
    public function shouldGenerateDocblock()
    {
        $expected = <<<DOC
**
 * Example summary block.
 *
 *
 * Example description block.
 *
 * @see https://github.com/phalcon/zephir Zephir homepage
 * @return void
 *
DOC;
        $this->docblock->setDescription('Example description block.');
        $this->docblock->setSummary('Example summary block.');
        $this->docblock->setAnnotations([
            new Annotation('see', 'https://github.com/phalcon/zephir Zephir homepage'),
        ]);
        $this->docblock->addAnnotation(
            new Annotation('return', 'void')
        );

        $this->assertSame($expected, $this->docblock->__toString());
    }
}
