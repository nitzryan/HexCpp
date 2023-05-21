#include "Template.h"

Template::Template(Chain *a, Chain *b)
{
    one = a;
    two = b;
    UpdateAdjacencies();
}

void Template::UpdateAdjacencies()
{
    sharedAdjacentTiles = BitArray::And(one->GetAdjacencies(), two->GetAdjacencies());
}

bool Template::IsBroken() const
{
    return broken;
}

void Template::CheckToBreak(short move)
{
    sharedAdjacentTiles.UnsetBit(move);
    broken = !sharedAdjacentTiles.MoreThanOneBit();
}

Chain* Template::GetOtherChain(const Chain* chain) const {
    if (chain == one) {
        return two;
    }
    if (chain == two) {
        return one;
    }
    return nullptr;
}

bool Template::ContainsChain(const Chain* chain) const
{
    return chain == one || chain == two;
}

void Template::Remap(const Chain* orig, const Chain* newer) {
    if (one == orig) {
        one = (Chain*)newer;
        return;
    }
    if (two == orig) {
        two = (Chain*)newer;
    }
}

bool Template::SameTemplate(const Template* t1, const Template* t2)
{
    return (t1->one == t2->one) && (t1->two == t2->two);
}

bool Template::IsATemplate(const Chain* c1, const Chain* c2)
{
    return BitArray::And(c1->GetAdjacencies(), c2->GetAdjacencies()).MoreThanOneBit();
}
