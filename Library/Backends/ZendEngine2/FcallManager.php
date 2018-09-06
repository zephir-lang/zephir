<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir                                                                   |
 | Copyright (c) 2013-present Zephir Team (https://zephir-lang.com/)        |
 |                                                                          |
 | This source file is subject the MIT license, that is bundled with this   |
 | package in the file LICENSE, and is available through the world-wide-web |
 | at the following url: http://zephir-lang.com/license.html                |
 +--------------------------------------------------------------------------+
 */

namespace Zephir\Backends\ZendEngine2;

use Zephir\Utils;
use Zephir\CodePrinter;
use Zephir\Fcall\FcallManagerInterface;

/**
 * Zephir\Backends\ZendEngine2\FcallManager
 *
 * @package Zephir\Backends\ZendEngine2
 */
class FcallManager implements FcallManagerInterface
{
    protected $requiredMacros = [];

    public function macroIsRequired($macro)
    {
        return isset($this->requiredMacros[$macro]);
    }

    /**
     * {@inheritdoc}
     *
     * @param bool $static
     * @param int $doReturn tri-state: 0 -> no return value, 1 -> do return, 2 -> do return to given variable
     * @param int $paramCount
     * @return string
     */
    public function getMacro($static, $doReturn, $paramCount)
    {
        /*
        $scopes = array('', 'STATIC');
        $modes = array('CALL_INTERNAL_METHOD_P', 'RETURN_CALL_INTERNAL_METHOD_P', 'CALL_INTERNAL_METHOD_NORETURN_P');
        */
        $scope = $static ? 'STATIC' : '';
        $mode = 'CALL_INTERNAL_METHOD_NORETURN_P';
        if ($doReturn) {
            $mode = 'RETURN_CALL_INTERNAL_METHOD_P';
            if ($doReturn === 2) {
                $mode = 'CALL_INTERNAL_METHOD_P';
            }
        }
        $macroName = 'ZEPHIR_' . ($scope ? $scope . '_' : '') . $mode . $paramCount;
        if (!$this->macroIsRequired($macroName)) {
            $this->requiredMacros[$macroName] = array($scope, $mode, $paramCount);
        }
        return $macroName;
    }

    public function genFcallCode()
    {
        $codePrinter = new CodePrinter();
        $codePrinter->output('#ifndef ZEPHIR_KERNEL_FCALL_INTERNAL_H');
        $codePrinter->output('#define ZEPHIR_KERNEL_FCALL_INTERNAL_H');
        $codePrinter->increaseLevel();

        ksort($this->requiredMacros);
        foreach ($this->requiredMacros as $name => $info) {
            list($scope, $mode, $paramCount) = $info;
            $paramsStr = '';
            $retParam = '';
            $retValueUsed = '0';
            $params = array();
            $zvals = array();
            $initStatements = array();
            $postStatements = array();

            for ($i = 0; $i < $paramCount; ++$i) {
                $params[] = 'p' . $i;
            }
            if ($paramCount) {
                $paramsStr = ', ' . implode(', ', $params);
            }

            if ($mode == 'CALL_INTERNAL_METHOD_P') {
                $retValueUsed = '1';
                $retParam = 'return_value_ptr';
                $initStatements[] = 'ZEPHIR_INIT_NVAR(*(return_value_ptr)); \\';
            }
            $objParam = $scope ? 'scope_ce, ' : 'object, ';
            $macroName = $name . '(' . ($retParam ? $retParam . ', ' : '') . $objParam . 'method' . $paramsStr . ')';
            $codePrinter->output('#define ' . $macroName . ' \\');
            if (!$retParam) {
                $retParam = 'return_value';
            }
            $codePrinter->increaseLevel();
            $codePrinter->output('do { \\');
            $codePrinter->increaseLevel();

            if ($mode == 'CALL_INTERNAL_METHOD_NORETURN_P') {
                $codePrinter->output('zval *rv = NULL; \\');
                $codePrinter->output('zval **rvp = &rv; \\');
                $codePrinter->output('ALLOC_INIT_ZVAL(rv); \\');
                $retParam = 'rvp';
            }

            $codePrinter->output('ZEPHIR_BACKUP_SCOPE() \\');
            $codePrinter->output('ZEPHIR_BACKUP_THIS_PTR() \\');
            if (!$scope) {
                $codePrinter->output('ZEPHIR_SET_THIS(object); \\');
                $codePrinter->output('ZEPHIR_SET_SCOPE((Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL), (Z_TYPE_P(object) == IS_OBJECT ? Z_OBJCE_P(object) : NULL)); \\');
            } else {
                $codePrinter->output('ZEPHIR_SET_THIS(NULL); \\');
                $codePrinter->output('ZEPHIR_SET_SCOPE(scope_ce, scope_ce); \\');
            }

            /* Create new zval's for parameters */
            for ($i = 0; $i < $paramCount; ++$i) {
                //$zv = '_' . $params[$i];
                //$zvals[] = $zv;
                //$initStatements[] = 'ALLOC_ZVAL(' . $zv . '); \\';
                //$initStatements[] = 'INIT_PZVAL_COPY(' . $zv . ', ' . $params[$i] . '); \\';
                //$postStatements[] = 'zval_ptr_dtor(&' . $zv . '); \\';
                $zv = $params[$i];
                $zvals[] = $zv;
                $initStatements[] = 'Z_ADDREF_P(' . $zv . '); \\';
                $postStatements[] = 'Z_DELREF_P(' . $zv . '); \\';
            }
            if ($i) {
                //$codePrinter->output('zval *' . implode(', *', $zvals) . '; \\');
            }
            foreach ($initStatements as $statement) {
                $codePrinter->output($statement);
            }
            $zvalStr = $i ? ', ' . implode(', ', $zvals) : '';
            $retExpr = '';
            if ($retParam) {
                if ($retParam == 'return_value') {
                    $retExpr = ', return_value, return_value_ptr';
                } else {
                    $retExpr = ', *' . $retParam . ', ' . $retParam;
                }
            }
            $codePrinter->output('method(0' . $retExpr . ', ' . ($scope ? 'NULL, ' : $objParam) . $retValueUsed . $zvalStr . ' TSRMLS_CC); \\');
            if ($mode == 'CALL_INTERNAL_METHOD_NORETURN_P') {
                $postStatements[] = 'zval_ptr_dtor(rvp); \\';
            }
            foreach ($postStatements as $statement) {
                $codePrinter->output($statement);
            }
            $codePrinter->output('ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \\');
            $codePrinter->output('ZEPHIR_RESTORE_THIS_PTR(); \\');

            $codePrinter->output('ZEPHIR_RESTORE_SCOPE(); \\');
            $codePrinter->decreaseLevel();
            $codePrinter->output('} while (0)');
            $codePrinter->decreaseLevel();
            $codePrinter->output('');
        }

        $codePrinter->decreaseLevel();
        $codePrinter->output("#endif");
        Utils::checkAndWriteIfNeeded($codePrinter->getOutput(), 'ext/kernel/fcall_internal.h');
    }
}
