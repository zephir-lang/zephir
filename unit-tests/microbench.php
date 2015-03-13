<?php

/*
 +--------------------------------------------------------------------------+
 | Zephir Language                                                          |
 +--------------------------------------------------------------------------+
 | Copyright (c) 2013-2015 Zephir Team and contributors                     |
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

require __DIR__ . '/../bootstrap.php';

$total = 0;

function start_test()
{
    ob_start();
    return microtime(true);
}

function end_test($start, $name, $overhead = null)
{
    global $total;
    global $last_time;
    $end = microtime(true);
    ob_end_clean();
    $last_time = $end-$start;
    $total += $last_time;
    $num = number_format($last_time, 3);
    $pad = str_repeat(" ", 24-strlen($name)-strlen($num));
    if (is_null($overhead)) {
        echo $name.$pad.$num."\n";
    } else {
        $num2 = number_format($last_time - $overhead, 3);
        echo $name.$pad.$num."    ".$num2."\n";
    }
    ob_start();
    return microtime(true);
}

function total()
{
    global $total;
    $pad = str_repeat("-", 24);
    echo $pad."\n";
    $num = number_format($total, 3);
    $pad = str_repeat(" ", 24-strlen("Total")-strlen($num));
    echo "Total".$pad.$num."\n";
}

const N = 5000000;

echo "Benchmark Zephir ".\Zephir\Compiler::VERSION." \n";

$t = start_test();
$x = new \Test\Bench\Foo();
$x->empty_for_in_range(N);
$t = end_test($t, 'empty for in range');
$x->read_prop(N);
$t = end_test($t, '$x = $this->x');
$x->write_prop(N);
$t = end_test($t, '$this->x = 0');
$x->assign_add_prop(N);
$t = end_test($t, '$this->x += 2');
//$x->pre_inc_prop(N);
//$t = end_test($t, '++$this->x');
//$x->pre_dec_prop(N);
//$t = end_test($t, '--$this->x');
$x->post_inc_prop(N);
$t = end_test($t, '$this->x++');
$x->post_dec_prop(N);
$t = end_test($t, '$this->x--');
$x->isset_prop(N);
$t = end_test($t, 'isset($this->x)');
$x->empty_prop(N);
$t = end_test($t, 'empty($this->x)');
$x->call(N);
$t = end_test($t, '$this->f()');
$x->read_const(N);
$t = end_test($t, '$x = Foo::TEST');
total();
