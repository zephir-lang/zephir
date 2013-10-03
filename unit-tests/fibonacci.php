<?php

$t = new Test\Fibonnaci();

assert($t->fibInt() == 13);

assert($t->fibDouble() == 13.0);

assert($t->fibArray() === array(
  1, 1, 2, 3, 5, 8, 13, 21, 34, 55
));

assert($t->fibArray2() === array(
  1, 1, 2, 3, 5, 8, 13, 21, 34, 55
));
