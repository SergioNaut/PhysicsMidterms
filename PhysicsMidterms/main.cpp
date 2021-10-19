#include <iostream>
#include <map>
#include <string>
#include <fstream>

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

#pragma region BitsfromProject1

//Not Sure if I`ll need any random bits

// Generate a random number between 0 and 1
float getRandom();

// Generate a random number between zero and a given high value
float getRandom(float high);

// Generate a random number in a given range
float getRandom(float low, float high);

// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
// The output is designed to be linearly independent from the output of getRandomZVector()
glm::vec3 getRandomXVector();

// Returns a vector laying on the x-z plane, randomized in direction and magnitude.
// The output is designed to be linearly independent from the output of getRandomXVector()
glm::vec3 getRandomZVector();

// Determine from the parameters if the particle is currently above the ground.
bool particleIsAboveGround(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration);

// Determine from the parameters if the particle is currently moving "up".
bool particleIsMovingUpward(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration);

// Perform one single time-step implementing Euler Integration.
void doTimeStepEuler(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration);
// Perform one single time-step implementing Midpoint Integration.
void doTimeStepMidpoint(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration);

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec);

void mainLoop();
#pragma endregion

#pragma region CameraSetup
nGraphics::sPerFrameVars PerFrameVars;
nGraphics::c3rdPersonCamera* camera = 0;
nGraphics::cGraphicsComponent* skyboxGraphics;
nGraphics::cGraphicsComponent* planeGraphics;
nGraphics::cGraphicsComponent* sphereGraphics;
#pragma endregion

int main()
{
#pragma region Tests
	sConfig projectileConfig;
	sCannonConfig cannonConfig;

	ReadConfigFromXML("BulletConfig.xml", projectileConfig);
	ReadCannonConfigFromXML("CannonConfig.xml", cannonConfig);

	/*std::cout << projectileConfig.ProjectileDef.gravity  << std::endl;
	std::cout << cannonConfig.CannonDef.maxYaw << std::endl;

	float gravity = projectileConfig.ProjectileDef.gravity;
	std::cout << gravity;*/
#pragma endregion
#pragma region StuffFromProject1
	camera = new nGraphics::c3rdPersonCamera();
	nGraphics::SetCamera(camera);
	glm::mat4 identity(1.f);
	camera->SetTargetTransform(identity);

	if (!nGraphics::Init())
	{
		std::cout << "booo graphics didn't start up right" << std::endl;
		system("pause");
		return -1;
	}
	if (!nInput::Init())
	{
		std::cout << "booo inputs didn't start up right" << std::endl;
		system("pause");
		return -1;
	}

	{
		// Loading textures to use with our meshes
		nGraphics::gTextureManager->Load("../Assets/skybox/tidepool", "skybox", true);
		nGraphics::gTextureManager->Load("../Assets/WhiteSquare.bmp", "whiteSquare", false);
		nGraphics::gTextureManager->Load("../Assets/stone_wall.bmp", "stone", false);
		nGraphics::gTextureManager->Load("../Assets/grass.bmp", "grass", false);
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
		// Lower poly sphere mesh for spheres
		loadingInfo.File = "../Assets/SoccerBall.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f); // diameter 2 spheref
		loadingInfo.SubMeshes[0].Name = "sphere";
		infos.push_back(loadingInfo);
		// Box mesh for planes and boxes
		loadingInfo.File = "../Assets/box.obj";
		loadingInfo.DoResize = true;
		loadingInfo.Extents = glm::vec3(2.f, 2.f, 2.f);  // cube 2x2x2
		loadingInfo.SubMeshes[0].Name = "box";
		infos.push_back(loadingInfo);

		if (!nGraphics::gMeshManager->Load(infos))
		{
			std::cout << "booooo failed to load meshes" << std::endl;
			return -1;
		}
	}

	// Create the graphics components
	{
		// Skybox Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "skybox";
		graphicsDef.TexCubeMap = "skybox";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 0.0f, 0.0f);
		glm::set(graphicsDef.Scale, 500.0f, 500.0f, 500.0f);
		skyboxGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Sphere Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "sphere";
		graphicsDef.TexDiffuse = "stone";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, 1.0f, 0.0f);
		glm::set(graphicsDef.Scale, 1.0f, 1.0f, 1.0f);
		sphereGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}
	{
		// Plane Graphics Component
		nGraphics::sGraphicsComponentDef graphicsDef;
		graphicsDef.Mesh = "box";
		graphicsDef.TexDiffuse = "grass";
		glm::set(graphicsDef.ModelColor, 1.0f, 1.0f, 1.0f, 1.0f);
		glm::set(graphicsDef.Position, 0.0f, -0.2f, 0.0f);
		glm::set(graphicsDef.Scale, 10.0f, 0.2f, 10.0f);
		planeGraphics = new nGraphics::cGraphicsComponent(graphicsDef);
	}


	// Enter the main loop
	mainLoop();

	// Clean up
	delete planeGraphics;
	delete sphereGraphics;
	delete skyboxGraphics;
	delete camera;

	nGraphics::Shutdown();

	// all done!
	return 0;
