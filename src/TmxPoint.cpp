#include "TmxPoint.h"

#include <cassert>
#include <cctype>

namespace Tmx
{
    namespace
    {
        size_t SkipWhitespaces(std::string_view s)
        {
            size_t result{ 0 };

            const auto count = s.size();
            while (result < count)
            {
                const auto c = s[result];
                if (!std::isspace(c))
                {
                    break;
                }

                result += 1;
            }

            return result;
        }

        float ParseFloat(std::string_view s, size_t *pos)
        {
            int divisor{ 1 };
            float value{ 0.0f };
            bool flush{ false };
            float sign{ 1.0f };
            const auto count = s.size();

            *pos = SkipWhitespaces(s);

            for (; *pos < count; (*pos)++)
            {
                const auto c = s[*pos];
                if ('0' <= c && c <= '9')
                {
                    const auto digit = static_cast<float>(c - '0');
                    if (divisor == 1)
                    {
                        value = value * 10 + digit;
                    }
                    else
                    {
                        value = value + digit / static_cast<float>(divisor);
                        divisor *= 10;
                    }
                    flush = true;
                }
                else if (c == '.')
                {
                    assert(divisor == 1);
                    divisor = 10;
                    flush = true;
                }
                else if (c == '-')
                {
                    assert(sign == 1.0f);
                    assert(value == 0.0f);
                    assert(!flush);
                    sign = -1.0f;
                }
                else
                {
                    if (flush)
                    {
                        return sign * value;
                    }
                    value = 0.0f;
                    divisor = 1;
                    flush = false;
                    sign = 1.0f;
                }
            }

            if (flush)
            {
                return sign * value;
            }

            return 0.0f;
        }
    }

    Point ParsePoint(std::string_view s)
    {
        Point result{ 0.0f, 0.0f };
        size_t pos;
        result.x = ParseFloat(s, &pos);
        result.y = ParseFloat({ s.begin() + pos + 1, s.end() }, &pos);
        return result;
    }
}
