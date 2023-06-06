#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Entity.h"
#include "Camera.h"


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

