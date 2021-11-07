#include "LevelLoader.h"
#include "StringHelper.h"
#include <bitset>
#include <fstream>
#include <string>
#include <map>

int NumberOfBits(int input);
void BinarySeparateInts(int* ints, int input, int bitNumber);

GameObject** LevelLoader::LoadLevel(int levelNumber, int& outGeometryNumber)
{
	std::string fileText;
	std::string levelFileName = "level" + std::to_string(levelNumber) + ".txt";
	std::ifstream levelFile(levelFileName);

	int highestNumber = 0;

	int lineNumber = 0;

	auto levelMap = new std::vector<std::pair<VECTOR2, int>>();

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
			}
		}
		lineNumber++;
	}

	levelFile.close();

	GameObject** levelBlocks = new GameObject* [levelMap->size()];

	int bitNumber = NumberOfBits(highestNumber);

	int numBlocks = 0;
	for (auto itr = levelMap->begin(); itr != levelMap->end(); ++itr)
	{
		int* values = new int[bitNumber];
		int decimal = itr->second;
		
		BinarySeparateInts(values, decimal, bitNumber);

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

//Return number of bits required for biggest number in the level map data
int NumberOfBits(int input)
{
	int bits = 1;
	while (input > 1)
	{
		input /= 2;
		bits++;
	}
	return bits;
}

void BinarySeparateInts(int* ints, int input, int bitNumber)
{
	for(int i = bitNumber-1; i >= 0; --i)
	{
		ints[i] = input % 2;
		input /= 2;
	}
}