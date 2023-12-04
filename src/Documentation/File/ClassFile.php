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

namespace Zephir\Documentation\File;

use Zephir\Class\Definition\Definition;
use Zephir\CompilerFile;
use Zephir\Documentation;
use Zephir\Documentation\FileInterface;

use function explode;

class ClassFile implements FileInterface
{
    protected Definition $class;

    public function __construct(protected CompilerFile $compilerFile)
    {
        $this->class = $compilerFile->getClassDefinition();
    }

    public function getData(): array
    {
        $nsPieces = explode('\\', $this->class->getNamespace());

        $nsPatches = [];
        $nsStr     = '';

        foreach ($nsPieces as $n) {
            if ($nsStr !== '') {
                $nsStr .= '\\';
            }
            $nsStr         .= $n;
            $nsPatches[$n] = $nsStr;
        }

        return [
            'classDefinition' => $this->class,
            'compilerFile'    => $this->compilerFile,
            'className'       => $this->class->getName(),
            'classNamespace'  => $this->class->getNamespace(),
            'fullName'        => $this->class->getCompleteName(),
            'methods'         => $this->class->getMethods(),
            'namespacePieces' => $nsPatches,
        ];
    }

    public function getOutputFile(): string
    {
        return Documentation::classUrl($this->class);
    }

    public function getTemplateName(): string
    {
        return 'class.phtml';
    }
}
