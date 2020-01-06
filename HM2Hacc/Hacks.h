#pragma once
#include "Vector.h"


class Utils {
public:
    static vec3d* getPosition();
};

class Hooks {
public:
    static void init();
    static void hook();
    static void unhook();
    static void loop();
};

class Teleport {
    static double speed;
    static vec3d latestPos;
public:
    static void loop();
};

class Airwalk {
public:
    static void loop();
};