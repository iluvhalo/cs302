// Matt Matto
// Lab 2
// counts the valid guesses for SRM-496

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <map>
#include <set>
#include <iostream>
#include <cstdio>
#include <cstdlib>

using namespace std;

class Ball {					// given class to hold Balls
	public:
		int val;				
		Ball *low;
		Ball *high;
};

class OneDimensionalBalls {		// given class for OneDimensionalBalls
  public:
    long long countValidGuesses(vector <int> firstPicture, vector <int> secondPicture);
    long long cvg(int v);
	vector <int> FP;
    vector <int> SP;
    vector <int> Matched;
    map <int, int> SP_Balls;

};

long long OneDimensionalBalls::cvg(int v) {				

	// variable declarations
	Ball *ball;													// used to fill the FP and SP maps
	int i, j, k, lowindex, low, high, highindex, groupSize;     // various ints used for calculations
	bool IChangedSomething;										// used to check if I changed something during the process of elimination
	long long key, returnValue = 1;								// used to calculate the return value
	map <int, Ball*> FPm;										// map of the first picture
	map <int, Ball*> SPm;										// map of the second picture
	map <int, Ball*>::iterator FPit;							// iterator for the first picture map
	map <int, Ball*>::iterator SPit;							// iterator for the second picture map

	FPm.clear();												// clear FPm and SPm just to be safe
	SPm.clear();												// not that there is any reason for these to be filled

	
	// fills up the FP map
	for (i = 0; i < FP.size(); i++) {							// iterates through the FP vector
		ball = new Ball;										// creates a new Ball object
		ball->val = FP[i];										// sets the value of the Ball object to the value in the FP vector
		FPm.insert(make_pair(FP[i], ball));						// inserts the new Ball into the map, keyed to its value
	}

	for (i = 0; i < SP.size(); i++) {							// iterates through the SP vector
		ball = new Ball;										// creates a new instance of a Ball
		ball->val = SP[i];										// sets the value of the new Ball
		ball->low = NULL;										// intializes the low
		ball->high = NULL;										// and high values of the new Ball to NULL
		SPm.insert(make_pair(SP[i], ball));						// inserts the new Ball into the SP map, keyed to its value
	}


	for (FPit = FPm.begin(); FPit != FPm.end(); FPit++) {		// iterates through the FP map
		low = FPit->second->val - v;							// calculates what the low value should be
		high = FPit->second->val + v;							// calculates what the high value should be

		SPit = SPm.find(low);									// searches for the low value in the SP map
		if (SPit == SPm.end()) {								// if it is not found
			FPit->second->low = NULL;							// sets the FP ball's low pointer to NULL
		} else if (SPit != SPm.end()) {							// else it finds it
			FPit->second->low = SPit->second;					// sets the ball in FP to point to the ball in SP
			SPm[SPit->first]->low = FPit->second;				// and sets the ball in SP to point back to the ball in FP
		}

		SPit = SPm.find(high);									// searches for the high value in the SP map
		if (SPit == SPm.end()) {								// if it is not found
			FPit->second->high = NULL;							// sets the FP ball's high pointer to NULL
		} else if (SPit != SPm.end()) {							// else if it finds it
			FPit->second->high = SPit->second;					// it sets the FP ball's high pointer to point to the ball in SP
			SPm[SPit->first]->high = FPit->second;				// and the ball in SP points back to the ball in FP
		}
	}
/*
//----------------------------------------------------------------------------------
	printf("First Picture:\n");
	for (FPit = FPm.begin(); FPit != FPm.end(); FPit++) {
		printf("	Ball #%d:\n", FPit->first);
		if (FPm[FPit->first]->low == NULL) {
			printf("		Low: NULL\n");
		} else {
			printf("		Low: %d\n", FPm[FPit->first]->low->val);
		}
		if (FPm[FPit->first]->high == NULL) {
			printf("		High: NULL\n");
		} else {
			printf("		High: %d\n", FPm[FPit->first]->high->val);
		}
	}
	printf("Second Picture:\n");
	for (SPit = SPm.begin(); SPit != SPm.end(); SPit++) {
		printf("	Ball #%d:\n", SPm[SPit->first]->val);
		if (SPm[SPit->first]->low == NULL) {
			printf("		Low: NULL\n");
		} else {
			printf("		Low: %d\n", SPm[SPit->first]->low->val);
		}
		if (SPm[SPit->first]->high == NULL) {
			printf("		High: NULL\n");
		} else {
			printf("		High: %d\n", SPm[SPit->first]->high->val);
		}
	}

//------------------------------------------------------------------------------------------
*/
	// checks the pointers of the first picture
	// if both of them are NULL, returns 0
	for (FPit = FPm.begin(); FPit != FPm.end(); FPit++) {								// iterates through the FP map
		if ((FPm[FPit->first]->low == NULL) and (FPm[FPit->first]->high == NULL)) {		// if a ball's low and high pointer both equal NULL
			return 0;																	// then there are no possible guesses so we return 0;
		}
	}

	// cleans up useless balls in the second picture map
	for (SPit = SPm.begin(); SPit != SPm.end(); SPit++) {								// iterates through the SP map
		if ((SPm[SPit->first]->low == NULL) and (SPm[SPit->first]->high == NULL)) {		// if a ball's low and high pointer both equal NULL
			SPm.erase(SPit);															// then it gets rid of them as there is no longer a point to care about them anymore
		}
	}

/*


	    my high-low notation may be confusing
	    if my pointer formation is:
			    12
			   /  \
		     11    13
	
	    Then, 12.low points to the 11 
			 12.high points to the 13
	
	    But, I have the next one kind of backwards
	
			11.high points to NULL as 11 is not the high of anything
			11.low points to 12 as 11 is the low of 12
	
			13.high points to 12 as it is the high of 12
		    13.low points to NULL
	
		I really should fix it, but it's a little late for that without running the risk of breaking everything
	

*/
	IChangedSomething = true;																// initializes my bool to true
		
	while (IChangedSomething) {
		IChangedSomething = false;															// immediately changes my bool to false
		
		for (FPit = FPm.begin(); FPit != FPm.end(); FPit++) {								// iterates through the FP map
				
			if ((FPm[FPit->first]->low != NULL) and (FPm[FPit->first]->high == NULL)) {		// if the ball my iterator is on has a low pointer, but no high pointer
																							// in this case, ball 12
				IChangedSomething = true;				//		10    12					// changes my bool
				ball = FPm[FPit->first]->low->high;		//     /  \  / 						// sets a pointer to be equal to
														//    9    11						// the pointer from ball 11 to ball 10
				if (ball != NULL) {															// if that pointer != NULL, then another ball points to ball 11
					if (ball->low == NULL) {												// if the ball (10) is only pointing to ball 11, then I would have a case like :
						return 0;																	//		10    12
					}																				//        \  /
																									//         11         which is not possible and should return 0;
																										


					FPm[FPit->first]->low->low = NULL;										// back to the FP{ 10, 12 }, SP{ 9, 11} picture. Sets 11's pointer to 12 to NULL
					FPm.erase(FPit);														// erases 12, making the new system:
																									//		10
																									//	   /  \
																									//	  9    11


				} else {																	// else it is a case like :
																									//          12
																									//		   /
																									//		 11
										
					SPit = SPm.find(FPm[FPit->first]->low->val);							// searches the SP map for ball 11
					
					if (SPit != SPm.end()) {												// if ball 11 is found (which it should)
						SPm.erase(SPit);													// it erases ball 11
					}

					FPm.erase(FPit);														// then it erases ball 12
				}
			}
			else if ((FPm[FPit->first]->high != NULL) and (FPm[FPit->first]->low == NULL)) {// if the ball my iterator is on has a high pointer, but no low pointer
																							// in this case ball 8
				IChangedSomething = true;				//    8   10						// changes my bool
				ball = FPm[FPit->first]->high->low;		//     \ /  \						// sets a pointer to be equal to
														//      9    11						// the pointer from ball 9 to ball 10
				if (ball != NULL) {															// if the pointer != NULL, then another ball points to ball 9
					if (ball->high == NULL) {												// if ball 10 is only pointing to ball 9, then I would have a case like:
						return 0;																	//		8   10
					}																				//       \ /
																									//        9			which is not possible and should return 0;
					
					FPm[FPit->first]->high->high = NULL;									// back to the FP{ 8, 10 }, SP{ 9, 11 } picture, Set's 9's pointer to 8 to NULL
																							
					FPm.erase(FPit);														// erases 8, making the new system:
																									//		10
																									//	   /  \
																									//	  9    11
				
				} else {																	// else if it a case like:
																									//		8
																									//		 \
																									//		  9

					SPit = SPm.find(FPm[FPit->first]->high->val);							// searches the SP map for ball 9

					if (SPit != SPm.end()) {												// when it finds ball 9
						SPm.erase(SPit);													// it erases it
					}		

					FPm.erase(FPit);														// then it finally erases 8
				} // end of else
			} // end of else if
		} // end of for
	} // end of while

	// if everything canceled out, that means there was only one possible guess
	if (FPm.size() == 0) return 1;


	// initializes my returnValue and groupSize variables
	returnValue = 1;
	groupSize = 1;

	// counts of the groups then *= the (groupSize + 1) with returnValue
	for (FPit = FPm.begin(); FPit != FPm.end(); FPit++) {							// iterates through the FP map						5   7
		if (FPm[FPit->first]->high->low == NULL) {									// if FPit = 7									   / \ / \ /
			returnValue *= (groupSize + 1);											// checks 8's hanging pointer					  4   6   8	
			groupSize = 1;															// if it is NULL, that means it has reached then end of a group and calculates returnValue
		}																			// then resets groupSize for the next group


		else if (FPm[FPit->first]->high->low->low == FPm[FPit->first]->high) {		// if Fpit = 5
			groupSize++;															// it checks if 7's pointer to 6 is equal to 5's pointer to 6
		}																			// this means it is still in the middle of a group, so it increments groupSize
	}
	
	return returnValue;																// returns returnValue
}

// given function
long long OneDimensionalBalls::countValidGuesses(
      vector <int> firstPicture, 
      vector <int> secondPicture)
{
	int i, d;
	set <int> velocities;
	set <int>::iterator vit;
	long long rv;

	FP = firstPicture;
	SP = secondPicture;
	Matched.resize(SP.size(), 0);
	for (i = 0; i < SP.size(); i++) SP_Balls[SP[i]] = i;

	for (i = 0; i < SP.size(); i++) {
		d = SP[i] - FP[0];
		if (d < 0) d = -d;
		if (d != 0) velocities.insert(d);
	}

	rv = 0;

	for (vit = velocities.begin(); vit != velocities.end(); vit++) {
		rv += cvg(*vit);
	}
	return rv;
}
