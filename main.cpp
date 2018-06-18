/*
 * Author: Aryan Gupta
 * Hash Table - this program allows the user to create a list of students using a hash table
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <time.h>
#include <stdlib.h>

using namespace std;

//struct to store a student's data
struct student{
  char* firstName;
  char* lastName;
  int id;
  float gpa;
};

//struct used for the linked list
struct node{
  student* value = NULL;
  node* next = NULL;
};

//function declarations
int hash1(int id, int size);
void rehash(node* newHash[], node* oldHash[], int &size);
void print(node* table[], int size);
student* create(char* firstName, char* lastName, int id, float gpa);
bool add(node* hashTable[], int index, student* s);
void deleteStudent(node* hashTable[], int id, int size);
bool isValid(node* hashTable[], int size);

int main(){
  cout << "Student List - Hash Table" << endl;
  char input[10];
  char response[5];
  node** hashTable = new node*[100]; //hash table
  vector<int> ids; //stores all student ids
  for(int i = 0; i < 100; i++){
    hashTable[i] = NULL; //initialize everything to NULL
  }
  int size = 100;
  ifstream inFile;
  ifstream inFile2;
  char fileName[20];
  char fileName2[20];
  char fileInput[1000];
  char fileInput2[1000];
  char* split;
  vector<char*> firstNames;
  vector<char*> lastNames;
  bool randomStudent = false;
  //reads in input from text file
  cout << "Do you want to randomly generate students? (YES/NO): ";
  cin.getline(response, 5);
  if(strcmp(response, "YES") == 0){
    randomStudent = true;
    cout << "Enter the file name for first names: " ;
    cin.getline(fileName, 20, '\n');
    inFile.open(fileName);
    if(!inFile){
      cout << "file does not exist!" << endl;
      randomStudent = false;
    }
    inFile.getline(fileInput, 1000);
    split = strtok(fileInput, " ");
    while(split != NULL){
      firstNames.push_back(split);
      split = strtok(NULL, " ");
    }
    char* split2;
    cout << "Enter the file name for last names: ";
    cin.getline(fileName2, 20);
    inFile2.open(fileName2);
    if(!inFile2){
      cout << "file does not exist!" << endl;
      randomStudent = false;
    }
    inFile2.getline(fileInput2, 1000);
    split2 = strtok(fileInput2, " ");
    while(split2 != NULL){
      lastNames.push_back(split2);
      split2 = strtok(NULL, " ");
    
    }
  }
  srand(time(NULL));

  cout << '\n';
  cout << "Type ADD to manual add a student to the list" << endl;
  cout << "Type RANDOM to generate a random student" << endl;
  cout << "Type PRINT to print out all the students" << endl;
  cout << "Type DELETE to delete a student from the list" << endl;
  cout << "Type 'QUIT to exit the program" << endl;
  cout << '\n';
  
  while(true){
    cout << '\n';
    cout << "Enter a command: ";
    cin.getline(input, 10, '\n');
    if(strcmp(input, "ADD") == 0){
      //add a student into the list
      student* s;
      int id;
      float gpa;
      char* firstName = new char(30);
      char* lastName = new char(30);
      //gets the information for the new student
      cout << "Enter the student's first name: ";
      cin.getline(firstName, 30, '\n');
      cout << "Enter the student's last name: ";
      cin.getline(lastName, 30, '\n');
      cout << "Enter the student's id number: ";
      cin >> id;
      cin.get();
      cout << "Enter the student's gpa: ";
      cin >> gpa;
      cin.get();
      //checks if a duplicate id was entered
      bool duplicate = false;
      for(vector<int>::iterator it = ids.begin(); it != ids.end(); ++it){
	if((*it) == id){
	  duplicate = true;
	}
      }
      if(!duplicate){
	ids.push_back(id);
	s = create(firstName, lastName, id, gpa);
	int index = hash1(id, size);
	bool valid = add(hashTable, index, s);
	if(!valid){
	  //need to rehash the table
	  cout << "Rehashing." << endl;
	  node** newHash = new node*[size * 2];
	  rehash(newHash, hashTable, size);
	  delete [] hashTable;
	  hashTable = newHash;
	}
      }else{
	cout << "There is already another student with that ID number!" << endl;
      }
    }else if(strcmp(input, "RANDOM") == 0){
      if(randomStudent){
	int max = 0;
	for(int i = 0; i < ids.size(); i++){
	  if(ids.at(i) > max){
	    max = ids.at(i);
	  }
	}
	//randomly generate a new student
	int loop;
	cout << "How many students would you like to add? ";
	cin >> loop;
	cin.get();
	bool needRehash = false;
	for(int i = 0; i < loop; i++){
	  char* firstName = new char(30);
	  char* lastName = new char(30);
	  int id;
	  float gpa;
	  //randomly picks names from the name vector
	  int fName = rand() % firstNames.size() + 1;
	  int lName = rand() % lastNames.size() + 1;
	  firstName = firstNames.at(fName - 1);
	  lastName = lastNames.at(lName - 1);
	  //ID is determined by adding 1 to the current highest ID number
	  id = max + 1;
	  ids.push_back(id);
	  max+= 1;
	  //generates a random decimal from 0-4
	  gpa = ((double) rand() / (RAND_MAX)) * 4;
	  student* s = create(firstName, lastName, id, gpa);
	  int index = hash1(id, size);
	  bool valid = add(hashTable, index, s);
	  if(!valid){
	    needRehash = true;
	  }
	}
	if(needRehash){
	  //need to rehash the table
	  cout << "Rehashing..." << endl;
	  node** newHash = new node*[size * 2];
	  rehash(newHash, hashTable, size);
	  delete [] hashTable;
	  hashTable = newHash;
	  
	}
      }else{
	//if the files from the beginning of the program were not found or never entered
	cout << "Missing name files" << endl;
      }
    }else if(strcmp(input, "PRINT") == 0){
      //print out the list of students
      print(hashTable, size);
    }else if(strcmp(input, "DELETE") == 0){
      //deletes a student from the list
      int id;
      cout << "Enter the ID of the student to be deleted" << endl;
      cin >> id;
      cin.get();
      ids.push_back(id);
      deleteStudent(hashTable, id, size);
    }else if(strcmp(input, "QUIT") == 0){
      //exits the program
      break;
    }else{
      //invalid input
      cout << "Invalid command. Try again ; ";
    }
  } //while loop
} //main


//if one of our chains is longer than 3, we must rehash our table
void rehash(node* newHash[], node* oldHash[], int &size){
  //initializes everything in the new hash table to null
  for(int i = 0; i < (size * 2); i++){
    newHash[i] = NULL;
  }
  //doubles the size
  int newSize = 2 * size;
  for(int i = 0; i < size; i++){
    if(oldHash[i] != NULL){
      node* current = oldHash[i];
      while(current != NULL){
	if(current->value != NULL){
	  //puts all nodes from the old hash table into the new hash table
	  student* s = current->value;
	  //reperforms hash function to adjust position in the new hash table
	  int index = hash1(s->id, newSize);
	  add(newHash, index, s);
	  current = current->next;
	}
      }
    }
  }
  //adjust size
  size = newSize;
}

//checks if a hash table is valid
bool isValid(node* hashTable[], int size){
  for(int i = 0; i < size; i++){
    if(hashTable[i] != NULL){
      int count = 1;
      node* current = hashTable[i];
      //checks to see how long the chain is
      while(current != NULL){
	current = current->next;
	count++;
      }
      //if we have more than 3 nodes in the chain, we need to rehash
      if(count > 3){
	//rehash
	return false;
      }
    }
  }
  //no chains longer than 3
  return true;
}

//returns the hash value for a given id
int hash1(int id, int size){
  //gets the sum of all the digits in the id
  int sum = 0;
  while(id != 0){
    sum = sum + id % 10;
    id = id / 10;
  }
  //multiplies by size of has htable divided by 7
  int multiplier = trunc(size/7);
  return ((sum  * multiplier) % size);
}

//creates a new student
student* create(char* firstName, char* lastName, int id, float gpa){
  student* temp = new student();
  //creates a new student
  temp->firstName = firstName;
  temp->lastName = lastName;
  temp->id = id;
  temp->gpa = gpa;
  return temp;
}

//adds a student into the has table
bool add(node* hashTable[], int index, student* s){
  //if we don't need to chain nodes together
  if(hashTable[index] == NULL){
    node* temp = new node();
    temp->value = s;
    hashTable[index] = temp;
    return true;
  }
  //if not, add this node to the beginning of the linked list
  node* current = hashTable[index];
  node* temp = new node();
  temp->next = current;
  temp->value = s;
  hashTable[index] = temp;
  current = temp;
  int count = 1;
  //checks to see how long the chain is
  while(current != NULL){
    current = current->next;
    count++;
  }
  //if we have more than 3 nodes in the chain, we need to rehash
  if(count > 3){
    //rehash
    return false;
  }else{
    //we don't need to rehash
    return true;
  }
}

//prints out the list of students
void print(node* table[], int size){
  //sets the precision to 2 decimal points
  cout << fixed;
  cout << setprecision(2);
  node* current;
  for(int i = 0; i < size; i++){
    if(table[i] != NULL){
      //makes sure there is a student at the index
      current = table[i];
      while(current != NULL){
	//while there are still students in the current chain, keep printing them out
	if(current->value != NULL){
	  cout << "First Name: " << current->value->firstName << endl;
	  cout << "Last Name: " << current->value->lastName << endl;
	  cout << "ID: " << current->value->id << endl;
	  cout << "GPA: " << current->value->gpa << endl;
	  cout << endl;
	}
	current = current->next;
	
      }
    }
  }
}

//deletes a student from the hash table
void deleteStudent(node* hashTable[], int id, int size){
  //finds which position in the hash table the given id is at
  int hashID = hash1(id, size);
  node* current = hashTable[hashID];
  node* previous = NULL;
  //there are no nodes at the hashID
  if(current == NULL){
    cout << "Student not found! (nothing in hashID)" << endl;
    return;
  }
  //loops through the chain and searches for that id
  while(current != NULL){
    if(current->value->id == id){
      if(previous == NULL){
	//if there is only one node in the chain
	delete current;
	hashTable[hashID] = NULL;
      }else{
	//if there are multiple nodes in the chain
	previous->next = current->next;
	delete current;
      }
      return;
    }
    //keep looping though the chain until we find that id
    previous = current;
    current = current->next;
  }
  //the student was not found
  cout << "Student not found! (ID was not in hashID)" << endl;
  return;
}
