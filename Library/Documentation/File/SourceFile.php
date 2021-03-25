<?php

/**
 * This file is part of the Zephir.
 *
 * (c) Phalcon Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Documentation\File;

use Zephir\ClassDefinition;
use Zephir\Documentation;

class SourceFile extends ClassFile
{
    /**
     * @var ClassDefinition
     */
    protected $class;

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getTemplateName(): string
    {
        return 'source.phtml';
    }

    /**
     * {@inheritdoc}
     *
     * @return string
     */
    public function getOutputFile(): string
    {
        return Documentation::sourceUrl($this->class);
    }
}
