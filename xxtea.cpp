#include "xxtea.h"

namespace
{

constexpr int32_t DELTA = 0x243f6a88;

inline int32_t
mx(int32_t z, int32_t y, int32_t sum, int32_t e, int32_t p, const std::array<int32_t, xxtea::KEY_LEN>& key)
{
    return (((z >> 5 ^ y << 2) + (y >> 3 ^ z << 4)) ^ ((sum ^ y) + (key[(p & 3) ^ e] ^ z)));
}

}

namespace xxtea
{

inline static std::vector<int32_t>
decrypt(const std::vector<int32_t>& v, int32_t n, const std::array<int32_t, KEY_LEN>& key)
{
    int32_t y;
    int32_t z;
    int32_t sum;
    int32_t p;
    int32_t e;
    int32_t rounds = 6 + 52 / n;
    std::vector<int32_t> ret{v};

    if (n > 1)
    {
        sum = rounds * DELTA;
        y = ret[0];

        while (rounds-- > 0)
        {
            e = (sum >> 2) & 3;

            for (p = n - 1; p > 0; p--)
            {
                z = ret[p - 1];
                y = ret[p] -= mx(z, y, sum, e, p, key);
            }

            z = ret[n - 1];
            y = ret[0] -= mx(z, y, sum, e, p, key);
            sum -= DELTA;
        }
    }

    return ret;
}

}
