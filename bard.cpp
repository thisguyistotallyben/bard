// Author:  Benjamin Johnson (AB3NJ)
// Date:    July 30, 2017
// Purpose: Ben's Logger for logging contacts on amateur radio


#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
//#include "bard_utils.h"
#include "ncu.h"

// globals
enum Mode {ESC, CASUAL, SEARCH};

vector<Mode> modeStack;
bool CMD;
NCU ncu;
Mode currMode, prevMode;

// prototypes
void cmdSetup();
void cmdLoop();
void push(Mode m);
void pop();

// mode prototypes
void escMenu();


int main(int argc, char **argv) {
	// setup
	cmdSetup();

	// cmd command loop
	cmdLoop();

	// end ncu
	ncu.end();
			
	return 0;
}

void cmdSetup() {
	// start ncu
	ncu.start();
	ncu.hideCursor();

	// setup windows
	ncu.addElement("navbar", NCU_BORDER_BOX, ncu.width(), 3, 0, 0);
	ncu.addElement("escmenu", NCU_BORDER_BOX, ncu.width()/2, ncu.height()/2,
										   ncu.width()/2-(ncu.width()/4),
										   ncu.height()/2-(ncu.height()/4));
	ncu.addElement("casualbase", NCU_BORDERLESS_BOX, ncu.width(), ncu.height()-6, 0, 3);
	ncu.addElement("casual", NCU_BORDER_BOX, ncu.width()/2, ncu.height()/2, 0, 3);

	// decorate
	ncu.addTitle("escmenu", "ESC Menu (ugly and temporary)");
	ncu.write("escmenu", "'c' - Casual Mode", 2, 2);
	ncu.write("escmenu", "'q' - quit", 2, 3);
}

void cmdLoop() {

	escMenu();
}

void escMenu() {
	bool q = false;

	// push
	push(ESC);

	while (1) {
		switch (getch()) {
			case 'q':
				q = true;
			case 'c':
				pop();
				break;
			case 27:
				push(ESC);
		}
		if (q) break;
	}

	// pop
	pop();
}



void push(Mode m) {
	modeStack.push_back(m);
	switch (m) {
		case ESC:
			ncu.showElement("escmenu");
	}
}

void pop() {
	if (modeStack.size() > 0) {
		switch(modeStack[modeStack.size()-1]) {
			case ESC:
				ncu.hideElement("escmenu");
		}

		modeStack.pop_back();
		
	}
}





#if 0
#include "ncurses_utils.h"
using namespace std;

// prototypes
void setupWindows();
void commandLoop();
void command(bool cb);
void command_input();
void esc_menu();
void mode(Mode m);
void search();
void casual(Person *p);
void printInfo(Person *p);
void cprintPerson(Person *p, Contact *c);

// some global crap (blah blah bad practice blah blah)
Bard b;
NcursesUtils ncu;
Mode prevMode;
bool beginb = true; // kind of stupid, but for top and bottom bars

int main() {
	string s;

	// setup
	b.init("bard_data");
	ncu.init();

	// setup windows
	setupWindows();
	curs_set(0);

	commandLoop();
	
	endwin();
}

void setupWindows() {
	string cancelmess = "Are you sure you want to cancel? (y/n)";
	// winder creationism
	ncu.addWin("top", 3, COLS, 0, 0, BOX);
	ncu.addWin("bottom", 3, COLS, LINES - 3, 0, BOX);

	ncu.addWin("searchtext", 3, 10, 3, 0, NO_BOX);
	ncu.addWin("searchbox", 3, COLS / 2 - 10, 3, 10, INPUT_BOX);
	ncu.addWin("call_right", LINES - 6, COLS / 2, 3, COLS - COLS / 2, BOX);
	ncu.addWin("searchresults", LINES - 9, COLS / 2, 6, 0, NO_BOX);

	ncu.addWin("casualselected", 3, COLS / 2, 3, 0, BOX);
	ncu.addWin("casualinput", 3, COLS / 2, 6, 0, INPUT_BOX);
	ncu.addWin("casualoverviewtop", /*LINES - 6 - 7*/11, COLS / 2, 3, COLS - COLS / 2, BOX);
	ncu.addWin("casualoverviewbottom", 7, COLS / 2, /*LINES - 10*/14, COLS - COLS / 2, BOX);
	ncu.addWin("casualmessage", LINES - 12, COLS / 2, 9, 0, NO_BOX);

	ncu.addWin("cancel", 3, COLS, LINES / 2, 0, NO_BOX);

	ncu.addWin("esc", LINES, 30, 0, 0, NO_BOX);

	// build escape menu
	wbkgd(ncu.getWin("esc"), COLOR_PAIR(1));
	mvwhline(ncu.getWin("esc"), 1, 0, 0, 30);
	mvwhline(ncu.getWin("esc"), 7, 0, 0, 30);

	// other stuff
	wbkgd(ncu.getWin("cancel"), COLOR_PAIR(1));
	mvwprintw(ncu.getWin("searchtext"), 1, 2, "Search");
	mvwprintw(ncu.getWin("esc"), 1, 2, " Modes ");
	mvwprintw(ncu.getWin("esc"), 3, 3, "c: Casual");
	mvwprintw(ncu.getWin("esc"), 4, 3, "C: Contest");
	mvwprintw(ncu.getWin("esc"), 5, 3, "n: Net");
	mvwprintw(ncu.getWin("esc"), 9, 3, "u: Set User Info");
	mvwprintw(ncu.getWin("esc"), 10, 3, "s: Search");
	mvwprintw(ncu.getWin("esc"), 11, 3, "q: Quit");
	
	mvwprintw(ncu.getWin("cancel"), 1, (COLS - cancelmess.size()) / 2, cancelmess.c_str());

	// layout creationism
	vector<string> v = {"top", "bottom"};
	ncu.addLayout("always", v, false, false);
	v = {"esc"};
	ncu.addLayout("esc", v, true, false);
	v = {"searchtext", "searchbox", "searchresults"};
	ncu.addLayout("search", v, true, true);
	v = {"casualselected", "casualinput", "casualoverviewtop", "casualoverviewbottom", "casualmessage"};
	ncu.addLayout("casual", v, true, true);
	v = {"cancel"};
	ncu.addLayout("cancel", v, true, false);
}

