#include <iostream>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ctime>
#include <cstring>
#include <ncurses.h>
#include "bard_utils.h"
using namespace std;

// prototypes
void spaceitup(string &s);
void printperson(Person *p);
void printpersonnc(Person *p);

// constructors and things

Person::Person(const Person &obj) {
	call = obj.call;
	name = obj.name;
	location = obj.location;
}

Person::Person() { }

Contact::Contact() {
	location = -1;
	sigs = 59;
	sigr = 59;
}

Time::Time() {
	hour = -1;
}

// functions

int Bard::init(string f) {
	int tmp, i;
	string line, tmps;
	ifstream in;
	istringstream inss;
	Person  *p;
	Contact *c;
	
	// setup
	in.open(f);
	MODE = ESC;

	// read in user info
	userInfo = new User;
	getline(in, line);
	inss.str(line);

	inss >> userInfo->call >> userInfo->name;
	inss >> tmp;
	for (i = 0; i < tmp; i++) {
		inss >> tmps;
		spaceitup(tmps);
		userInfo->location.push_back(tmps);
	}
	inss >> tmp;
	for (i = 0; i < tmp; i++) {
		inss >> tmps;
		spaceitup(tmps);
		userInfo->radio.push_back(tmps);
	}
	inss.clear();
	getline(in, line);
	inss.str(line);
	for (i = 0; i < 4; i++) inss >> userInfo->def[i];
	memcpy(userInfo->cur, userInfo->def, sizeof(userInfo->def));
	inss.clear(); // cleanse
	
	// read in people
	while (getline(in, line)) {
		p = new Person;
		inss.str(line);

		// calls
		inss >> tmp;
		for (i = 0; i < tmp; i++) {
			inss >> tmps;
			p->call.push_back(tmps);
		}

		// name
		inss >> p->name;
		spaceitup(p->name);
		
		// locations
		inss >> tmp;
		for (i = 0; i < tmp; i++) {
			inss >> tmps;
			spaceitup(tmps);
			p->location.push_back(tmps);
		}

		// contacts
		inss >> tmp;
		for (i = 0; i < tmp; i++) {
			getline(in, line);
			inss.clear();
			inss.str(line);
			c = new Contact;
			//inss >> c->day >> c->month >> c->year >> c->location;
			p->contact.push_back(c);
		}

		// clean up
		//printperson(p);
		for (i = 0; i < p->call.size(); i++) {
			people.insert(make_pair(p->call[i], p));
		}
		inss.clear();
	}
	return 0;
}

void spaceitup(string &s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '_') s[i] = ' ';
	}
}

/*
void printperson(Person *p) {
	for (int i = 0; i < p->call.size(); i++) {
		if (i == 0) cout << "call: ";
		else cout << "  previous: ";
		cout << p->call[i] << endl;
	}
	cout << "name: " << p->name << endl;
	cout << "locations:\n";
	for (int i = 0; i < p->location.size(); i++) {
		cout << "  " << i+1 << ": " << p->location[i] << endl;
	}
	cout << "contacts:\n";
	for (int i = 0; i < p->contact.size(); i++) {
		cout << "  date: " << p->contact[i]->day << "."
			               << p->contact[i]->month << "."
						   << p->contact[i]->year << "\n";
		cout << "  location: " << p->location[p->contact[i]->location] << endl << endl;
	}
	cout << endl;
}
*/

void Bard::print() {
	map<string, Person*>::iterator pit;
	/*
	for (int i = 0; i < people.size(); i++) {
		printpersonnc(people[i]);
	}
	*/
	for (pit = people.begin(); pit != people.end(); pit++) {
		cout << pit->first;
	}
}

void Bard::setMode(Mode m) {
	MODE = m;
}

Mode Bard::getCurrMode() {
	return MODE;
}

Mode Bard::getMode() {
	return MODE;
}

void Bard::cap(string &s) {
	for (int i = 0; s[i] != '\0'; i++) {
		s[i] = toupper(s[i]);
	}
}

Person* Bard::search(string name) {
	map<string, Person*>::iterator pit;
	pit = people.find(name);
	if (pit == people.end()) return NULL;
	else return pit->second;
}
void Bard::log(Person *p) {
	Person *pl = search(p->name);
	
	// create person
	if (pl == NULL) {
		for (int i = 0; i < p->call.size(); i++) {
			people.insert(make_pair(p->call[i], p));
		}
	}
	// add contact if exists
	else {
		pl->contact.insert(pl->contact.end(),
				            p->contact.begin(),
							p->contact.end());
	}
}

Time* Bard::getTime() {
	time_t t;
	struct tm *ptm;
	Time *ts;

	ts = new Time;

	// get the time
	time(&t);
	ptm = gmtime(&t);

	// set the time
	ts->hour = ptm->tm_hour;
	ts->minute = ptm->tm_min;
	ts->day = ptm->tm_mday;
	ts->month = ptm->tm_mon + 1;
	ts->year = ptm->tm_year + 1900;
	
	return ts;
}

string Bard::getRadioMode(int i) {
	switch (i) {
		case 0: return "SSB";
		case 1: return "CW";
		case 2: return "FM";
		case 3: return "AM";
		case 4: return "SAT";
		default: return "BAD_NUM";
	}
}
