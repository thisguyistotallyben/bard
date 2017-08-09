#ifndef BL_UTILS_H
#define BL_UTILS_H

#include <iostream>
#include <vector>
#include <map>
#include <ctime>
using namespace std;


// variables ----------------------------------------------------

enum Mode {ESC, SEARCH, CASUAL};

// classes ------------------------------------------------------

class Time {
	public:
		Time();
		int hour;
		int minute;
		int day;
		int month;
		int year;
};
class Contact {
	public:
		Contact();
		Time *time;

		// user section
		float frequency;
		int power;
		int radio;                  // index of user radio
		string *mode;
		
		int sigs;
		int sigr;

		int location;               // index of location
};

class Person {
	public:
		// copy constructor
		Person(const Person &obj);
		Person();

		vector<string> call;
		string name;
		vector<string> location;
		vector<Contact*> contact;
};

class User {
	public:
		string call;
		string name;
		vector<string> location;
		vector<string> radio;
		int def[4]; // location, radio, power, mode (0:ssb 1:cw 2:fm
					//							     3:am 4:sat)
		int cur[4];
};

class Bard {
	private:
		map<string, Person*> people;
		Mode MODE;

	public:
		User *userInfo;

		int init(string fn);
		// MADE THESE UP/LEFT OFF HERE				TODO
		int insertPerson(Person *p);
		int insertContact(string name, Contact *);
		Time* getTime();
		// ----------------------------------------------
		void print();
		void setMode(Mode);
		Mode getCurrMode();
		Person* search(string call);
		void log(Person *p);
		void cap(string &s);
		
		string getRadioMode(int i);
		Mode getMode();
};

#endif
