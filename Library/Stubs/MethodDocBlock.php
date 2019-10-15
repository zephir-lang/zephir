<?php

/*
 * This file is part of the Zephir.
 *
 * (c) Zephir Team <team@zephir-lang.com>
 *
 * For the full copyright and license information, please view
 * the LICENSE file that was distributed with this source code.
 */

namespace Zephir\Stubs;

use Zephir\AliasManager;
use Zephir\ClassMethod;

/**
 * Stubs Generator.
 *
 * @todo: Merge class with documentation generator
 */
class MethodDocBlock extends DocBlock
{
    private $parameters = [];

    private $return;

    private $shortcutName = '';

    private $deprecated = false;

    /**
     * @var AliasManager
     */
    private $aliasManager;

    public function __construct(ClassMethod $method, AliasManager $aliasManager, $indent = '    ')
    {
        parent::__construct($method->getDocBlock(), $indent);

        $this->deprecated = $method->isDeprecated();
        $this->aliasManager = $aliasManager;
        $this->shortcutName = $method->isShortcut() ? $method->getShortcutName() : '';
        $this->classMethod = $method;
    }

    /**
     * Process DocBlock and Method arguments
     *
     * @return string
     */
    public function processMethodDocBlock()
    {
        $this->parseMethodParameters($this->classMethod);
        $this->parseLines();
        $this->parseMethodReturnType($this->classMethod);
        $this->appendParametersLines();

        if (!empty($this->return)) {
            $this->appendReturnLine();
        }

        return $this->__toString();
    }

    protected function parseMethodReturnType(ClassMethod $method)
    {
        $return = [];
        $returnTypes = $method->getReturnTypes();

        if ($returnTypes) {
            foreach ($returnTypes as $type) {
                if (isset($type['data-type'])) {
                    $return[] = 'variable' == $type['data-type'] ? 'mixed' : $type['data-type'];
                }
            }
        }

        $returnClassTypes = $method->getReturnClassTypes();
        if ($returnClassTypes) {
            foreach ($returnClassTypes as $key => $returnClassType) {
                if ($this->aliasManager->isAlias($returnClassType)) {
                    $returnClassTypes[$key] = '\\'.$this->aliasManager->getAlias($returnClassType);
                }
            }

            $return = array_merge($return, $returnClassTypes);
        }

        if ($method->hasReturnTypesRaw()) {
            $returnClassTypes = $method->getReturnTypesRaw();

            if (!empty($returnClassTypes['list'])) {
                foreach ($returnClassTypes['list'] as $returnType) {
                    if (empty($returnType['cast']) || !$returnType['collection']) {
                        continue;
                    }

                    $key = $returnType['cast']['value'];
                    $type = $key;

                    if ($this->aliasManager->isAlias($type)) {
                        $type = '\\'.$this->aliasManager->getAlias($type);
                    }

                    $return[$key] = $type.'[]';
                }
            }
        }

        if (!empty($return)) {
            $this->return = [implode('|', $return), ''];
        }
    }

    protected function parseLines()
    {
        $lines = [];

        foreach ($this->lines as $line) {
            if (0 === preg_match('#^@(param|return|var) +(.*)$#', $line, $matches)) {
                $lines[] = $line;
            } else {
                list(, $docType, $tokens) = $matches;

                $tokens = preg_split('/\s+/', $tokens, 3);
                $type = $tokens[0];

                if ('var' == $docType && 'set' == $this->shortcutName) {
                    $docType = 'param';
                    $name = array_keys($this->parameters);
                    $name = $name[0];
                } elseif ('var' == $docType && 'get' == $this->shortcutName) {
                    $docType = 'return';
                } else {
                    $name = isset($tokens[1]) ? '$'.trim($tokens[1], '$') : '';
                }

                // TODO: there must be a better way
                if (0 === strpos($type, 'Phalcon\\')) {
                    $type = str_replace('Phalcon\\', '\Phalcon\\', $type);
                }

                $description = isset($tokens[2]) ? $tokens[2] : '';

                switch ($docType) {
                    case 'param':
                        $this->parameters[$name] = [$type, $description];
                        break;
                    case 'return':
                        $this->return = [$type, $description];
                        break;
                }
            }
        }

        $this->lines = $lines;
    }

    private function appendReturnLine()
    {
        list($type, $description) = $this->return;

        $return = $type.' '.$description;
        $this->lines[] = '@return '.trim($return, ' ');
    }

    private function parseMethodParameters(ClassMethod $method)
    {
        $parameters = $method->getParameters();
        $aliasManager = $method->getClassDefinition()->getAliasManager();

        if (!$parameters) {
            return;
        }

        foreach ($method->getParameters() as $parameter) {
            if (isset($parameter['cast'])) {
                if ($aliasManager->isAlias($parameter['cast']['value'])) {
                    $type = '\\'.$aliasManager->getAlias($parameter['cast']['value']);
                } else {
                    $type = $parameter['cast']['value'];
                }
            } elseif (isset($parameter['data-type'])) {
                if ('variable' == $parameter['data-type']) {
                    $type = 'mixed';
                } else {
                    $type = $parameter['data-type'];
                }
            } else {
                $type = 'mixed';
            }
            $this->parameters['$'.trim($parameter['name'], '$')] = [$type, ''];
        }
    }

    private function appendParametersLines()
    {
        foreach ($this->parameters as $name => $parameter) {
            list($type, $description) = $parameter;

            $param = $type.' '.$name.' '.$description;
            $this->lines[] = '@param '.trim($param, ' ');
        }

        if ($this->deprecated) {
            $this->lines[] = '@deprecated';
        }

        $this->lines = array_unique($this->lines);
    }
}