#pragma endregion
 }

 //TO DO: Change So each type of projectile is created
void InitProject1Variables(glm::mat3& axes, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration)
{
	axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
	// because our "sphere" has a radius of 1
	position = glm::vec3(0.0, 1.1f, 0.0);
	velocity = (axes[0] * getRandom(-2.f, 2.f)) + (axes[1] * 5.f) + (axes[2] * getRandom(-2.f, 2.f));
	velocity = glm::normalize(velocity);
	velocity *= 10.f;

	// Step 4: Define the acceleration due to gravity, and deltaTime.
	acceleration = glm::vec3(0.f, -9.8f, 0.f);

	// Step 5: Enter the main loop.
	// Track the total elapsed simulation time.
	timeElapsed = 0;
}

void InitProjectileVariables(glm::mat3& axes, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration,
	float size, float muzzle, float gravity, /*take lifetime out?*/ float& lifetime)
{	//Axes Becomes the cannon values?
	axes = orthonormalBasis(getRandomXVector(), getRandomZVector());
	//Added size so projectile won't get stuck on the ground
	position = glm::vec3(0.0, size + size/10, 0.0);
	velocity = (axes[0] * getRandom(-2.f, 2.f)) + (axes[1] * 5.f) + (axes[2] * getRandom(-2.f, 2.f));
	velocity = glm::normalize(velocity);
	
	//Added Muzzle to velocity
	velocity *= muzzle;

	// Added custom gravity
	acceleration = glm::vec3(0.f, -9.8f*gravity, 0.f);

	// Step 5: Enter the main loop.
	// Track the total elapsed simulation time.
	timeElapsed = 0;
}


