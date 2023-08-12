#ifndef BITARRAY_H
#define BITARRAY_H

#include <cstdint>
#include <vector>

class BitArray
{
public:
    /**
     * @brief BitArray
     * Needs to be initialized using an operation
     */
    BitArray();
    /**
     * @brief Copy Constructor
     */
    BitArray(const BitArray* ba);
    /**
     * @brief BitArray
     * Initialized as all 0
     * @param numTiles Number of bits required
     */
    BitArray(int numTiles);
    /**
     * @brief BitArray
     * @param numTiles
     * @param tile
     */
    BitArray(int numTiles, short tile);
    /**
     * @brief BitArray
     * @param numTiles
     * @param tiles
     */
    BitArray(int numTiles, const std::vector<short> tiles);
    /**
     * @brief ~BitArray
     */
    ~BitArray();

    // Bitwise Operations
    /**
     * @brief And
     * @param a
     * @param b
     * @param result
     */
    static BitArray And(const BitArray* a, const BitArray* b);
    /**
     * @brief Or
     * @param a
     * @param b
     * @param result
     */
    static BitArray Or(const BitArray* a, const BitArray* b);
    /**
     * @brief Not
     * @param a
     * @param result
     */
    static BitArray Not(const BitArray* a);
    /**
     * @brief ExactlyOneBit
     * @return
     */
    bool MoreThanOneBit() const;
    /**
     * @brief AtLeaseOneBit
     * @return
     */
    bool AtLeaseOneBit() const;
    /**
    * @brief UnsetBit
    * @param bit Bit to set to 0
    */
    void UnsetBit(short bit);
    /**
    * @brief SetBit
    * @param bit Bit to set to 1
    */
    void SetBit(short bit);
    /**
    * @brief Equal
    * @return 
    */
    static bool Equal(const BitArray* a, const BitArray* b);
    bool BitIsSet(short bit) const;
private:
    std::vector<uint32_t> bits;

};

#endif // BITARRAY_H
