#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <vector>
#include <filesystem>
#include <unistd.h>
#include <cmath>
#include <sys/types.h>
#include <pwd.h>

#include "utils.h"

using namespace std;

bool fileExists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }   
}


vector<string> split(string og, string delimiter) {
	string s = og;
	size_t pos = 0;
	std::string token;
	vector<string> toReturn;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		toReturn.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	return toReturn;
}

string split(string og, string delimiter, bool isFirst) {
	if(og.find(delimiter) == string::npos) {
		return og;
	}
	int where = og.find(delimiter);
	if(isFirst) {
		return og.substr(0, where);
	} else {
		return og.substr(where+1, og.length());
	}
	return "";
}

bool directoryExists(string dir) {
	return filesystem::exists(dir);
}

namespace saves {
	string homeDir = "";
	const string saveDirectory = "saves/";
	const string masterSaveFile = "masterSave.bin";
	string savePath = "";
	string defaultValues[] = {"money", "hits", "power"};
	string masterDefaultValues[] = {"bestX", "xp", "xpLevel"};
	double xpLevels[] = {10.d, 20.d, 30.d, 40.d, 50.d, 75.d, 100.d, 200.d, 300.d, 400.d, 500.d, 750.d, 1000.d, 2500.d, 5000.d, 10000.d,
							25000.d, 75000.d, 200000.d, 500000.d};
	map<string, double> parts;
	map<string, double> masterSaveParts;
	GameSettings currentSettings;
	void updateHome() {
		if(homeDir != "") return;
#ifdef __linux__
    if ((homeDir = getenv("HOME")) == "") {
		homeDir = getpwuid(getuid())->pw_dir;
	}
	return;
#elif defined(_WIN32)
    homeDir = getenv("HOMEDRIVE");
    const char* Hhmepath = getenv("HOMEPATH");
    homeDir = malloc(strlen((char*)homeDir)+strlen(Hhmepath)+1);
    homeDir += Hhmepath;
	return;
#endif
		cout << "Cannot determine home directory, returning '~'" << endl;
		homeDir = "~";
	}
	string getSaveDirectory() {
		updateHome();
		return homeDir + "/" + saveDirectory;
	}
	string getSavePath() {
		updateHome();
		return homeDir + "/" + saveDirectory + savePath;
	}
	void resetParts() {
		parts.clear();
		for(int i = 0; i < sizeof(defaultValues)/sizeof(defaultValues[0]); i++) {
			parts.insert(std::pair<string, double>(defaultValues[i], -1.d));
		}
	}
	void create(string file) {
		if(file == getSaveDirectory()) return;
		if(!directoryExists(getSaveDirectory())) {
			system(("mkdir " + getSaveDirectory()).c_str());
		}
		
		std::ofstream outfile(file);
		outfile << "";
		
		outfile.close();
		
		//resetParts();
	}
	void read(string file, GameSettings s) {
		updateHome();
		currentSettings = s;
		if(fileExists(file)) {
			std::ifstream in(file);
			std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			in.close();
			int len = contents.length();
			char* charr = (char*)contents.c_str();
			
			for(int i = 0; i < len; i++) {
				charr[i] = ~charr[i];
			}
			contents = string(charr);
			//resetParts();
			
			vector<string> partss = split(contents, ";");
			for(int i = 0; i < partss.size(); i++) {
				parts[split(partss[i], ":", true)] = stod(split(partss[i], ":", false));
			}
		} else {
			create(file);
			parts["money"] = 0.d;
			parts["power"] = 1.d;
			parts["hits"]  = 30.d;
		}
	}
	void save(map<string, double> vec, string filepath) {
		FILE *file = fopen(filepath.c_str(), "w");
		
		std::vector<string> keys;
		for(std::map<string, double>::iterator it = vec.begin(); it != vec.end(); it++) {
			keys.push_back(it->first);
		}
		
		string toEnter = "";
		
		for(int i = 0; i < vec.size(); i++) {
			toEnter += keys[i] + ":" + to_string(vec[keys[i]]) + (i <= vec.size()-1 ? ";" : "");
		}
		
		int len = toEnter.length();
		char* charr = (char*)toEnter.c_str();
						
		for(int i = 0; i < len; i++) {
			charr[i] = ~charr[i];
		}

		int results = fputs(charr, file);
		fclose(file);
		if (results == EOF) {
			cout << "Saving file to " + filepath + " failed! Exiting.";
			exit(EXIT_FAILURE);
		}
	}
	void save(string filepath) {
		save(parts, filepath);
	}
	void masterSave() {
		string filepath = getSaveDirectory() + masterSaveFile;
		FILE *file = fopen(filepath.c_str(), "w");
		
		std::vector<string> keys;
		for(std::map<string, double>::iterator it = masterSaveParts.begin(); it != masterSaveParts.end(); it++) {
			keys.push_back(it->first);
		}
		
		string toEnter = "";
		
		for(int i = 0; i < masterSaveParts.size(); i++) {
			toEnter += keys[i] + ":" + to_string(masterSaveParts[keys[i]]) + (i <= masterSaveParts.size()-1 ? ";" : "");
		}
		
		int len = toEnter.length();
		char* charr = (char*)toEnter.c_str();
						
		for(int i = 0; i < len; i++) {
			charr[i] = ~charr[i];
		}
		if(!fileExists(filepath)) {
			cerr << "File " << filepath << " does not exist! Exiting." << endl;
			exit(EXIT_FAILURE);
		}
		int results = fputs(charr, file);
		fclose(file);
		if (results == EOF) {
			cout << "Saving file to " + filepath + " failed! Exiting.";
			exit(EXIT_FAILURE);
		}
	}
	void masterLoad() {
		string file = getSaveDirectory() + masterSaveFile;
		updateHome();
		if(fileExists(file)) {
			std::ifstream in(file);
			std::string contents((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
			in.close();
			int len = contents.length();
			char* charr = (char*)contents.c_str();
			
			for(int i = 0; i < len; i++) {
				charr[i] = ~charr[i];
			}
			contents = string(charr);
			masterSaveParts.clear();
			for(int i = 0; i < sizeof(defaultValues)/sizeof(defaultValues[0]); i++) {
				masterSaveParts.insert(std::pair<string, double>(masterDefaultValues[i], 0.d));
			}
			
			vector<string> partss = split(contents, ";");
			for(int i = 0; i < partss.size(); i++) {
				masterSaveParts[split(partss[i], ":", true)] = stod(split(partss[i], ":", false));
			}
		} else {
			create(file);
		}
		if(masterSaveParts["xpLevel"] <= 0.d) {
			masterSaveParts["xpLevel"] = 1.d;
		}
	}
	void init() {
		if(!fileExists(getSaveDirectory() + masterSaveFile)) {
			if(!directoryExists(getSaveDirectory())) {
				system(("mkdir " + getSaveDirectory()).c_str());
			}
			
			std::ofstream outfile(getSaveDirectory() + masterSaveFile);
			outfile << "";
			
			outfile.close();
		}
		masterSaveParts.clear();
		for(int i = 0; i < sizeof(defaultValues)/sizeof(defaultValues[0]); i++) {
			masterSaveParts.insert(std::pair<string, double>(masterDefaultValues[i], -1.d));
		}
	}
	double getXPforLevel(int level) {
		//return 1000.d*pow(2, level);
		if(level < sizeof(xpLevels)/sizeof(double)) {
			return xpLevels[level-1];
		} else {
			return 100000.d*(level-sizeof(xpLevels)/sizeof(double));
		}
	}
	double getXPforLevel() {
		return getXPforLevel((int)masterSaveParts["xpLevel"]);
	}
}