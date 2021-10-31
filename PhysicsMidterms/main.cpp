#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <math.h>

//XML
#include <rapidxml/rapidxml.hpp>
#include <rapidxml/rapidxml_utils.hpp>

//Graphics
#include <graphics/graphics.h>
#include <input/input.h>
#include <graphics/cCamera.h>
#include <graphics/cameras/c3rdPersonCamera.h>
#include <graphics/cGraphicsComponent.h>
#include <physics/cParticleWorld.h>
#include<physics/cProjectile.h>


#pragma region GenericXMLstuff

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

//struct sProjectileDef
//{
//	float size;
//	float mass;
//	float muzzle;
//	float gravity;
//	float lifetime;
//};

nPhysics::sProjectileDef bulletDef;
nPhysics::sProjectileDef cannonBallDef;
nPhysics::sProjectileDef laserDef;
nPhysics::sProjectileDef energyBallDef;

//struct sConfig
//{
//	nPhysics::sProjectileDef ProjectileDef;
//};
//
//bool ParseProjectileDef(rapidxml::xml_node<>* valueIn, nPhysics::sProjectileDef& valueOut)
//{
//	if (!valueIn)
//	{
//		return false;
//	}
//	bool result = true;
//	result &= SetValue(valueIn->first_attribute("size"), valueOut.size);
//	result &= SetValue(valueIn->first_attribute("mass"), valueOut.mass);
//	result &= SetValue(valueIn->first_attribute("muzzle"), valueOut.muzzle);
//	result &= SetValue(valueIn->first_attribute("gravity"), valueOut.gravity);
//	result &= SetValue(valueIn->first_attribute("lifetime"), valueOut.lifetime);
//
//	return result;
//}
//
//bool ParseConfig(rapidxml::xml_node<>* valueIn, sConfig& valueOut)
//{
//	if (!valueIn)
//	{
//		return false;
//	}
//	bool result = true;
//	result &= ParseProjectileDef(valueIn->first_node("Data"), valueOut.ProjectileDef);
//	return result;
//}
//
//bool ReadConfigFromXML(const std::string& filePath, sConfig& configOut)
//{
//	//Seems really dumb but that`s how it`s working
//	std::ifstream i(filePath);
//
//	using namespace rapidxml;
//
//	// load the file
//	//file<>* xmlFile = new file<>("BulletConfig.xml");
//	//file<>* xmlFile = new file<>(filePath);
//	file<>* xmlFile = new file<>(i);
//
//	xml_document<>* doc = new xml_document<>;    // character type defaults to char
//	doc->parse<0>(xmlFile->data());// 0 means default parse flags
//
//	bool result = ParseConfig(doc->first_node("Config"), configOut);
//
//	delete xmlFile;
//	delete doc;
//
//	return result;
//}
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

	file<>* xmlFile = new file<>(i);

	xml_document<>* doc = new xml_document<>;    // character type defaults to char
	doc->parse<0>(xmlFile->data());// 0 means default parse flags

	bool result = ParseCannonConfig(doc->first_node("Config"), configOut);

	delete xmlFile;
	delete doc;

	return result;
}
#pragma endregion

#pragma region BitsfromProject1

bool particleIsAboveGround(glm::vec3& position);

void mainLoop();
#pragma endregion

#pragma region CameraSetup
nGraphics::sPerFrameVars PerFrameVars;
nGraphics::c3rdPersonCamera* camera = 0;
nGraphics::cGraphicsComponent* skyboxGraphics;
nGraphics::cGraphicsComponent* planeGraphics;
nGraphics::cGraphicsComponent* rockGraphics;

nGraphics::cGraphicsComponent* cannonGraphics;
#pragma endregion

