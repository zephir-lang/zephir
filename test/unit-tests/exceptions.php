<?php

$t = new Test\Exceptions();

try { $t->testException1(); } catch (Exception $e) { assert($e->getMessage() == 'hello1'); }
try { $t->testException2(); } catch (Exception $e) { assert($e->getMessage() == 'hello2'); }
try { $t->testException3(); } catch (Exception $e) { assert($e->getMessage() == 'hello3'); }
try { $t->testException4(); } catch (Exception $e) { assert($e->getMessage() == 'hello4'); }