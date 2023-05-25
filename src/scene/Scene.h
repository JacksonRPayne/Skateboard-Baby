#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Entity.h"
#include "Camera.h"

/* Old scene class
struct Scene
{
public:

	// When calling this constructor, the creation of the camera is left to the scene
	Scene(const std::string& name, Renderer* renderer);
	Scene(const std::string &name, Camera* mainCamera, Renderer* renderer);

	// Loads in resources and creates entities
	virtual void Load(){}
	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void End() {}
	virtual void Unload(){}

	const std::string& GetName() const { return name; } 

	// Doesn't have ownership of these
	Camera* mainCamera;
	Renderer* renderer;

	bool loaded;

	std::string name;

};*/

struct Scene {
	Scene() :
		name(), Load(nullptr), Start(nullptr),
		Update(nullptr), End(nullptr), Unload(nullptr)
	{}
	Scene(std::string _name, void(*load)(), void(*start)(),
		void(*update)(float dt), void(*end)(), void(*unload)())
		:name(_name), Load(load), Start(start), Update(update),
		End(end), Unload(unload)
	{}

	void SetVals(std::string _name, void(*load)(), void(*start)(),
		void(*update)(float dt), void(*end)(), void(*unload)()) {
		name = _name;
		Load = load;
		Start = start;
		Update = update;
		End = end;
		Unload = unload;
	}

	std::string name;
	void(*Load)();
	void(*Start)();
	void(*Update)(float dt);
	void(*End)();
	void(*Unload)();

	bool loaded = false;
};

