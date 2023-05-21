#ifndef TEMPLATE_H
#define TEMPLATE_H

#include "BitArray.h"
#include "Chain.h"

class Template
{
public:
    Template(Chain* a, Chain* b);
    void UpdateAdjacencies();
    void SetVisited(bool v) { visited = v; }
    bool IsBroken() const;
    void CheckToBreak(short move);
    Chain* GetOtherChain(const Chain* chain) const;
    bool ContainsChain(const Chain* chain) const;
    void Remap(const Chain* orig, const Chain* newer);
    void GetChains(const Chain** a, const Chain** b) { *a = one; *b = two; }
    static bool SameTemplate(const Template* t1, const Template* t2);
    static bool IsATemplate(const Chain* c1, const Chain* c2);
private:
    bool broken = false; // Causes it to not immedately go away, so that AI doesn't think breaking a Template
    // is a good move, but that it will go away before the response so that the ai will likely connect them
    bool visited = false; // Used to track if chain was visited to attempt to calculate the largest chain
    Chain* one;
    Chain* two;
    BitArray sharedAdjacentTiles;
};

#endif // TEMPLATE_H