int main()
{
#pragma region ProjectileSetups
	//Not using the data from the xml files currently
	
	//Bullet
	bulletDef.Acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	bulletDef.EnergyAcceleration = 0.0f;
	bulletDef.Speed = 25.0f;
	bulletDef.Position = glm::vec3(0.f);
	bulletDef.Damping = 0.95f;
	bulletDef.Mass = 20.0f;
	bulletDef.Radius = 1.5f;
	bulletDef.Colour = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
	bulletDef.GroundHeight = 0.0f;

	//Laser
	laserDef.Acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	laserDef.EnergyAcceleration = 0.0f;
	laserDef.Speed = 25.0f;
	laserDef.Position = glm::vec3(0.f);
	laserDef.Damping = 0.95f;
	laserDef.Mass = 20.0f;
	laserDef.Radius = 1.5f;
	laserDef.Colour = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
	laserDef.GroundHeight = 0.0f;

	//CannonBall
	cannonBallDef.Acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	cannonBallDef.EnergyAcceleration = 0.0f;
	cannonBallDef.Speed = 25.0f;
	cannonBallDef.Position = glm::vec3(0.f);
	cannonBallDef.Damping = 0.95f;
	cannonBallDef.Mass = 20.0f;
	cannonBallDef.Radius = 1.5f;
	cannonBallDef.Colour = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
	cannonBallDef.GroundHeight = 0.0f;

	//EnergyBall
	energyBallDef.Acceleration = glm::vec3(0.0f, -9.8f, 0.0f);
	energyBallDef.EnergyAcceleration = 0.0f;
	energyBallDef.Speed = 25.0f;
	energyBallDef.Position = glm::vec3(0.f);
	energyBallDef.Damping = 0.95f;
	energyBallDef.Mass = 20.0f;
	energyBallDef.Radius = 1.5f;
	energyBallDef.Colour = glm::vec4(1.f, 0.2f, 0.2f, 1.0f);
	energyBallDef.GroundHeight = 0.0f;

#pragma endregion


#pragma region StuffFromProject1
	camera = new nGraphics::c3rdPersonCamera();
	nGraphics::SetCamera(camera);
	glm::mat4 identity(1.f);
	camera->SetTargetTransform(identity);

	if (!nGraphics::Init())
	{
		std::cout << "Error Setting up Graphics" << std::endl;
		system("pause");
		return -1;
	}
	if (!nInput::Init())
	{
		std::cout << "Error Setting up inputs" << std::endl;
		system("pause");
		return -1;
	}

	{
		// Loading textures to use with our meshes
		//nGraphics::gTextureManager->Load("../Assets/skybox/tidepool", "skybox", true);
		nGraphics::gTextureManager->Load("../Assets/WhiteSquare.bmp", "whiteSquare", false);
		nGraphics::gTextureManager->Load("../Assets/stone_wall.bmp", "stone", false);
		nGraphics::gTextureManager->Load("../Assets/grafitti.bmp", "grafitti", false);
		nGraphics::gTextureManager->Load("../Assets/white.bmp", "white", false);
	}

	{
		// Loading meshes
		std::vector<nGraphics::sMeshLoadingInfo> infos;
		nGraphics::sMeshLoadingInfo loadingInfo;

		// Skybox sphere mesh
		loadingInfo.File = "../Assets/Isoshphere_xyz_InvertedNormals.ply";
		loadingInfo.DoResize = false;
		loadingInfo.AddSubMesh("skybox");
		infos.push_back(loadingInfo);
		// Lower poly Reck mesh for Projectiles
		loadingInfo.File = "../Assets/rock.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(1.f, 1.f, 1.f); // diameter 2 spheref
		loadingInfo.SubMeshes[0].Name = "rock";
		infos.push_back(loadingInfo);
		// Box mesh for planes and boxes
		loadingInfo.File = "../Assets/box.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // cube 2x2x2
		loadingInfo.SubMeshes[0].Name = "box";
		infos.push_back(loadingInfo);

		//Cannon
		loadingInfo.File = "../Assets/Cannon.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(0.1f, 0.1f, 0.1f);
		loadingInfo.SubMeshes[0].Name = "cannon";
		infos.push_back(loadingInfo);

		if (!nGraphics::gMeshManager->Load(infos))
		{
			std::cout << "Error loading meshes" << std::endl;
			return -1;
		}
	}

	// Create the graphics components
	{
		// Skybox Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		skyboxGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Rock Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "rock";
		glm::set(graphicsDef.ModelColor, 0.0f, 0.0f, 0.0f, 0.0f);
		glm::set(graphicsDef.Position, 0.0f, 1.0f, 0.0f);
		glm::set(graphicsDef.Scale, 1.0f, 1.0f, 1.0f);
		rockGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grafitti";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -0.2f, 0.0f);
		glm::set(graphicsDef.Scale, 10.0f, 0.2f, 10.0f);
		planeGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	//Cannon
	nGraphics::sGraphicsComponentDef graphicsDef;
	graphicsDef.Mesh = "cannon";
	graphicsDef.TexDiffuse = "white";
	glm::set(graphicsDef.ModelColor, 0.0f, 0.0f, 0.0f, 1.0f);
	glm::set(graphicsDef.Position, 0.5f, 0.0f, 0.5f);
	glm::set(graphicsDef.Rotation, -1.57f, 0.f, 0.f);
	glm::set(graphicsDef.Scale, 20.f, 20.1f, 20.1f);
	cannonGraphics = new nGraphics::cGraphicsComponent(graphicsDef);

	// Enter the main loop
	mainLoop();

	// Clean up
	delete planeGraphics;
	delete rockGraphics;
	delete skyboxGraphics;
	delete cannonGraphics;
	delete camera;

	nGraphics::Shutdown();

	// all done!
	return 0;
#pragma endregion
 }

 //TODO: MAKE CANNON ROTATION WORK
 glm::mat3 yawMatrix(float yaw)
 {
	 return glm::mat3(cos(yaw), sin(yaw), 0, -sin(yaw), cos(yaw), 0, 0, 0, 1);
 }

 glm::mat3 pitchMatrix(float pitch)
 {
	 return glm::mat3(cos(pitch), 0, -sin(pitch), 0, 1, 0, sin(pitch), 0, cos(pitch));
 }

 glm::mat3 rotationMatrix(float yaw, float pitch)
 {
	 return yawMatrix(yaw) * pitchMatrix(pitch);
 }


void InitProjectileVariables(float yaw, float pitch, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration,
	float size, float muzzle, float gravity)
{
	glm::mat3 rotMat = rotationMatrix(yaw, pitch);
	//Added size so projectile won't get stuck on the ground
	position = glm::vec3(0.0, size , 0.0);
	//velocity = glm::vec3(7.0f, 10.f, 0.f);
	velocity = glm::vec3((rotMat[0]*7.0f)+(rotMat[1]* 10.f)+ (rotMat[2] * 0.f));
	velocity = glm::normalize(velocity);
	
	//Added Muzzle to velocity
	velocity *= muzzle;

	// Added custom gravity
	acceleration = glm::vec3(0.f, -9.8f*gravity, 0.f);

	// Step 5: Enter the main loop.
}


void mainLoop()
{
	int rockColor = 1;
	nInput::gInputManager->ClearState();

	nGraphics::Focus();

	bool continueMainLoop = true;

	//Setup projectile
	//sConfig projectileConfig;
	//Setup cannon
	sCannonConfig cannonConfig;
	ReadCannonConfigFromXML("CannonConfig.xml", cannonConfig);

	//TODO: Make cannon turn grafically
	
	float cannonYaw = 0.3f;
	float cannonPitch = 10.0f;
	float* pCannonYaw = &cannonYaw;
	float* pCannonPitch = &cannonPitch;
	float minYaw = cannonConfig.CannonDef.minYaw;
	float maxYaw = cannonConfig.CannonDef.maxYaw;
	float minPitch = cannonConfig.CannonDef.minPitch;
	float maxPitch = cannonConfig.CannonDef.maxPitch;

	float previousTime = static_cast<float>(glfwGetTime());

	//World Setup
	std::vector<nPhysics::cProjectile*> projectiles;
	nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld();
	//nPhysics::cParticle* particle = new nPhysics::cParticle(1.0f, glm::vec3(0.f));
	/*if (world->AddParticle(particle))
	{
		std::cout << "Particle Added!" << std::endl;
	}
	else
	{
		std::cout << "Error adding particle." << std::endl;
	}
	glm::mat3 axes;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float timeElapsed = 0;
	
	particle->SetPosition(position);
	particle->SetVelocity(velocity);
	particle->SetAcceleration(acceleration);
	bool inFlight = false;*/
#pragma region Keys
	nInput::cKey* Key1 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_1);
	nInput::cKey* Key2 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_2);
	nInput::cKey* Key3 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_3);
	nInput::cKey* Key4 = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_4);
	nInput::cKey* wKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_W);
	nInput::cKey* aKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_A);
	nInput::cKey* sKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_S);
	nInput::cKey* dKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_D);
