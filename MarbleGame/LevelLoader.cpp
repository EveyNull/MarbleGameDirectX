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

	auto levelMap = new std::vector<std::tuple<VECTOR2, int, int>>();
	int blocksTotal = 0;

	while (getline(levelFile, fileText))
	{

		std::vector<std::string> line = StringHelper::split(fileText, ",");
		
		for(int i = 0; i < (int)line.size(); ++i)
		{
			auto splitEntry = StringHelper::split(line[i], ":");
			int unit = std::stoi(splitEntry[0]);
			if (unit > 0)
			{
				VECTOR2 coords = { i, lineNumber };
				int type = 2;
				if (splitEntry.size() > 1) type = std::stoi(splitEntry[1]);
				auto entry = std::tuple<VECTOR2, int, int>(coords, unit, type);
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
		VECTOR2 location = std::get<0>(*itr);
		int decimal = std::get<1>(*itr);
		int type = std::get<2>(*itr);
		
		MathHelper::BinarySeparateInts(values, decimal, bitNumber);

		for (int i = bitNumber-1; i >= 0; --i)
		{
			if (values[i] == 1)
			{
				GameObject* gameObject = new GameObject(type);
				gameObject->SetPosition({ location.x, -(float)(bitNumber - i), -location.y });
				levelBlocks[numBlocks++] = gameObject;
			}
		}
	}
	outGeometryNumber = numBlocks;
	return levelBlocks;
}
