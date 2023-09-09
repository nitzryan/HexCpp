#include "HBTMethods.h"
#include <functional>

void HBTMethods::CreateChain(short move, short size, bool isRed, std::vector<Chain>& chains, const std::vector<Chain>& oldChains, std::vector<Template>& templates, std::vector<Edge>& edges, std::vector<SpecialEdge>& sEdges, BitArray& placedTiles, BitArray& sameTiles, const BitArray& oppTiles, const HexBoardHelper* helper)
{
	Chain chain = Chain(move, size, isRed, &placedTiles);
	short seRank = chain.GetMaxRank() > size / 2 ? size - 1 : 0;
	placedTiles.SetBit(move);
	sameTiles.SetBit(move);

	// Check which chains should be with the new chain, and delete those chains
	for (auto& i : chains)
		if (chain.ShouldMerge(i))
			chain.MergeWith(&i); // Merge with will update chain, mark i as needing deletion

	std::vector<bool> didDelete(chains.size()); // Used to tell which templates/special edges need to be updated
	for (size_t i = 0; i < chains.size(); i++)
		didDelete[i] = chains.at(i).ShouldDelete();

	chains.erase(std::remove_if(chains.begin(), chains.end(), [](const Chain& c) {
		return c.ShouldDelete();
		}), chains.end());

	// Delete Templates, Edges that were removed
	// Special Edges are not removed
	for (size_t i = 0; i < oldChains.size(); i++)
	{
		if (didDelete[i])
		{
			for (int j = (int)templates.size() - 1; j >= 0; j--)
			{
				if (templates.at(j).ContainsChain(&oldChains[i]))
					templates.erase(templates.begin() + j);
			}
			for (int j = (int)edges.size() - 1; j >= 0; j--)
			{
				if (&oldChains[i] == edges[j].GetChain())
					edges.erase(edges.begin() + j);
			}
		}
	}

	

	// Add new chain, remap special edges
	chains.push_back(chain);
	for (size_t i = 0; i < oldChains.size(); i++)
		if (didDelete[i])
			for (auto& j : sEdges)
				j.RemapAddresses(&oldChains[i], &chains.back());

	// Remap Edges and Templates
	size_t idx = 0;
	for (size_t i = 0; i < oldChains.size(); i++)
	{
		if (!didDelete[i])
		{
			for (auto& j : templates)
				j.Remap(&oldChains[i], &chains[idx]);
			for (auto& j : edges)
				j.RemapAddresses(&oldChains[i], &chains[idx]);
			for (auto& j : sEdges)
				j.RemapAddresses(&oldChains[i], &chains[idx]);

			idx++;
		}
	}

	// Check new chain for templates, edges, and special edges
	for (size_t i = 0; i < chains.size() - 1; i++)
		if (Template::IsATemplate(&chains.back(), &chains[i]))
			templates.emplace_back(&chains.back(), &chains[i]);

	if (Edge::ShouldBeEdge(&chains.back(), (char)0, helper))
		edges.emplace_back(&chains.back(), (char)0, isRed);
	if (Edge::ShouldBeEdge(&chains.back(), (char)(size - 1), helper))
		edges.emplace_back(&chains.back(), (char)(size - 1), isRed);

	if (helper->IsEdge1(move, isRed, &oppTiles))
		sEdges.emplace_back(&chains.back(), move, seRank, helper, &oppTiles);
	
}

void HBTMethods::RemapAddresses(std::vector<Template>& templates, std::vector<Edge>& edges, std::vector<SpecialEdge>& sEdges, const std::vector<Chain>& oldChains, const std::vector<Chain>& newChains)
{
	for (size_t i = 0; i < oldChains.size(); i++)
	{
		for (auto& j : templates)
			j.Remap(&oldChains[i], &newChains[i]);
		for (auto& j : edges)
			j.RemapAddresses(&oldChains[i], &newChains[i]);
		for (auto& j : sEdges)
			j.RemapAddresses(&oldChains[i], &newChains[i]);
	}
}

void HBTMethods::ApplyMoveToOpponentStructures(short move, std::vector<Chain>& chains, std::vector<Template>& templates, std::vector<SpecialEdge>& sEdges)
{
	for (auto& i : chains)
		i.OpponentTilePlaced(move);
	for (auto& i : templates)
		i.CheckToBreak(move);
	for (auto& i : sEdges)
		i.OpponentMoveMade(move);
}

void HBTMethods::PoisonUpdateTilesRanks(const Poison& poison, short size, bool isRed, const HexBoardHelper* helper, std::vector<Chain>& chains, const std::vector<Template>& oTemplates, const std::vector<Edge>& oEdges, const std::vector<SpecialEdge>& osEdges)
{
	// Create temp poison, add templates/edges to
	// They only effect when they exist, and go away when active.  Poison class doesn't support removal
	Poison p = HBTMethods::ApplyStructuresToPoison(poison, size, isRed, helper, oTemplates, oEdges, osEdges);

	// Modify ranks of existing chains for current poison
	for (auto& i : chains)
		i.ApplyPoison(p.GetMax(), p.GetMin(), helper);
}

