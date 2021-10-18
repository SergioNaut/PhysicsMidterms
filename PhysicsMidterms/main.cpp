#include <iostream>
#include <map>
#include <string>
#include <fstream>

#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>



#pragma region GenericXMLstuff
//TO DO: Useless?
//bool SetValue(rapidxml::xml_attribute<>* valueIn, int& valueOut)
//{
//	if (!valueIn)
//	{
//		return false;
//	}
//	valueOut = std::stoi(valueIn->value());
//	return true;
//}

bool SetValue(rapidxml::xml_node<>* valueIn, std::string& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	valueOut = valueIn->value();
	return true;
}

bool SetValue(rapidxml::xml_attribute<>* valueIn, float& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	valueOut = std::stof(valueIn->value());
	return true;
}
#pragma endregion

#pragma region ProjectileStuff

struct sProjectileDef
{
	float size;
	float mass;
	float muzzle;
	float gravity;
	float lifetime;
};

struct sConfig
{
	sProjectileDef ProjectileDef;
};

bool ParseProjectileDef(rapidxml::xml_node<>* valueIn, sProjectileDef& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("size"), valueOut.size);
	result &= SetValue(valueIn->first_attribute("mass"), valueOut.mass);
	result &= SetValue(valueIn->first_attribute("muzzle"), valueOut.muzzle);
	result &= SetValue(valueIn->first_attribute("gravity"), valueOut.gravity);
	result &= SetValue(valueIn->first_attribute("lifetime"), valueOut.lifetime);

	return result;
}

bool ParseConfig(rapidxml::xml_node<>* valueIn, sConfig& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	bool result = true;
	result &= ParseProjectileDef(valueIn->first_node("Data"), valueOut.ProjectileDef);
	return result;
}

bool ReadConfigFromXML(const std::string& filePath, sConfig& configOut)
{
	//Seems really dumb but that`s how it`s working
	std::ifstream i(filePath);

	using namespace rapidxml;

	// load the file
	//file<>* xmlFile = new file<>("BulletConfig.xml");
	//file<>* xmlFile = new file<>(filePath);
	file<>* xmlFile = new file<>(i);

	xml_document<>* doc = new xml_document<>;    // character type defaults to char
	doc->parse<0>(xmlFile->data());// 0 means default parse flags

	bool result = ParseConfig(doc->first_node("Config"), configOut);

	delete xmlFile;
	delete doc;

	return result;
}
#pragma endregion

#pragma region CannonStuff

struct sCannonDef
{
	//Vertical
	float minYaw;
	float maxYaw;
	//Horizontal
	float minPitch;
	float maxPitch;
};
struct sCannonConfig
{
	sCannonDef CannonDef;
};

bool ParseCannonDef(rapidxml::xml_node<>* valueIn, sCannonDef& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	bool result = true;
	result &= SetValue(valueIn->first_attribute("minYaw"), valueOut.minYaw);
	result &= SetValue(valueIn->first_attribute("maxYaw"), valueOut.maxYaw);
	result &= SetValue(valueIn->first_attribute("minPitch"), valueOut.minPitch);
	result &= SetValue(valueIn->first_attribute("maxPitch"), valueOut.maxPitch);

	return result;
}

bool ParseCannonConfig(rapidxml::xml_node<>* valueIn, sCannonConfig& valueOut)
{
	if (!valueIn)
	{
		return false;
	}
	bool result = true;
	result &= ParseCannonDef(valueIn->first_node("Data"), valueOut.CannonDef);
	return result;
}

bool ReadCannonConfigFromXML(const std::string& filePath, sCannonConfig& configOut)
{
	//Seems really dumb but that`s how it`s working
	std::ifstream i(filePath);

	using namespace rapidxml;

	// load the file
	//file<>* xmlFile = new file<>("BulletConfig.xml");
	//file<>* xmlFile = new file<>(filePath);
	file<>* xmlFile = new file<>(i);

	xml_document<>* doc = new xml_document<>;    // character type defaults to char
	doc->parse<0>(xmlFile->data());// 0 means default parse flags

	bool result = ParseCannonConfig(doc->first_node("Config"), configOut);

	delete xmlFile;
	delete doc;

	return result;
}
#pragma endregion

int main()
{
	sConfig projectileConfig;
	sCannonConfig cannonConfig;

	ReadConfigFromXML("BulletConfig.xml", projectileConfig);
	ReadCannonConfigFromXML("CannonConfig.xml", cannonConfig);

	std::cout << projectileConfig.ProjectileDef.gravity  << std::endl;
	std::cout << cannonConfig.CannonDef.maxYaw;
 }