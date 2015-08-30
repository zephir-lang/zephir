<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2015 Zephir Team and contributors                          |
 +--------------------------------------------------------------------------+
 | This source file is subject the MIT license, that is bundled with        |
 | this package in the file LICENSE, and is available through the           |
 | world-wide-web at the following url:                                     |
 | http://zephir-lang.com/license.html                                      |
 |                                                                          |
 | If you did not receive a copy of the MIT license and are unable          |
 | to obtain it through the world-wide-web, please send a note to           |
 | license@zephir-lang.com so we can mail you a copy immediately.           |
 +--------------------------------------------------------------------------+
*/

namespace Zephir\Backends\ZendEngine3;

use Zephir\CodePrinter;
use Zephir\Utils;

/**
 * Class FcallManager
 *
 */
class FcallManager
{
    protected $requiredMacros = array();


    public function macroIsRequired($macro)
    {
        return isset($this->requiredMacros[$macro]);
    }

    /**
     * Resolve internal fcall attributes to a suitable macro and ensure that it's generated during compilation
     * @param doReturn tri-state: 0 -> no return value, 1 -> do return, 2 -> do return to given variable
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
                $retParam = 'return_value_ptr';
                $initStatements[] = 'ZEPHIR_INIT_NVAR((return_value_ptr)); \\';
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
                $codePrinter->output('zval rv; \\');
                $codePrinter->output('zval *rvp = &rv; \\');
                $codePrinter->output('ZVAL_UNDEF(&rv); \\');
                $retParam = 'rvp';
            }

            $codePrinter->output('ZEPHIR_BACKUP_SCOPE(); \\');
            $codePrinter->output('ZEPHIR_BACKUP_THIS_PTR(); \\');
            if (!$scope) {
                $codePrinter->output('ZEPHIR_SET_THIS(object); \\');
                $codePrinter->output('ZEPHIR_SET_SCOPE((Z_OBJ_P(object) ? Z_OBJCE_P(object) : NULL), (Z_OBJ_P(object) ? Z_OBJCE_P(object) : NULL)); \\');
            } else {
                $codePrinter->output('ZEPHIR_SET_THIS_EXPLICIT_NULL(); \\');
                $codePrinter->output('ZEPHIR_SET_SCOPE(scope_ce, scope_ce); \\');
            }

            /* Create new zval's for parameters */
            for ($i = 0; $i < $paramCount; ++$i) {
                $zv = '_' . $params[$i];
                $zvals[] = $zv;
                $initStatements[] = 'ZVAL_COPY(&' . $zv . ', ' . $params[$i] . '); \\';
                $postStatements[] = 'Z_TRY_DELREF_P(' . $params[$i] . '); \\';
                //$postStatements[] = 'zval_ptr_dtor(' . $params[$i] . '); \\';
            }
            if ($i) {
                $codePrinter->output('zval ' . implode(', ', $zvals) . '; \\');
            }
            foreach ($initStatements as $statement) {
                $codePrinter->output($statement);
            }
            $zvalStr = $i ? ', &' . implode(', &', $zvals) : '';
            $codePrinter->output('method(0, ' . $retParam . ', ' . ($scope ? 'NULL, ' : $objParam) . '1' . $zvalStr . '); \\');
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

        /*#define ZEPHIR_CALL_INTERNAL_METHOD_NORETURN_P0(object, method) \
        do { \
            ZEPHIR_BACKUP_THIS_PTR(); \
            zval rv; \
            zval *rvp = &rv; \
            ZVAL_UNDEF(&rv); \
            ZEPHIR_SET_THIS(object); \
            method(0, rvp, object, 0); \
            ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
            ZEPHIR_RESTORE_THIS_PTR(); \
            zval_ptr_dtor(rvp); \
        } while (0)*/

        /*#define ZEPHIR_CALL_INTERNAL_METHOD_P0(return_value_ptr, object, method) \
        do { \
            ZEPHIR_BACKUP_THIS_PTR(); \
            ZEPHIR_SET_THIS(object); \
            ZEPHIR_INIT_NVAR(return_value_ptr); \
            method(0, return_value_ptr, object, 1); \
            ZEPHIR_LAST_CALL_STATUS = EG(exception) ? FAILURE : SUCCESS; \
            ZEPHIR_RESTORE_THIS_PTR(); \
        } while (0)*/

        $codePrinter->decreaseLevel();
        $codePrinter->output("#endif");
        Utils::checkAndWriteIfNeeded($codePrinter->getOutput(), 'ext/kernel/fcall_internal.h');
    }

    /**
     * Obtains the existing concatenation keys
     *
     * @return array
     */
    public function getConcatKeys()
    {
        return $this->concatKeys;
    }
}