void commandLoop() {
	char c;
	string tmp;
 
	// start in escape mode
	mode(ESC);

	while (1) {
		switch (b.getMode()) {
			case ESC:
				switch (getch()) {
					case 'q':
						return;
						break;
					case 's':
						mode(SEARCH);
						break;
					case 'c':
						mode(CASUAL);
						break;
					case 27:
						mode(prevMode);
						break;
					default: break;
				}
				break;
			case SEARCH:
				search();
				mode(ESC);
				break;
				break;
			case CASUAL:
				casual(NULL);
				mode(ESC);
				break;
			default: break;
		}
	}
}

// function functions --------------------------------------------------------

void mode(Mode m) {
	prevMode = b.getCurrMode();
	// set the mode
	b.setMode(m);
	int result;

	if (m != ESC && beginb == true) {
		ncu.showLayout("always");
		beginb = false;
	}
	else if (m != ESC && beginb == false) {
		ncu.clearWin("top");
		ncu.clearWin("bottom");
	}

	switch(m) {
		case SEARCH:
			ncu.printWin("top", 1, 2, "Search Callsign Database");
			ncu.printWin("bottom", 1, 2, "s: New Search  n: Create Log  e: Edit Contact");
			ncu.showLayout("search");
			break;
		case ESC:
			ncu.showLayout("esc");
			break;
		case CASUAL:
			ncu.printWin("top", 1, 2, "Casual Mode");
			ncu.printWin("bottom", 1, 2, "c: Call Sign  n: Name  l: Location  s: Signal Sent  r: Signal Received  t: Time  T: Custom Time  f: Finish");
			ncu.showLayout("casual");
			break;
		default:
			break;
	}
	
}

void search() {
	Person *p = NULL;
	string s;

	while (1) {
		ncu.clearWin("searchbox");

		switch(getch()) {
			case 's':
				ncu.clearWin("searchresults");
				ncu.clearWin("searchbox");

				s = ncu.getInput("searchbox");
				if (s != "") {
					b.cap(s);
					p = b.search(s);
					if (p != NULL) printInfo(p);
					else ncu.printWin("searchresults", 1, 2, "No results."); 
				}
				break;
			case 'n':
				if (p == NULL) ncu.printWin("searchresults", 1, 2, "Enter a valid callsign to start a log");
				else {
					mode(CASUAL);
					casual(p);
					mode(SEARCH);
				}
				break;
			case 27:
				return;
			default: break;
		}
	}
}

