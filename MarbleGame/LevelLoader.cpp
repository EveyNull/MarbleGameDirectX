#include "LevelLoader.h"
#include "MathHelper.h"
#include "StringHelper.h"
#include <bitset>
#include <fstream>
#include <string>
#include <map>

GameObject** LevelLoader::LoadLevel(int levelNumber, int& outGeometryNumber)
{
	std::string fileText;
	std::string levelFileName = "level" + std::to_string(levelNumber) + ".txt";
	std::ifstream levelFile(levelFileName);

	int highestNumber = 0;

	int lineNumber = 0;

	auto levelMap = new std::vector<std::pair<VECTOR2, int>>();
	int blocksTotal = 0;

	while (getline(levelFile, fileText))
	{

		std::vector<std::string> line = StringHelper::split(fileText, ",");
		
		for(int i = 0; i < (int)line.size(); ++i)
		{
			int unit = std::stoi(line[i]);
			if (unit > 0)
			{
				VECTOR2 coords = { i, lineNumber };
				auto entry = std::pair<VECTOR2, int>(coords, unit);
				levelMap->push_back(entry);
				if (unit > highestNumber)
				{
					highestNumber = unit;
				}
				while (unit >= 1)
				{
					if (unit % 2 == 1)
					{
						blocksTotal++;
					}
					unit /= 2;
				}
			}
		}
		lineNumber++;
	}

	levelFile.close();

	GameObject** levelBlocks = new GameObject* [blocksTotal];

	int bitNumber = MathHelper::NumberOfBits(highestNumber);

	int numBlocks = 0;
	for (auto itr = levelMap->begin(); itr != levelMap->end(); ++itr)
	{
		int* values = new int[bitNumber];
		int decimal = itr->second;
		
		MathHelper::BinarySeparateInts(values, decimal, bitNumber);

		for (int i = bitNumber-1; i >= 0; --i)
		{
			if (values[i] == 1)
			{
				GameObject* gameObject = new GameObject();
				gameObject->SetPosition({ itr->first.x, -(float)(bitNumber - i), -itr->first.y });
				levelBlocks[numBlocks++] = gameObject;
			}
		}
	}
	outGeometryNumber = numBlocks;
	return levelBlocks;
}
