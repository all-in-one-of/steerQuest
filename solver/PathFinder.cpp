#include "PathFinder.h"

std::vector<Vec2f>
PathFinder::getPath(Scene* scene)
{
	cleanup();
	mScene = scene;
	int startX = (int)mScene->getStartPosition().x;
	int startY = (int)mScene->getStartPosition().y;
	int startId = startY * mScene->getGrid().getMaxX() + startX;
	mStartNode = new SearchNode(startX, startY, startId);

	int endX = (int)mScene->getEndPosition().x;
	int endY = (int)mScene->getEndPosition().y;
	int endId = endY * mScene->getGrid().getMaxX() + endX;
	mEndNode = new SearchNode(endX, endY, endId);
	mStartNode->calculateHeuristicCost(mEndNode);

	mOpenList.push_back(mStartNode);

	searchPath();

	if (mFoundPath) {
		return mPath;
	}
	else {
		return std::vector<Vec2f>();
	}
}

std::vector<Vec2f>
PathFinder::getPath(Scene* scene, Vec2f currentPosition)
{
	cleanup();
	mScene = scene;
	int startX = (int)currentPosition.x;
	int startY = (int)currentPosition.y;
	int startId = startY * mScene->getGrid().getMaxX() + startX;
	mStartNode = new SearchNode(startX, startY, startId);


	int endX = (int)mScene->getEndPosition().x;
	int endY = (int)mScene->getEndPosition().y;
	int endId = endY * mScene->getGrid().getMaxX() + endX;
	mEndNode = new SearchNode(endX, endY, endId);
	mStartNode->calculateHeuristicCost(mEndNode);

	mOpenList.push_back(mStartNode);

	searchPath();

	if (mFoundPath) {
		return mPath;
	}
	else {
		return std::vector<Vec2f>();
	}
}


SearchNode*
PathFinder::getNextNode()
{
	double bestCost = std::numeric_limits<float>::max();
	int nodeIndex = -1;
	size_t i = 0;
	SearchNode* nextNode = nullptr;

	for (SearchNode* node : mOpenList)
	{
		if (node->getExpectedCost() < bestCost)
		{
			bestCost = node->getExpectedCost();
			nodeIndex = i;
		}
		i++;
	}

	if (nodeIndex >= 0)
	{
		nextNode = mOpenList[nodeIndex];
		mClosedList.push_back(nextNode);
		mOpenList.erase(mOpenList.begin() + nodeIndex);
	}

	return nextNode;
}

void
PathFinder::searchNode(unsigned int x,unsigned int y, double movementCost, SearchNode* parent)
{
	if (!mScene->getCell(x, y))
	{
		return;
	}

	int id = y * mScene->getGrid().getMaxX() + x;
	SearchNode* newNode = new SearchNode(x, y, id, parent);
	newNode->calculateHeuristicCost(mEndNode);
	newNode->movementCost = movementCost;

	auto iter = mClosedList.begin();
	while (iter != mClosedList.end())
	{
		SearchNode* node = (*iter);
		if (node->id == newNode->id) {
			if (node->getExpectedCost() > newNode->getExpectedCost())
			{
				mClosedList.erase(iter);
				break;
			}
			else
			{
				delete newNode;
				return;
			}
		}
		++iter;
	}

	for (SearchNode* node : mOpenList)
	{
		if (id == node->id)
		{
			if (node->getExpectedCost() > newNode->getExpectedCost())
			{
				node->movementCost = newNode->movementCost;
				node->parent = parent;
				return;
			}
			else
			{
				delete newNode;
				return;
			}
		}
	}

	mOpenList.push_back(newNode);
}

void
PathFinder::searchPath()
{
	while (!mOpenList.empty()) {
		SearchNode* node = getNextNode();
		if (node->id == mEndNode->id)
		{
			std::vector<Vec2f> inversePath;
			while (node != nullptr)
			{
				inversePath.push_back(Vec2f(node->x, node->y));
				node = node->parent;
			}

			mPath.assign(inversePath.rbegin(), inversePath.rend());
			mFoundPath = true;
			break;
		}
		else {
			int maxX = mScene->getGrid().getMaxX() - 1;
			int maxY = mScene->getGrid().getMaxY() - 1;

			if (node->x < maxX)
				searchNode(node->x + 1, node->y, node->movementCost + 1, node);
			if (node->x > 0)
				searchNode(node->x - 1, node->y, node->movementCost + 1, node);
			if (node->y < maxY)
				searchNode(node->x, node->y + 1, node->movementCost + 1, node);
			if (node->y > 0)
				searchNode(node->x, node->y - 1, node->movementCost + 1, node);
			if (node->x < maxX && node->y < maxY)
				searchNode(node->x + 1, node->y + 1, node->movementCost + 1.414, node);
			if (node->x > 0 && node->y < maxY)
				searchNode(node->x - 1, node->y + 1, node->movementCost + 1.414, node);
			if (node->x < maxX && node->y > 0)
				searchNode(node->x + 1, node->y - 1, node->movementCost + 1.414, node);
			if (node->x > 0 && node->y > 0)
				searchNode(node->x - 1, node->y - 1, node->movementCost + 1.414, node);
		}
	}
}

void
PathFinder::cleanup()
{
	for (SearchNode* node : mOpenList)
	{
		delete node;
	}

	for (SearchNode* node : mClosedList)
	{
		delete node;
	}

	mFoundPath = false;
	mPath.clear();
	mOpenList.clear();
	mClosedList.clear();
}
