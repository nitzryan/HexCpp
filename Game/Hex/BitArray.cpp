#include "BitArray.h"

#include <cmath>

#define INT_SIZE 32

BitArray::BitArray()
{
    // Intentionally don't initialize bits, as it will be initialized elsewhere
}

BitArray::BitArray(const BitArray* ba)
{
    bits = ba->bits;
}

BitArray::BitArray(int numTiles)
{
    bits = std::vector<uint32_t>(static_cast<int>(std::ceil(static_cast<float>(numTiles) / 32.0f)));
    for (auto& i : bits) {
        i = 0;
    }
}

BitArray::BitArray(int numTiles, short tile)
{
    bits = std::vector<uint32_t>(static_cast<int>(std::ceil(static_cast<float>(numTiles) / 32.0f)));
    for (auto& i : bits) {
        i = 0;
    }

    bits[tile/32] |= (1 << (tile % 32));
}

BitArray::BitArray(int numTiles, const std::vector<short> tiles)
{
    bits = std::vector<uint32_t>(static_cast<int>(std::ceil(static_cast<float>(numTiles) / 32.0f)));
    for (auto& i : bits) {
        i = 0;
    }

    for (auto tile : tiles) {
        bits[tile/32] |= (1 << (tile % 32));
    }
}

BitArray::~BitArray()
{

}

BitArray BitArray::And(const BitArray* a, const BitArray* b)
{
    BitArray result;
    result.bits = a->bits;
    for (size_t i = 0; i < result.bits.size(); i++) {
        result.bits.at(i) &= b->bits.at(i);
    }
    return result;
}

BitArray BitArray::Or(const BitArray* a, const BitArray* b)
{
    BitArray result;
    result.bits = a->bits;
    for (size_t i = 0; i < result.bits.size(); i++) {
        result.bits.at(i) |= b->bits.at(i);
    }
    return result;
}

BitArray BitArray::Not(const BitArray* a)
{
    BitArray result;
    result.bits = std::vector<uint32_t>(a->bits.size());
    for (size_t i = 0; i < result.bits.size(); i++) {
        result.bits.at(i) = ~a->bits.at(i);
    }
    return result;
}

bool BitArray::MoreThanOneBit() const
{
    uint32_t bitCount = 0;
    for (auto i : bits) {
        bitCount += (i != 0) * (!((i & (i - 1)) == 0) + 1); // 0 if 0, 1 if power of 2, 2 otherwise
    }
    return bitCount > 1;
}

bool BitArray::AtLeaseOneBit() const
{
    for (auto i : bits) {
        if (i != 0) {
            return true;
        }
    }
    return false;
}

void BitArray::UnsetBit(short bit)
{
    int div = bit / INT_SIZE;
    int rem = bit % INT_SIZE;
    bits[div] &= ~(1 << rem);
}

void BitArray::SetBit(short bit)
{
    int div = bit / INT_SIZE;
    int rem = bit % INT_SIZE;
    bits[div] |= (1 << rem);
}

bool BitArray::Equal(const BitArray* a, const BitArray* b)
{
    if (a->bits.size() != b->bits.size()) {
        return false;
    }
    bool equals = true;

    for (size_t i = 0; i < a->bits.size(); i++) {
        equals = equals && (a->bits[i] == b->bits[i]);
    }
    return equals;
}

bool BitArray::BitIsSet(short bit) const
{
    return bits[bit/32] & (1 << (bit % 32));
}
