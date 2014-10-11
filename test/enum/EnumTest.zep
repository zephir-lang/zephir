namespace Test\Enum;

class EnumTest
{
    enum empty_enum {}

    enum enum_with_id {
        bar_const
    }

    enum enum_with_default_value {
        foo_const = 1
    }

    enum enum_with_ids {
        baz_const,
        enum_const
    }

}
