namespace Example;

class Test
{
    public t_int1 =        10;
    public t_int2 =      -100;
    public t_int3 =  0xFFFFFF;
    public t_int4 = -0x000000;

    public t_double1 =    0.000001;
    public t_double1 =   -0.000001;
    public t_double1 =  909.999999;
    public t_double1 = -909.999999;

    public t_char1 = 'a';

    public t_const1 = FOOBAR;

    public function someString()
    {
        return "hello";
    }

    public function someIString()
    {
        return ~"hello";
    }
}