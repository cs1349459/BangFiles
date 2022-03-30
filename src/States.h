#pragma once

#include "utils.h"
#include "main.h"

class State {
    public:
        virtual void update();
		virtual void unfixedUpdate() {};
        virtual void draw();
        virtual void onKeyPress(int key, int action, int mods);
        virtual void onMouseClick(double x, double y, int action, int button);
		virtual string getDebugMenu() {return "Current FPS: " + to_string(currentFps);};
		virtual void postProcessing() {};
		virtual void onLoad() {};
		virtual void onUnload() {};
		virtual ~State() {};
        State();
};

/*
base state template
class BaseState: public State {
   public:
      void update() override;
      void draw() override;
      void onKeyPress(int key, int action, int mods) override;
      void onMouseClick(double x, double y, int action, int button) override;
};
*/

class MenuState: public State {
    public:
        float hrot;
        float rot;
        float rotvel;
        bool goingup;
        Point2 points[4] = {{-1.f, -0.2f},{1.f, -0.2f},{1.f, -0.5f},{-1.f, -0.5f}};
        void update() override;
        void draw() override;
        void onKeyPress(int key, int action, int mods) override;
        void onMouseClick(double x, double y, int action, int button) override;
};

class TestState: public State {
    public:
        bool isGreen = true;
        void update() override;
        void draw() override;
        void onKeyPress(int key, int action, int mods) override;
        void onMouseClick(double x, double y, int action, int button) override;
};

class PlayState: public State {
    public:
        void update() override;
		void unfixedUpdate() override;
        void draw() override;
        void onKeyPress(int key, int action, int mods) override;
        void onMouseClick(double x, double y, int action, int button) override;
		void postProcessing() override;
		string getDebugMenu() override;
		void onLoad() override;
		void onUnload() override;
};

class MainMenuState: public State {
   public:
      void update() override;
      void draw() override;
      void onKeyPress(int key, int action, int mods) override;
      void onMouseClick(double x, double y, int action, int button) override;
};

class GameChooserState: public State {
   public:
      void update() override;
      void draw() override;
      void onKeyPress(int key, int action, int mods) override;
      void onMouseClick(double x, double y, int action, int button) override;
};
class ShopState: public State {
   public:
      void update() override;
      void draw() override;
      void onKeyPress(int key, int action, int mods) override;
      void onMouseClick(double x, double y, int action, int button) override;
};