void casual(Person *passp) {
	int i, choice;
	char ch;
	bool timeb = false;
	string s;
	stringstream ss;
	Person *p, *ptmp;
	Contact *c;

	// setup
	p = new Person;
	c = new Contact;
	c->time = new Time;
	if (passp == NULL) {
		p->call.resize(1);
	}
	else {
		*p = *passp;
	}
	if (p->location.size() != 0) c->location = 0;

	while (1) {
		// update screen
		ncu.clearWin("casualselected");
		ncu.clearWin("casualoverviewtop");
		cprintPerson(p, c);

		switch (getch()) {
			case 'c':
				ncu.printWin("casualselected", 1, (COLS/2-4)/2, "Callsign");
				s = ncu.getInput("casualinput");
				if (s != "") {
					b.cap(s);
					// search for call sign and add info if found
					ptmp = b.search(s);
					if (ptmp == NULL) p->call[0] = s;
					else {
						*p = *ptmp;
						if (p->location.size() != 0) c->location = 0;
					}
				}
				break;
			case 'n':
				ncu.printWin("casualselected", 1, (COLS/2-4)/2, "Name");
				s = ncu.getInput("casualinput");
				if (s != "") p->name = s;
				break;
			case 't':
				if (timeb) {
					ncu.printWin("casualmessage", 1, 2, "Time already recorded. Overwrite? (y/n)");
					if (getch() == 'y') c->time = b.getTime();
					ncu.clearWin("casualmessage");
				}
				else {
					c->time = b.getTime();
					timeb = true;
				}
				break;
			case 'l':
				ncu.printWin("casualselected", 1, ((COLS/2) - 8) / 2, "Location");
				if (p->location.size() == 0) {
					s = ncu.getInput("casualinput");
					if (s != "") {
						p->location.push_back(s);
						c->location = 0;
					}
				}
				else {
					ss.str(""); // for cleanliness sake
					ncu.printWin("casualmessage", 1, 2, "Choose a location (or esc to cancel):");
					wmove(ncu.getWin("casualmessage"), 2, 1);
					whline(ncu.getWin("casualmessage"), 0, COLS/2 - 2);

					for (i = 0; i < p->location.size(); i++) {
						ss << i + 1 << ": " << p->location[i];
						ncu.printWin("casualmessage", i + 3, 2, ss.str());
						ss.str("");
					}
					ss << i+1 << ": + New Location";
					ncu.printWin("casualmessage", p->location.size() + 3, 2, ss.str());

					while (1) {
						ch = getch();
						if (ch == 27) break; // esc the madness
						choice = ch - '0';
						cout << choice << endl;
						if (choice > 0 && choice < p->location.size() + 1) {
							c->location = choice - 1;
							break;
						}
						else if (choice == p->location.size() + 1) {
							s = ncu.getInput("casualinput");
							p->location.push_back(s);
							c->location = p->location.size() - 1;
							break;
						}
					}	
				}
				ncu.clearWin("casualmessage");
				break;
			case 27:
				ncu.showLayout("cancel");
				if (getch() == 'y') {
					ncu.hideLayout("cancel");
					return;
				}
				ncu.hideLayout("cancel");
				break;
			default: break;
		}
	}
}

void cprintPerson(Person *p, Contact *c) {
	string cw = "casualoverviewtop";
	string you = "casualoverviewbottom";
	int cl = 0;
	stringstream ss;

	ncu.printWin(cw, cl++, 1, " Contact ");
	cl++;

	ncu.printWin(cw, cl, 2, "Callsign:");
	if (p->call.size() != 0) ncu.printWin(cw, cl, 19, p->call[0]);
	cl++;
	ncu.printWin(cw, cl++, 2, "Name:            " + p->name);
	ncu.printWin(cw, cl, 2, "Location:");
	if (p->location.size() != 0 && c->location != -1)
		ncu.printWin(cw, cl, 19, p->location[c->location]);
	cl++;
	ss << c->sigs;
	ncu.printWin(cw, cl++, 2, "Report Sent:     " + ss.str());
	ss.clear(); ss.str(""); ss << c->sigr;
	ncu.printWin(cw, cl++, 2, "Report Received: " + ss.str());
	cl++;
	ncu.printWin(cw, cl, 2, "Time: ");
	ncu.printWin(cw, cl+1, 2, "Date: ");
	if (c->time->hour != -1) {
		// time
		ss.clear(); ss.str("");
		if (c->time->hour < 10) ss << 0; ss << c->time->hour << ":";
		if (c->time->minute < 10) ss << 0; ss << c->time->minute;
		ncu.printWin(cw, cl++, 19, ss.str());

		// date
		ss.str("");
		if (c->time->day < 10) ss << 0; ss << c->time->day << ".";
		if (c->time->month < 10) ss << 0; ss << c->time->month << ".";
		ss << c->time->year;
		ncu.printWin(cw, cl++, 19, ss.str());
	}

	// you part
	cl = 0;
	ncu.printWin(you, cl++, 1, " You ");
	cl++;

	ncu.printWin(you, cl++, 2, "Location:         " + b.userInfo->location[b.userInfo->cur[0]]);
	ncu.printWin(you, cl++, 2, "Radio:            " + b.userInfo->radio[b.userInfo->cur[1]]);
	ss.str("");
	ss <<                      "Power:            " << b.userInfo->cur[2];
	ncu.printWin(you, cl++, 2, ss.str());
	ncu.printWin(you, cl++, 2, "Mode:             " + b.getRadioMode(b.userInfo->cur[3]));
}

void printInfo(Person *p) {
	string cw = "searchresults"; // for my sake
	stringstream ss;
	int cl = 1;

	ncu.printWin(cw, cl++, 2, "CALL: " + p->call[0]);
	if (p->call.size() > 1) {
		ncu.printWin(cw, cl, 2, "PREVIOUS CALL(S): ");
		for (int i = 1; i < p->call.size(); i++) {
			ncu.printWin(cw, cl++, 20, p->call[i]);
		}
	}
	ncu.printWin(cw, cl++, 2, "Location: " + p->location[0]);
	cl++;
	ss << p->contact.size() << " previous contacts";
	ncu.printWin(cw, cl++, 2, ss.str());
}
#endif
