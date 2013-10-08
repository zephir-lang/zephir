<?php

$t = new Test\Fibonnaci();

assert($t->fibInt() == 13);

assert($t->fibDouble() == 13.0);

assert($t->fibArray() === array(
  0, 1, 1, 2, 3, 5, 8, 13, 21, 34
));

assert($t->fibArray2() === array(
  0, 1, 1, 2, 3, 5, 8, 13, 21, 34
));
