<?php

/*
 +----------------------------------------------------------------------+
 | Zephir Language                                                      |
 +----------------------------------------------------------------------+
 | Copyright (c) 2013-2017 Zephir Team                                  |
 +----------------------------------------------------------------------+
 | This source file is subject to version 1.0 of the MIT license,       |
 | that is bundled with this package in the file LICENSE, and is        |
 | available through the world-wide-web at the following url:           |
 | http://www.zephir-lang.com/license                                   |
 |                                                                      |
 | If you did not receive a copy of the MIT license and are unable      |
 | to obtain it through the world-wide-web, please send a note to       |
 | license@zephir-lang.com so we can mail you a copy immediately.       |
 +----------------------------------------------------------------------+
*/

namespace Zephir;

/**
 * FunctionDefinition
 *
 * Represents a function (method)
 */
class FunctionDefinition extends ClassMethod
{
    /**
     * The namespace of the function
     */
    private $namespace;

    /**
     * Whether the function is declared in a global or namespaced scope
     *
     * @var boolean
     */
    private $isGlobal;

    public function __construct($namespace, $name, $parameters, StatementsBlock $statements = null, $returnType = null, array $original = null)
    {
        $this->namespace = $namespace;
        $this->name = $name;
        $this->parameters = $parameters;
        $this->statements = $statements;
        $this->expression = $original;

        if ($returnType['void']) {
            $this->void = true;
            return;
        }

        if (isset($returnType['list'])) {
            $types = array();
            $castTypes = array();
            foreach ($returnType['list'] as $returnTypeItem) {
                if (isset($returnTypeItem['cast'])) {
                    if (isset($returnTypeItem['cast']['collection'])) {
                        continue;
                    }
                    $castTypes[$returnTypeItem['cast']['value']] = $returnTypeItem['cast']['value'];
                } else {
                    $types[$returnTypeItem['data-type']] = $returnTypeItem;
                }
            }
            if (count($castTypes)) {
                $types['object'] = array();
                $this->returnClassTypes = $castTypes;
            }
            if (count($types)) {
                $this->returnTypes = $types;
            }
        }
    }

    /**
     * Get the internal name used in generated C code
     */
    public function getInternalName()
    {
        return ($this->isGlobal() ? 'g_' : 'f_') . str_replace('\\', '_', $this->namespace) . '_' . $this->getName();
    }

    public function isGlobal()
    {
        return $this->isGlobal;
    }

    public function setGlobal($global)
    {
        $this->isGlobal = $global;
    }

    public function getNamespace()
    {
        return $this->namespace;
    }

    public function setNamespace($namespace)
    {
        $this->namespace = $namespace;
    }

    public function hasModifier($modifier)
    {
        return false;
    }

    public function getVisibility()
    {
        return array();
    }
}