#pragma endregion

	while (continueMainLoop)
	{
		float currentTime = static_cast<float>(glfwGetTime());
		float deltaTime = currentTime - previousTime;
		previousTime = currentTime;
		
		world->TimeStep(deltaTime);

		if (deltaTime == 0.f)
		{
			deltaTime = 0.03f;
		}

		
		if (aKey->IsJustPressed())
		{
			if (cannonPitch < maxPitch)
			{
				cannonPitch += 1.0f;

			}
			if (cannonPitch > maxPitch)
			{
				cannonPitch = maxPitch;
			}
			std::cout << cannonPitch << std::endl;
		}
		if (sKey->IsJustPressed())
		{
			if (cannonYaw > minYaw)
			{
				cannonYaw -= 0.3f;
			}
			if (cannonYaw < minYaw)
			{
				cannonYaw = minYaw;
			}
			std::cout << cannonYaw << std::endl;
		}
		if (dKey->IsJustPressed())
		{
			if (cannonPitch > minPitch)
			{
				cannonPitch -= 1.0f;
			}
			if (cannonPitch < minPitch)
			{
				cannonPitch = minPitch;
			}
			std::cout << cannonPitch << std::endl;
		}
		if (wKey->IsJustPressed())
		{
			if (cannonYaw < maxYaw)
			{
				cannonYaw += 0.3f;
			}
			if (cannonYaw > maxYaw)
			{
				cannonYaw = maxYaw;
			}
			std::cout << cannonYaw << std::endl;
		}

		glm::mat4 launchMatrix = glm::mat4(1.0);
		launchMatrix = glm::eulerAngleXYX(cannonPitch, 0.0f, cannonYaw);
		glm::vec4 launchDirection(0.0f, 1.0f, 0.0f, 0.0f);
		launchDirection = launchMatrix * launchDirection;
		glm::vec3 launchPosition = launchDirection * 4.0f;

		if (Key1->IsJustPressed())
		{
			bulletDef.Position = launchPosition;
			bulletDef.Direction = launchDirection;
			nPhysics::cProjectile* projectile = new nPhysics::cProjectile(bulletDef);
			projectiles.push_back(projectile);
			world->AddParticle(projectile);
		}
		if (Key2->IsJustPressed())
		{
			laserDef.Position = launchPosition;
			laserDef.Direction = launchDirection;
			nPhysics::cProjectile* projectile = new nPhysics::cProjectile(laserDef);
			projectiles.push_back(projectile);
			world->AddParticle(projectile);
		}
		if (Key3->IsJustPressed())
		{
			cannonBallDef.Position = launchPosition;
			cannonBallDef.Direction = launchDirection;
			nPhysics::cProjectile* projectile = new nPhysics::cProjectile(cannonBallDef);
			projectiles.push_back(projectile);
			world->AddParticle(projectile);
		}
		if (Key4->IsJustPressed())
		{	
			energyBallDef.Position = launchPosition;
			energyBallDef.Direction = launchDirection;
			nPhysics::cProjectile* projectile = new nPhysics::cProjectile(energyBallDef);
			projectiles.push_back(projectile);
			world->AddParticle(projectile);
		}

		
		// update the camera
		camera->Update(deltaTime);

		// done with all the updates involving input, so clear it out
		nInput::gInputManager->ClearState();

		// begin setting per-frame vars
		camera->GetEyePosition(PerFrameVars.EyePosition);
		camera->GetViewMatrix(PerFrameVars.ViewMatrix);
		camera->GetProjectionMatrix(PerFrameVars.ProjectionMatrix);
		// end setting per-frame vars

		nGraphics::BeginFrame(PerFrameVars);

		planeGraphics->Render();

		glm::mat4 cannonMatrix = glm::mat4(1.0);
		cannonMatrix = glm::eulerAngleXYX(cannonPitch, 0.0f, cannonYaw);
		cannonMatrix = glm::translate(cannonMatrix, glm::vec3(0.0f, 0.0f, 0.0f));
		cannonMatrix = glm::scale(cannonMatrix, glm::vec3(20.f, 20.1f, 20.1f));
		cannonGraphics->GetVars()->ModelMatrix = cannonMatrix;
		cannonGraphics->Render();
		//cannonGraphics->GetVars()->ModelMatrix = glm::rotateY(glm::vec3(0.f, 1.0f, 0.f),1.57f);
		//Different Projectile Colors
		//if (rockColor == 1)
		//{
		//	//Gray bullet
		//	glm::set(rockGraphics->GetVars()->ModelColor, 1.0f, 1.0f, 1.0f);
		//}
		//else if (rockColor == 2)
		//{
		//	//Red laser
		//	glm::set(rockGraphics->GetVars()->ModelColor, 1.0f, 0.0f, 0.0f);
		//}
		//else if(rockColor ==3)
		//{
		//	//Black cannonball
		//	glm::set(rockGraphics->GetVars()->ModelColor, 0.0f, 0.0f, 0.0f);
		//}
		//else if (rockColor == 4)
		//{
		//	//Green energyball
		//	glm::set(rockGraphics->GetVars()->ModelColor, 0.0f, 1.0f, 0.0f);
		//}
		////Code to Turn Cannon
		//
		//rockGraphics->Render();

		for (nPhysics::cProjectile* p : projectiles)
		{
			glm::mat4 projectileMatrix(1.0f);
			projectileMatrix = glm::translate(projectileMatrix, p->GetPosition());
			projectileMatrix = glm::scale(projectileMatrix, glm::vec3(p->GetRadius()));
			rockGraphics->GetVars()->ModelMatrix = projectileMatrix;
			rockGraphics->GetVars()->ModelColor = p->GetColour();
			rockGraphics->Render();
		}
		
		/*glm::set(graphicsDef.Position, 0.5f, 0.0f, 0.5f);
	glm::set(graphicsDef.Rotation, -1.57f, 0.f, 0.f);
	glm::set(graphicsDef.Scale, 20.f, 20.1f, 20.1f);*/



		nGraphics::EndFrame();

		for (std::vector<nPhysics::cProjectile* > ::iterator it = projectiles.begin(); it != projectiles.end();)
		{
			if ((*it)->GetIsAlive())
			{
				it++;
			}
			else
			{
				std::vector<nPhysics::cProjectile*>::iterator removeIt = it;
				nPhysics::cProjectile* deadProjectile = *it;
				it = projectiles.erase(removeIt);
				world->RemoveParticle(deadProjectile);
				delete deadProjectile;
			}
		}
		// Exit conditions: press escape or close the window by the 'x' button
		if (!(nInput::IsKeyUp::Escape() && !nGraphics::WindowShouldClose()))
		{
			continueMainLoop = false;
		}
	}

	// clean up!
	for (nPhysics::cProjectile* p : projectiles)
	{
		world->RemoveParticle(p);
		delete p;
	}
	delete world;
}
