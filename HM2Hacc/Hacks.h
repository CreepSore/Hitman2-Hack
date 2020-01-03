#pragma once
#include "Vector.h"

static vec3d* getPosition();

static int currentEntityId = 0;
static float jumpstep = -1;
static DWORD64 lastJumpUpdate = 0;

class Hooks {
public:
    static void init();
    static void hook();
    static void unhook();
};

class Keybinds {
public:
    static void run();
    static vec3d latestPos;
};