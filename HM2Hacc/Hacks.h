#pragma once
#include "Vector.h"

static vec3d * getPosition();
static DWORD64 getEntityList();
static int * getEntityCount();

static int currentEntityId = 0;
static float jumpstep = -1;
static long lastJumpUpdate = 0;

struct Hooks {
	static void hook();
};

struct Jump{
	static void run();
	static vec3d latestPos;
};