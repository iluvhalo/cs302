// Matt Matto
// Lab 3
// definitions for the el_to_nd() and nd_to_el() functions

#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <list>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include "cs302-midi.h"

using namespace std;

// cleans up my nd_to_el() variable declarations
typedef map <int, multimap <int, Event *>* > TempMap;
typedef multimap <int, Event *> TempMapMultiMap;
	
void CS302_Midi::el_to_nd() {								// variable declarations
	vector <ND *> pitches(128, NULL);						// declares an initializes a vector <ND *> to size 128, and all the pointers to NULL
	ND *note;												// points to my note ND
	ND *damper = NULL;										// points to my damper ND
	int i, k, pit, vol, pedalPosition;						// ints mostly used to make my coade easier to read
	double elapsedTime;										// hold my time for starts and stops
	EventList::iterator elit;								// iterator for el
	
	nd = new NDMap;
	
	elapsedTime = 0.0;										// initializes elapsedTime

	for (elit = el->begin(); elit != el->end(); elit++) {	// iterates through el()
		elapsedTime += (*elit)->time / 480.0;				// increments time and converts to a double
		k = (*elit)->key;									// stores the key
		
		if (k == 'O') {										// checks if the event is an ON event
			pit = (*elit)->v1;								// stores the pitch
			vol = (*elit)->v2;								// stores the volume

			note = new ND;									// declares a new note ND
			note->key = 'N';								// sets the note's key to N
			note->start = elapsedTime;						// sets the note's start time
			note->pitch = pit;								// sets the note's pitch
			note->volume = vol;								// sets the note's volume

			pitches[pit] = note;							// stores the note in the vector of ND pointers
		}
		else if (k == 'F') {								// checks if the event is an OFF event
			pit = (*elit)->v1;								// collects the pitch

			if (pitches[pit] != NULL) {						// checks if the note at the collected pitch is currently set to ON
				note = pitches[pit];						// makes the note pointer point to the note in the vector
				note->stop = elapsedTime;					// adds its stop time
				nd->insert(make_pair(note->start, note));	// inserts the note into the nd map, keyed to its start time
				pitches[pit] = NULL;						// resets the note in the pitch vector
			}
		}
		else if (k == 'D') {											// checks if the event is a DAMPER event
			pedalPosition = (*elit)->v1;								// collects the pedal position

			if ((damper == NULL) and (pedalPosition == 1)) {			// if the damper is new, and its position is DOWN
				damper = new ND;										// declare a new damper
				damper->key = 'D';										// set its key
				damper->start = elapsedTime;							// set it start time
			} 
			else if ((damper != NULL) and (pedalPosition == 0)) {		// if there already exists and damper and its position should be UP
				damper->stop = elapsedTime;								// set the stop time
				nd->insert(make_pair(damper->start, damper));			// insert the damper into the nd map
				damper = NULL;											// reset the damper ND
			}
		}
	} // end of for


}