Poison HBTMethods::ApplyStructuresToPoison(const Poison& poison, short size, bool isRed, const HexBoardHelper* helper, const std::vector<Template>& templates, const std::vector<Edge>& edges, const std::vector<SpecialEdge>& sEdges)
{
	Poison p = Poison(poison);

	// Apply templates
	for (auto& i : templates)
		p.PlaceTiles(i.GetTiles(), size, isRed);
	for (auto& i : edges)
	{
		auto tmp = i.GetTiles(helper);
		p.PlaceTiles(&tmp, size, isRed);
	}
	for (auto& i : sEdges)
	{
		auto tmp = i.GetTiles();
		p.PlaceTiles(&tmp, size, isRed);
	}

	return p;
}

int HBTMethods::GetLongestChainLength(const std::vector<Chain>& chains)
{
	int longest = 0;
	for (auto& i : chains)
	{
		auto val = i.GetChainLength();
		if (val > longest)
			longest = val;
	}

	return longest;
}

int HBTMethods::GetLongestTemplateLength(std::vector<Chain>& chains, std::vector<Template>& templates, std::vector<Edge>& edges, std::vector<SpecialEdge>& sEdges)
{
	for (auto& i : chains)
		i.SetTraversed(false);

	for (auto& i : templates)
		i.SetVisited(false);

	std::function<void(int&, int&, Chain*, std::vector<Template>&, std::vector<Edge>&, std::vector<SpecialEdge>&)> getRank =
		[&](int& min, int& max, Chain* chain, std::vector<Template>& templates, std::vector<Edge>& edges, std::vector<SpecialEdge>& sEdges) {
		if (chain->Traversed()) {
			return;
		}
		// Update Min/Max to include this chain
		auto minr = chain->GetMinRank();
		auto maxr = chain->GetMaxRank();
		min = (minr < min) ? minr : min;
		max = (maxr > max) ? maxr : max;
		chain->SetTraversed(true);
		// Recursively traverse templates
		for (auto& t : templates) {
			if (!t.IsVisited())
			{
				Chain* otherChain = t.GetOtherChain(chain);
				if (otherChain != nullptr && !otherChain->Traversed()) {
					getRank(min, max, otherChain, templates, edges, sEdges);
				}
			}
		}
		// Check if edge increases or decreases rank
		for (auto& e : edges) {
			if (e.GetChain() == chain) {
				auto rank = e.GetRank();
				if (rank == 0) {
					min = 0;
				}
				else {
					max = rank;
				}
			}
		}

		for (auto& se : sEdges) {
			if (se.GetChain() == chain) {
				auto rank = se.GetRank();
				if (rank == 0)
					min = 0;
				else
					max = rank;
			}
		}
	};

	// Iterate through chains to find longest template
	int longest = 0;
	for (auto& i : chains) {
		int minRank = std::numeric_limits<int>::max();
		int maxRank = -1;
		getRank(minRank, maxRank, &i, templates, edges, sEdges);
		auto rankDif = maxRank - minRank;
		longest = (rankDif > longest) ? rankDif : longest;
	}
	return longest;
}

void HBTMethods::RemoveBrokenTemplates(std::vector<Template>& templates, std::vector<Edge>& edges, std::vector<SpecialEdge>& sEdges, const HexBoardHelper* helper)
{
	templates.erase(std::remove_if(templates.begin(), templates.end(), [](const Template& t) {
		return t.IsBroken();
		}), templates.end());

	edges.erase(std::remove_if(edges.begin(), edges.end(), [helper](const Edge& e) {
		return !e.IsStillEdge(helper);
		}), edges.end());

	sEdges.erase(std::remove_if(sEdges.begin(), sEdges.end(), [](const SpecialEdge& se) {
		return se.IsBroken();
		}), sEdges.end());
}

float HBTMethods::GetScore(int redChainLength, int blueChainLength, int redTemplateLength, int blueTemplateLength, float& centerScore, short move, short size, bool isRed, const BitArray& , const BitArray& , const HexWeights* weights)
{
	if (redChainLength == size - 1)
		return 1000;
	if (blueChainLength == size - 1)
		return -1000;

	// Apply center score for next move
	auto halfSize = size / 2;
	auto row = move / size;
	auto col = move % size;
	int rowPlusColOffset = (row < halfSize && col > halfSize) || (row > halfSize && col < halfSize) ?
		std::max(std::abs(row - halfSize), std::abs(col - halfSize)) :
		std::abs(row - halfSize) + std::abs(col - halfSize);
	
	if (isRed) {
		centerScore -= rowPlusColOffset * weights->CenterPenalty;
	}
	else {
		centerScore += rowPlusColOffset * weights->CenterPenalty;
	}

	float score = centerScore;

	score += weights->ChainLengthFactor[redChainLength];
	score -= weights->ChainLengthFactor[blueChainLength];
	score += weights->TemplateLengthFactor[redTemplateLength];
	score -= weights->TemplateLengthFactor[blueTemplateLength];

	if (isRed)
		score -= weights->TempoBias;

	return score;
}
