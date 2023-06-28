#pragma once
#include "Scene_Testing.h"
#include "Playground_1.h"

namespace Scenes {
	inline Scene testing("testing", Load_Testing, Start_Testing, Update_Testing, End_Testing, Unload_Testing);
	inline Scene playground_1("playground_1", Load_Playground_1, Start_Playground_1, Update_Playground_1, End_Playground_1, Unload_Playground_1);
}