void mainLoop()
{
	nInput::gInputManager->ClearState();

	nGraphics::Focus();

	//Setup projectile
	sConfig projectileConfig;
	//Setup cannon
	sCannonConfig cannonConfig;

	bool continueMainLoop = true;

	float previousTime = static_cast<float>(glfwGetTime());

	float fpsFrameCount = 0.f;
	float fpsTimeElapsed = 0.f;

	// BEGIN PROJECT 1 SETUP
	nPhysics::cParticleWorld* world = new nPhysics::cParticleWorld();
	nPhysics::cParticle* particle = new nPhysics::cParticle(1.0f, glm::vec3(0.f));
	if (world->AddParticle(particle))
	{
		std::cout << "Hurray!" << std::endl;
	}
	else
	{
		bool sexybreakpoint = true;
	}
	glm::mat3 axes;

	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec3 acceleration;
	float timeElapsed = 0;
	InitProject1Variables(axes, timeElapsed, position, velocity, acceleration);
	particle->SetPosition(position);
	particle->SetVelocity(velocity);
	particle->SetAcceleration(acceleration);
	bool inFlight = false;
	// END PROJECT 1 SETUP

#pragma region Keys
	nInput::cKey* spaceKey = nInput::cInputManager::GetInstance()->ListenToKey(nInput::KeyCode::KEY_SPACE);
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

#pragma region FPSPrinter
		// FPS TITLE STUFF
		{
			fpsTimeElapsed += deltaTime;
			fpsFrameCount += 1.0f;
			if (fpsTimeElapsed >= 0.03f)
			{
				std::string fps = std::to_string(fpsFrameCount / fpsTimeElapsed);
				std::string ms = std::to_string(1000.f * fpsTimeElapsed / fpsFrameCount);
				std::string newTitle = "FPS: " + fps + "   MS: " + ms;
				nGraphics::SetWindowTitle(newTitle);
				// reset times and counts
				fpsTimeElapsed = 0.f;
				fpsFrameCount = 0.f;
			}
		}
#pragma endregion

		// TODO: update inputs/controls/physics
		// Continue the simulation so long as the particle is above the ground.
		if (!inFlight)
		{
			if (spaceKey->IsJustPressed())
			{
				InitProject1Variables(axes, timeElapsed, position, velocity, acceleration);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
			}
			else if (Key1->IsJustPressed())
			{
				ReadConfigFromXML("BulletConfig.xml", projectileConfig);
				InitProjectileVariables(axes, timeElapsed, position, velocity, acceleration,
					projectileConfig.ProjectileDef.size, projectileConfig.ProjectileDef.muzzle, projectileConfig.ProjectileDef.gravity, /*take lifetime out?*/ projectileConfig.ProjectileDef.lifetime);
				particle->SetMass(projectileConfig.ProjectileDef.mass);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
			}
			else if (Key2->IsJustPressed())
			{
				ReadConfigFromXML("LaserConfig.xml", projectileConfig);
				InitProjectileVariables(axes, timeElapsed, position, velocity, acceleration,
					projectileConfig.ProjectileDef.size, projectileConfig.ProjectileDef.muzzle, projectileConfig.ProjectileDef.gravity, /*take lifetime out?*/ projectileConfig.ProjectileDef.lifetime);
				particle->SetMass(projectileConfig.ProjectileDef.mass);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
			}
			else if (Key3->IsJustPressed())
			{
				ReadConfigFromXML("CannonBallConfig.xml", projectileConfig);
				InitProjectileVariables(axes, timeElapsed, position, velocity, acceleration,
					projectileConfig.ProjectileDef.size, projectileConfig.ProjectileDef.muzzle, projectileConfig.ProjectileDef.gravity, /*take lifetime out?*/ projectileConfig.ProjectileDef.lifetime);
				particle->SetMass(projectileConfig.ProjectileDef.mass);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
			}
			else if (Key4->IsJustPressed())
			{
				ReadConfigFromXML("EnergyBallConfig.xml", projectileConfig);
				InitProjectileVariables(axes, timeElapsed, position, velocity, acceleration,
					projectileConfig.ProjectileDef.size, projectileConfig.ProjectileDef.muzzle, projectileConfig.ProjectileDef.gravity, /*take lifetime out?*/ projectileConfig.ProjectileDef.lifetime);
				particle->SetMass(projectileConfig.ProjectileDef.mass);
				particle->SetPosition(position);
				particle->SetVelocity(velocity);
				particle->SetAcceleration(acceleration);
				inFlight = true;
			}
		}

		particle->GetPosition(position);
		particle->GetVelocity(velocity);
		particle->GetAcceleration(acceleration);

		if (inFlight)
		{
			if (particleIsAboveGround(axes, deltaTime, timeElapsed, position, velocity, acceleration))
			{
				if (particleIsMovingUpward(axes, deltaTime, timeElapsed, position, velocity, acceleration))
				{
					// Let the user know the particle is moving up.
					std::cout << "going up! \tx=" << position.x << "\ty=" << position.y << "\tz=" << position.z << std::endl;
				}
				else
				{
					// Let the user know the particle is moving down.
					std::cout << "going down! \tx=" << position.x << "\ty=" << position.y << "\tz=" << position.z << std::endl;
				}
				world->TimeStep(deltaTime);
				// Step the simulation forward
				//doTimeStepEuler(axes, deltaTime, timeElapsed, position, velocity, acceleration);
				//doTimeStepMidpoint(axes, deltaTime, timeElapsed, position, velocity, acceleration);
			}
			else
			{
				std::cout << "Impacted after " << timeElapsed << " seconds with a velocity x=" << velocity.x << "\ty=" << velocity.y << "\tz=" << velocity.z << std::endl;
				inFlight = false;
			}
		}

		// Safety, mostly for first frame
		if (deltaTime == 0.f)
		{
			deltaTime = 0.03f;
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

		// begin per-item rendering
		skyboxGraphics->Render();
		planeGraphics->Render();
		sphereGraphics->GetVars()->ModelMatrix = glm::translate(glm::mat4(1.0f), position);
		sphereGraphics->Render();
		// end per-item rendering

		nGraphics::EndFrame();

		// Exit conditions: press escape or close the window by the 'x' button
		if (!(nInput::IsKeyUp::Escape() && !nGraphics::WindowShouldClose()))
		{
			continueMainLoop = false;
		}
	}

	// clean up!
	world->RemoveParticle(particle);
	delete particle;
	delete world;
}

