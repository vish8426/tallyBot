#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <algorithm>    // std::find

#define MAX 100

using namespace std;

int main()
{
	int run = 1;
	char ch;
	// string vector
	string data;
	vector<string>dataVector;
	vector<string>oldDataVector;
	// Shelf variables
	int numShelves = 0;
	int chCount = 0;
	int trigger = 0;
	int l = 0;
	char rawData[MAX];
	int endlist = 0;
	int startlist = 0;	
	char inputShelf[MAX];
	char shelf;
	int intShelf;
	char shelfContent[MAX];
	
	// Lookup table
	vector<int> numItemsTable;
	vector<string> itemLookupTable;

	// Open oldFile to read data=====================//
	ifstream file;
	file.open("/home/pi/oldInventory.txt");
	while(file >> data){
		oldDataVector.push_back(data);
	}
	file.close();
	// Load the old data into the itemLookupTable
	for(int i = 0; i < oldDataVector.size(); i++){
		itemLookupTable.push_back(oldDataVector[i]);
		numItemsTable.push_back(0);
	}
	// Clear the old data file=======================//
	ofstream outfile;
	outfile.open("/home/pi/oldInventory.txt", ofstream::out | ofstream::trunc);
	outfile.close();
	// Open datafile for 1st time===================//
	file.open("/home/pi/inventory.txt");
	if(!file)
	{
		cerr << "Unable to open file inventory.txt";
		exit(1); 		
	}
	// Read from the file and store it inside a vector	
	while(file >> data){
		dataVector.push_back(data);
	}
	file.close();
	// make a copy of dataVector
	vector<string> copyVector(dataVector);
	vector<string> copyVector2(dataVector);

	// Open datafile for 2nd time=======================//
	file.open("/home/pi/inventory.txt");
	while(file >> noskipws >> ch){
		if(isdigit(ch)){
			numShelves++;
		}
		rawData[l] = ch;
		chCount++;
		l++;
	}
	rawData[l] = '\n';
	rawData[l+1] = '\0'; 
	file.close();
	
	//==========================First loop to update with new items=============================//
	
	string tempStr;
	int itemFoundFlag = 0;
	// First loop to update the item list if any new item was added
	for( int i = 0; i < dataVector.size(); i++)
	{
		tempStr = copyVector.back();	// store the last element in vector in a variable to compare
		for(int j = 0; j< numItemsTable.size(); j++){	
			if(tempStr.compare( itemLookupTable[j] ) == 0 || tempStr.length() < 2){
				itemFoundFlag = 1;
			}
		}
		// Check for added items
		if(itemFoundFlag == 0){
			cout << "New item added: " << tempStr << endl;
			// Check if the new item is already added
				itemLookupTable.push_back(tempStr);
				numItemsTable.push_back(0);
		}
		// pop the last character so that 
		// second last character will be check next time
		copyVector.pop_back();	
		itemFoundFlag = 0;
	}

	// Write the data to oldFile to check against next time
   	outfile.open("/home/pi/oldInventory.txt");
	for(int i = 0; i < itemLookupTable.size(); i++){
		outfile << itemLookupTable[i] << " ";
	}
	outfile.close();
	
	//=====================Second loop to count the number of items of each kind===================//
	for( int i = 0; i < dataVector.size(); i++)
	{
		tempStr = copyVector2.back();	// store the last element in vector in a variable to compare
		for(int j = 0; j< numItemsTable.size(); j++){	
			if(tempStr.compare( itemLookupTable[j] ) == 0){
				numItemsTable[j] = numItemsTable[j] + 1;
			}
		}
		// pop the last character so that 
		// second last character will be check next time
		copyVector2.pop_back();		
	}
	
	//==================================Start the user interface==============================//
	while(run){
		int input;
		cout << "Select Enquiry (1-3): \n1. Item Count\n2. Check Items On Each Shelf\n3. Exit\n";
		cout << "Input: " ;
		cin >> input;
		cout << "\n";
		
		switch(input){
			case 1:
				int j;
				do{
					// Ask for input
					cout << "Select an item to count (1-"<< numItemsTable.size() << ") \n";
					for(int i = 0; i < numItemsTable.size(); i++){
						cout << i+1 << ". " << itemLookupTable[i] << endl;
					}
					cout << numItemsTable.size() + 1 << ". Back To Main Menu" << endl;
					cout << "Input: " ;
					cin >> j;

					if(j > numItemsTable.size() + 1 || j < 1){		// Filter input for errors
						cout << "Invalid input! Try again" << endl;
					}else if(j < numItemsTable.size() + 1){			// Check if the selected key is BACK
						cout << "\n";
						cout << "Number of " << itemLookupTable[j-1] << ": " << numItemsTable[j-1] << "\n" << endl;
					}else{
						cout << "\n";
					}
				}while(j != numItemsTable.size() + 1);		// Stay in the loop until BACK key is selected
				cout << "****************************" << endl;
				break;
				
			case 2:
				do{
					// Ask for input
					cout << "Select shelf ID to check (1-" << numShelves << ") 0=EXIT\n";
					cout << "Input: " ;
					cin >> inputShelf;
					cout << "\n";
					// Filter input for errors started================//
					shelf = inputShelf[0];
					intShelf = (int)shelf -48; 	// convert to integer value

					if(strlen(inputShelf) > 1 || intShelf > numShelves || !isdigit(shelf) ){
						cout << "Invalid input!!!" << "\n\n";
						break;
					}
					// Filter input for errors ends=======================//
					trigger = 0;				
					l = 0;
					while(l< chCount + 2){
						if(trigger > 0 && isdigit(rawData[l]) || trigger > 0 && rawData[l] == '\0'){
								endlist = l;
								break;
						}	
						if(rawData[l] == shelf){
							startlist = l +2;
							trigger++;
						}
						l++;
					}
					while(startlist < endlist){
						cout << rawData[startlist];
						startlist++;
					}
					if(intShelf != 0){
						cout << "\n\n";
					}
				}while(intShelf != 0);
				cout << "****************************" << endl;			
				break;

			case 3: 
				run = 0;
				cout << "Farewell" << endl;
				cout << "****************************" << endl;
				break;
			default:
				cout << "Invaid Input\n";
				cout << "****************************" << endl;
				break;
		}
	}
	return 0;
}

