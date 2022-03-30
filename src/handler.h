#include "Image.h"
#include "States.h"

void handOnLoad();

void handlerUnfixedUpdate();
void handlerUpdate();

void onKey(int key, int action, int mods);
void onMouse(double x, double y, int action, int button);

void handlerDraw();

void goToState(State* newState);

void exitToParent();