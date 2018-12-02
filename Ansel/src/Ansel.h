#pragma once

/* Core Engine */
#include "headers/util.h"
#include "headers/Engine.h"
#include "headers/Window.h"
#include "headers/Screen.h"
#include "vendor/SimplexNoise.h"

#include "headers/event/Mouse.h"
#include "headers/event/Keyboard.h"

#include "headers/entities/Model.h"
#include "headers/entities/camera.h"
#include "headers/entities/StaticModelArray.h"
#include "headers/entities/ParticleSystem.h"

#include "headers/rendering/Renderer.h"
#include "headers/rendering/Shader.h"
#include "headers/rendering/Font.h"
#include "headers/rendering/Text.h"

#ifdef __CLIENT_ECS
#	 include <ECS.h>
#endif // __CLIENT_ECS

