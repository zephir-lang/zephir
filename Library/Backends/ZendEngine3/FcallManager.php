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

namespace Zephir\Backends\ZendEngine3;

use Zephir\Utils;
use Zephir\CodePrinter;
use Zephir\Fcall\FcallManagerInterface;
use Zephir\Backends\ZendEngine2\FcallManager as ZE2FcallManager;

/**
 * Zephir\Backends\ZendEngine3\FcallManager
 *
 * @package Zephir\Backends\ZendEngine3
 */
class FcallManager extends ZE2FcallManager implements FcallManagerInterface
{
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
            $codePrinter->output('method(0, ' . $retParam . ', ' . ($scope ? 'NULL, ' : $objParam) . $retValueUsed . $zvalStr . '); \\');
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
