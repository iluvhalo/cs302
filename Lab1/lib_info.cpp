// Matt Matto
// Lab 1
// Takes a formatted list of songs and puts them into maps and prints out a formatted summary

#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <iostream>
#include <string>

using namespace std;

// given classes to use
class Song {
	public:
		string title;
		int time;
		int track;
};

class Album {
	public:
		map <int, Song *> songs;
		string name;
		int time;
};

class Artist {
	public:
		map <string, Album *> albums;
		string name;
		int time;
		int nsongs;
};

// prototyped functions. StringToTime and TimeToString were described in the writeup
// the other two were ones I made to clean up main a little bit
int StringToTime (string timeString);
string TimeToString (int timeInt);
void DeleteUnderscores(string& line);
void Print(map <string, Artist *>& artists);

// main
int main(int argc, char* argv[]) {

	// declarations
	string line, title, stime, artist, album, genre;
	istringstream buffer;
	ifstream inputFile;
	int track, itime;
	Song* currentSong;
	Artist* currentArtist;
	Album* currentAlbum;
	map <string, Artist *> artists;
	map <string, Artist *>::iterator arit;
	map <string, Album *>::iterator alit;
	map <int, Song *>::iterator sit;
	vector <int> thing;

	cout << thing[3];
	
	// writeup said I didn't have to but I did anyway.
	if (argc != 2) {
		printf("improper input format");
	} // end of if

	// opens the input file
	inputFile.open(argv[1]);
	
	// continuously reads lines from the input file until it is exhausted
	while (getline(inputFile, line)) {
		
		// step 1 - reads the input file into variables using string streams
		buffer.str(line);													
		buffer >> title >> stime >> artist >> album >> genre >> track;
		buffer.clear();
	
		// calculates the time of the song as an int
		itime = StringToTime(stime);

		// step 2 - gets rid of the underscores in the strings
		DeleteUnderscores(title);
		DeleteUnderscores(artist);
		DeleteUnderscores(album);
		DeleteUnderscores(genre);

		// creates a pointer to a Song object and calls new
		currentSong = new Song;
		currentSong->title = title;
		currentSong->time = itime;
		currentSong->track = track;
		
		// step 3 - searches for the artist in the artist map
		// if it was not found, it creates a new artist and adds it to the map
		// otherwise it does nothing
		arit = artists.find(artist);
		if (arit == artists.end()) {
			currentArtist = new Artist;
			currentArtist->name = artist;
			currentArtist->time = 0;
			currentArtist->nsongs = 0;
			arit = artists.insert(make_pair(artist, currentArtist));
			artists.erase(arit);
			artists.erase(arit);
		} // end of if
		
		// resets the iterator back to the artist
		arit = artists.find(artist);
		
		// step 4 - searches the artist's albums to se if it already is there
		// if it is not, it adds the new album
		alit = arit->second->albums.find(album);
		if (alit == arit->second->albums.end()) {
			currentAlbum = new Album;
			currentAlbum->name = album;
			currentAlbum->time = 0;
			arit->second->albums.insert(make_pair(album, currentAlbum));
		} // end of if
		
		// resets the album iterator to the album in the map
		alit = arit->second->albums.find(album);


		// step 5 - searches for the song in the album map of the artist map
		// same as above, if it is not found, it adds it
		sit = alit->second->songs.find(track);
		if (sit == alit->second->songs.end()) {
			alit->second->songs.insert(make_pair(track, currentSong));
			alit->second->time += itime;
			arit->second->time += itime;
			arit->second->nsongs++;
		} // end of if
	} // end of while loop
	
	// prints the data
	Print(artists);

	// fin
	return 0;
} // end of main

// uses string streams to convert the time string into an int
int StringToTime (string timeString) {
	istringstream buffer;
	int minutes, seconds;
	int i, returnTime = 0;

	for (i = 0; i < timeString.length(); i++) {
		if (timeString[i] == ':') timeString[i] = ' ';
	}

	buffer.str(timeString.c_str());
	buffer >> minutes >> seconds;
	buffer.clear();

	returnTime += (minutes * 60) + seconds;

	return returnTime;
} // end of StringToTime

// converts the time int back into a readable time string using string streams
string TimeToString (int timeInt) {
	string returnString;
	ostringstream buffer;
	int minutes, seconds;

	minutes = (timeInt - (timeInt % 60)) / 60;
	seconds = timeInt - (minutes * 60);
	
	buffer << minutes << ':';
	if (seconds < 10) buffer << '0';
	buffer << seconds;
	returnString = buffer.str();

	return returnString;
} // end of TimeToString

// cycles through a passed in string and changes the '_' to ' '
void DeleteUnderscores(string& line) {
	for (int i = 0; i < line.length(); i++) {
		if (line[i] == '_') line[i] = ' ';
	} // end of for
} // end of DeleteUnderscores

// prints the data in the maps
void Print(map <string, Artist *>& artists) {
	map <string, Artist *>::iterator arit;
	map <string, Album *>::iterator alit;
	map <int, Song *>::iterator sit;

	for (arit = artists.begin(); arit != artists.end(); arit++) {
		printf("%s: %d, %s\n", arit->second->name.c_str(), arit->second->nsongs, TimeToString(arit->second->time).c_str());

		for (alit = arit->second->albums.begin(); alit != arit->second->albums.end(); alit++) {
			printf("        %s: %d, %s\n", alit->second->name.c_str(), alit->second->songs.size(), TimeToString(alit->second->time).c_str());

			for (sit = alit->second->songs.begin(); sit != alit->second->songs.end(); sit++) {
				printf("                %d. %s: %s\n", sit->second->track, sit->second->title.c_str(), TimeToString(sit->second->time).c_str());
		
			} // end of song for
		} // end of album for
	} // end of artist for
} // end of Print