void CS302_Midi::nd_to_el() {
	el = new EventList;
	NDMap::iterator mit;	// multimap <double, ND *>
	TempMapMultiMap *multi;										// multimap within the temp map pointer
	TempMapMultiMap::iterator mmit;								// mulitmap iterator for the multimap within the temp map
	TempMap temp;												// temporary map to hold the events in order
	TempMap::iterator tempit;									// iterator
	Event *event1;												// pointer to a first event: ON/DAMPER DOWN
	Event *event2;												// pointer to a second event: OFF/DAMPER UP
	int k, p, v, beg, fin, lasttime;							// misc ints

	for (mit = nd->begin(); mit != nd->end(); mit++) {			// iterates through nd
		k = mit->second->key;									// get the key from nd
		if (k == 'N') {											// checks if it is a note
			p = mit->second->pitch;								// stores the pitch of the note
			v = mit->second->volume;							// stores the colume of the note
			beg = rint(mit->second->start * 480.0);				// converts the start time to an int (1/480 second)
			fin = rint(mit->second->stop * 480.0);				// converts the end time to an int (1/480 second)
			if (beg != fin) {									// as long as the start and finish times are not the same
			
				event1 = new Event;								// declares two new events
				event2 = new Event;
			
				event1->key = 'O';								// sets the first event to an ON event
				event1->time = beg;								// sets the time to execute the event
				event1->v1 = p;									// sets the pitch to turn on
				event1->v2 = v;									// sets the volume

				event2->key = 'F';								// sets the second event to an OFF event
				event2->time = fin;								// sets the time to execute the event
				event2->v1 = p;									// sets the pitch to turn off
			
				tempit = temp.find(beg);						// searches the temporary map for the time of the first event

				if (tempit == temp.end()) {						// if there is not an entry for the time of the first event
					multi = new TempMapMultiMap;				// makes a new multimap (key: 4 for insertion priority into el of 4)
					multi->insert(make_pair(4, event1));		// inserts the first event into the multimap
					temp.insert(make_pair(beg, multi));			// inserts the multimap into the temporary map
				} else {										// else the multimap already exists
					multi = tempit->second;						// points to the multimap that the iterator is on
					multi->insert(make_pair(4, event1));		// inserts our event into the multimap
				}

				tempit = temp.find(fin);						// searches the temporary map for the time of the second event
	
				if (tempit == temp.end()) {						// if there is not an entry for the time of the second event
					multi = new TempMapMultiMap;				// makes a new multimap (key: 1 for insertion priority into el of 1)
					multi->insert(make_pair(1, event2));		// inserts the second event into the multimap
					temp.insert(make_pair(fin, multi));			// inserts the multimap into the temporary map
				} else {										// else the multimap already exists
					multi = tempit->second;						// points to the multimap that the iterator is on
					multi->insert(make_pair(1, event2));		// inserts our event into the multimap
				}
			}
		}

		else if (k == 'D') {									// checks if it is a damper
			beg = rint(mit->second->start * 480.0);				// calculates the beginning time
			fin = rint(mit->second->stop * 480.0);				// calculates the finish time

			if (beg != fin) {									// as long as the beginning and finish times aren't equal
				event1 = new Event;								// creates two new events
				event2 = new Event;
	
				event1->key = 'D';								// first event is for when you set the damper DOWN
				event1->time = beg;								// saves the time
				event1->v1 = 1;									// 1 means DOWN
	
				event2->key = 'D';								// second event is for when you set the damper UP
				event2->time = fin;								// saves the time
				event2->v1 = 0;									// 0 means UP
	
				tempit = temp.find(beg);						// searches the map for the time of the first event
	
				if (tempit == temp.end()) {						// if there is no multimap
					multi = new TempMapMultiMap;				// make a new one
					multi->insert(make_pair(3, event1));		// insert the first event into the new multimap 
					temp.insert(make_pair(beg, multi));			// insert the new multimap into the temporary map
				} else {										// else there already is a multimap at the keyed time
					multi = tempit->second;						// set a pointer to that map
					multi->insert(make_pair(3, event1));		// insert the event into that map
				}
	
				tempit = temp.find(fin);						// searches the temp map for the time of the second event
	
				if (tempit == temp.end()) {						// if there is no multimap
					multi = new TempMapMultiMap;				// make a new one
					multi->insert(make_pair(2, event2));		// insert the event into the new multimap
					temp.insert(make_pair(fin, multi));			// insert the new multimap into the temporary map
				} else {										// else there is already a multimap in the temporary map
					multi = tempit->second;						// point to it
					multi->insert(make_pair(2, event2));		// insert the event into the multimap
				}
			}
		} // end of else if
	} // end of for(iterate through nd)
	
	lasttime = 0;														// initailize lasttime

	for (tempit = temp.begin(); tempit != temp.end(); tempit++) {		// iterate through the temporary map
		multi = tempit->second;											// point to the multimap
		for (mmit = multi->begin(); mmit != multi->end(); mmit++) {		// iterate through the multimap
			if (mmit == multi->begin()) {								// if it is the first time through
				lasttime = mmit->second->time - lasttime;				// calculates the time since the last event
				mmit->second->time = lasttime;							// sets the newly calculated time
			} else {													// if it is NOT the first time through
				mmit->second->time = 0;									// sets the time since last event to 0
			}

			el->push_back(mmit->second);								// inserts the event into the Event list
		}

		lasttime = tempit->first;										// saves the total time elapsed before it loops
	}
}
