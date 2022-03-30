#pragma once

#include <string>
#include <map>

using namespace std;

#ifndef SAVES_H_
#define SAVES_H_

namespace saves {
	extern map<string, double> parts;
	extern map<string, double> masterSaveParts;
	extern string savePath;
	extern GameSettings currentSettings;
	
	string getSaveDirectory();
	string getSavePath();
	void read(string file, GameSettings s);
	void save(string file);
	void init();
	void masterSave();
	void masterLoad();
	
	double getXPforLevel(int level);
	double getXPforLevel();
}

#endif