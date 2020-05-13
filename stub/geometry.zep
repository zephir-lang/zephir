
namespace Stub;

class Geometry
{
    public static function run(array list, int count) -> void
    {
        int i = 0;

        while (i < count) {
            static::distanceStatic(list[i][0], list[i][1], list[i][2], list[i][3]);
            let i = i + 1;
        }
    }

    public static function runOptimize(array list, int count) -> void
    {
        int i = 0;
        var item;

        while (i < count) {
            let item = list[i];

            static::distanceStatic(item[0], item[1], item[2], item[3]);

            let i++;
        }
    }


    public static function distanceStatic(float x1, float y1, float x2, float y2) -> double
    {
        return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    }
}
