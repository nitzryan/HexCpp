#pragma once

#include "Chain.h"
#include "Template.h"
#include "Edge.h"
#include "SpecialEdge.h"
#include "Poison.h"
#include "HexWeights.h"

class HBTMethods
{
public:
	/// <summary>
	/// Creates chain, and updates templates/edges for that chain
	/// </summary>
	/// <param name="move"></param>
	/// <param name="size"></param>
	/// <param name="isRed"></param>
	/// <param name="chains"></param>
	/// <param name="oldChains"></param>
	/// <param name="templates"></param>
	/// <param name="edges"></param>
	/// <param name="sEdges"></param>
	/// <param name="placedTiles"></param>
	/// <param name="oppTiles"></param>
	/// <param name="helper"></param>
	/// <returns></returns>
	static void CreateChain(short move, short size, bool isRed,
		std::vector<Chain>& chains,
		const std::vector<Chain>& oldChains,
		std::vector<Template>& templates,
		std::vector<Edge>& edges,
		std::vector<SpecialEdge>& sEdges,
		BitArray& placedTiles,
		BitArray& sameTiles,
		const BitArray& oppTiles,
		const HexBoardHelper* helper);

	static void RemapAddresses(std::vector<Template>& templates, 
		std::vector<Edge>& edges, 
		std::vector<SpecialEdge>& sEdges, 
		const std::vector<Chain>& oldChains, 
		const std::vector<Chain>& newChains);

	/// <summary>
	/// Takes the move and updates opponents structures for the move
	/// </summary>
	/// <param name="move"></param>
	/// <param name="chains"></param>
	/// <param name="templates"></param>
	/// <param name="sEdges"></param>
	/// <param name="sameTiles"></param>
	static void ApplyMoveToOpponentStructures(short move,
		std::vector<Chain>& chains,
		std::vector<Template>& templates,
		std::vector<SpecialEdge>& sEdges);

	/// <summary>
	/// Takes the poison, applies the Templates/Edges, and updates chain ranks
	/// </summary>
	/// <param name="poison"></param>
	/// <param name="size"></param>
	/// <param name="isRed"></param>
	/// <param name="helper"></param>
	/// <param name="chains"></param>
	/// <param name="oTemplates"></param>
	/// <param name="oEdges"></param>
	/// <param name="osEdges"></param>
	static void PoisonUpdateTilesRanks(
		const Poison& poison,
		short size,
		bool isRed,
		const HexBoardHelper* helper,
		std::vector<Chain>& chains,
		const std::vector<Template>& oTemplates,
		const std::vector<Edge>& oEdges,
		const std::vector<SpecialEdge>& osEdges
		);


	static Poison ApplyStructuresToPoison(
		const Poison& poison,
		short size,
		bool isRed,
		const HexBoardHelper* helper,
		const std::vector<Template>& templates,
		const std::vector<Edge>& edges,
		const std::vector<SpecialEdge>& sEdges
	);

	/// <summary>
	/// Gets the length of the longest chain
	/// </summary>
	/// <param name="chains"></param>
	/// <returns></returns>
	static int GetLongestChainLength(const std::vector<Chain>& chains);

	/// <summary>
	/// Gets the length of a chain, with any chains that have a template with it,
	/// As well as applying edges
	/// </summary>
	/// <param name="chains"></param>
	/// <param name="templates"></param>
	/// <param name="edges"></param>
	/// <param name="sEdges"></param>
	/// <returns></returns>
	static int GetLongestTemplateLength(
		std::vector<Chain>& chains,
		std::vector<Template>& templates,
		std::vector<Edge>& edges,
		std::vector<SpecialEdge>& sEdges
	);


	/// <summary>
	/// Removes any templates that have had an intrusive move
	/// Needs to be called after scoring so that worthless intrusions aren't rewarded
	/// </summary>
	/// <param name="templates"></param>
	/// <param name="edges"></param>
	/// <param name="sEdges"></param>
	/// <param name="helper"></param>
	static void RemoveBrokenTemplates(
		std::vector<Template>& templates,
		std::vector<Edge>& edges,
		std::vector<SpecialEdge>& sEdges,
		const HexBoardHelper* helper
	);

	/// <summary>
	/// Converts the board state to a score
	/// Which is used to evaluate which positions are good
	/// And which branches to explore further
	/// </summary>
	/// <param name="redChainLength"></param>
	/// <param name="blueChainLength"></param>
	/// <param name="redTemplateLength"></param>
	/// <param name="blueTemplateLength"></param>
	/// <param name="centerScore"></param>
	/// <param name="move"></param>
	/// <param name="size"></param> 
	/// <param name="isRed"></param>
	/// <param name="redTiles"></param>
	/// <param name="blueTiles"></param>
	/// <param name="weights"></param>
	/// <returns></returns>
	static float GetScore(
		int redChainLength,
		int blueChainLength,
		int redTemplateLength,
		int blueTemplateLength,
		float& centerScore,
		short move,
		short size,
		bool isRed,
		const BitArray& redTiles,
		const BitArray& blueTiles,
		const HexWeights* weights
	);

private:
	HBTMethods() {}
};