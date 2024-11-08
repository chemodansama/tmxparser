#include "TmxPoint.h"

#include <charconv>

namespace Tmx
{
    bool Point::operator==(const Point &rhs) const
    {
        return x == rhs.x && y == rhs.y;
    }

    Point ParsePoint(std::string_view s)
    {
        Point result{ 0.0f, 0.0f };
        const auto xresult = std::from_chars(s.data(), s.data() + s.size(), result.x);
        if (xresult.ec == std::errc::invalid_argument
            || xresult.ec == std::errc::result_out_of_range)
        {
            return result;
        }

        std::from_chars(xresult.ptr + 1, s.data() + s.size(), result.y);

        return result;
    }
}