#pragma region OriginalHelpers
float getRandom()
{
	return static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
}

float getRandom(float high)
{
	return static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / high);
}

float getRandom(float low, float high)
{
	return low + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX) / (high - low));
}

// Use as-is.  Do not change.
glm::vec3 getRandomXVector()
{
	return glm::vec3(getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

// Use as-is.  Do not change.
glm::vec3 getRandomZVector()
{
	return glm::vec3(-getRandom(0.1f, 1.f), 0.f, getRandom(0.1f, 1.f));
}

bool particleIsAboveGround(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration)
{
	// TODO: Use the parameters to determine if the particle
	//       is currently above the ground.
	//       Return true if the particle is above the ground, false otherwise.
	return position.y > 1.0f; // because our "sphere" has a radius of 1
}

bool particleIsMovingUpward(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration)
{
	// TODO: Use the parameters to determine if the particle
	//       is currently moving upward.
	//       Return true if the particle is above the ground, false otherwise.
	return glm::dot(velocity, axes[1]) > 0;
}

void doTimeStepEuler(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration)
{
	// TODO: Implement the Euler Integration algorithm to advance
	//       the simulation one step forward in time.
	position += velocity * deltaTime;
	velocity += acceleration * deltaTime;
	timeElapsed += deltaTime;
}

void doTimeStepMidpoint(glm::mat3& axes, float& deltaTime, float& timeElapsed, glm::vec3& position, glm::vec3& velocity, glm::vec3& acceleration)
{
	// TODO: Implement the Midpoint Integration algorithm to advance
	//       the simulation one step forward in time.
	glm::vec3 oldVelocity = velocity;
	velocity += acceleration * deltaTime;
	position += (oldVelocity + velocity) * 2.f * deltaTime;
	timeElapsed += deltaTime;
}

glm::mat3 orthonormalBasis(const glm::vec3& xVec, const glm::vec3& zVec)
{
	// TODO: Generate an orthonormal basis, using xVec and zVec.
	//       The input vectors may be manipulated, however the 
	//       returned axes must essentially be:
	//       x-axis: sourced from xVec
	//       y-axis: generated using math!
	//       z-axis: sourced from zVec

	// Generate y, by crossing z and x.
	glm::vec3 x(xVec);
	glm::vec3 z(zVec);
	glm::vec3 y(glm::cross(z, x));
	// Ensure z is orthogonal to both x and y.
	z = glm::cross(x, y);
	// Normalize everything.
	x = glm::normalize(x);
	y = glm::normalize(y);
	z = glm::normalize(z);
	// Return the result.
	return glm::mat3(x, y, z);
}
#pragma endregion