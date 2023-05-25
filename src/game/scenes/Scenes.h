#pragma once
#include "Scene_Testing.h"

namespace Scenes {
	inline Scene testing("testing", Load_Testing, Start_Testing, Update_Testing, End_Testing, Unload_Testing);
}