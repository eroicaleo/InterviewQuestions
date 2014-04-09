#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

const size_t codeLength = 6;
const size_t tupleLength = 3;
size_t readyLength = codeLength - 1;

struct LetterEntry {
    char myLetter;
    bool valid {false};
    bool repeat {false};
    int repeatCount {0};
    bool done {false};
    vector<int> preArray {vector<int>(26, 0)};
    vector<int> postArray {vector<int>(26, 0)};

    // member function
    void updateMyRelation(char own, const map<char, int> &preRelation, const map<char, int> &postRelation);
    bool satisfyARelation(char own, const map<char, int> &preRelation, const map<char, int> &postRelation) const;
    bool readyRegular() const;
    bool readyFromFront() const;
    bool readyFromBack() const;

    int RemoveOneLetter(char c, int frontOrBack);
};

int LetterEntry::RemoveOneLetter(char c, int frontOrBack) {
    vector<int>& vecRef = frontOrBack > 0 ? preArray : postArray;
    if (vecRef[c-'a'] == 0) {
	cerr << "Can not remove " << c << " from relations of " << myLetter << endl;
	exit(66);
    }
    return --vecRef[c-'a'];
}

bool LetterEntry::readyFromFront() const {
    if (!repeat)
	return false;
    int sum = 0;
    for (auto i : postArray) {
	sum += i;
    }
    if (sum < readyLength-1) {
	return false;
    }
    // post processing current letter
    // 1. Move one letter to ready array
    // 2. Remove the invalid successor, trick part
    cout << "char " << myLetter << " is ready from front." << endl;
    return true;
}

bool LetterEntry::readyFromBack() const {
    if (!repeat)
	return false;
    int sum = 0;
    for (auto i : preArray) {
	sum += i;
    }
    if (sum == readyLength-1) {
	cout << "char " << myLetter << " is ready from back." << endl;
	return true;
    }
    return false;
}

bool LetterEntry::readyRegular() const {
    if (repeat)
	return false;
    int sum = 0;
    for (auto i : preArray) {
	sum += i;
    }
    for (auto i : postArray) {
	sum += i;
    }
    if (sum == readyLength-1) {
	cout << "char " << myLetter << " is ready regularly." << endl;
	return true;
    }
    return false;
}

bool LetterEntry::satisfyARelation(char own, const map<char, int> &preRelation, const map<char, int> &postRelation) const {
    if ((own != myLetter) && valid) {
	cerr << "Wrong char in satisfyARelation!" << endl;
	exit(66);
    }
    for (auto entry : preRelation) {
	char c = entry.first;
	char cnt = entry.second;
	if (preArray[c-'a'] < cnt)
	    return false;
    }
    for (auto entry : postRelation) {
	char c = entry.first;
	char cnt = entry.second;
	if (postArray[c-'a'] < cnt)
	    return false;
    }
    cout << "char " << own << " satisfy the relation." << endl;
    return true;
}

void LetterEntry::updateMyRelation(char own, const map<char, int> &preRelation, const map<char, int> &postRelation) {
    if (!valid) {
	valid = true;
	myLetter = own;
    }
    for (auto iter = preRelation.begin(); iter != preRelation.end(); ++iter) {
	char c = iter->first;
	int cnt = iter->second;
	if (preArray[c-'a'] < cnt)
	    preArray[c-'a'] = cnt;
	if (myLetter == c) {
	    repeat = true;
	    repeatCount = 1+max(cnt, preArray[c-'a']);
	}
    }
    for (auto iter = postRelation.begin(); iter != postRelation.end(); ++iter) {
	char c = iter->first;
	int cnt = iter->second;
	if (postArray[c-'a'] < cnt)
	    postArray[c-'a'] = cnt;
	if (myLetter == c) {
	    repeat = true;
	    repeatCount = 1+max(cnt, preArray[c-'a']);
	}
    }
    readyRegular();
    readyFromFront();
    readyFromBack();
}

// operator functions
ostream& operator<<(ostream& os, const LetterEntry &letter) {
    if (!letter.valid)
	return os;
    os << "repeat : " << letter.repeat << endl;
    os << "repeatCount : " << letter.repeatCount << endl;
    os << "done : " << letter.done << endl;
    os << "The relation of char: " << letter.myLetter << endl;
    os << "Pre relation: " << endl;
    for (size_t i = 0; i < letter.preArray.size(); ++i) {
	os << char('a' + i) << " ";
    }
    os << endl;
    for (size_t i = 0; i < letter.preArray.size(); ++i) {
	os << letter.preArray[i] << " ";
    }
    os << endl;
    os << "Post relation: " << endl;
    for (size_t i = 0; i < letter.postArray.size(); ++i) {
	os << char('a' + i) << " ";
    }
    os << endl;
    for (size_t i = 0; i < letter.postArray.size(); ++i) {
	os << letter.postArray[i] << " ";
    }
    os << endl;
    return os;
}

// helper functions
void GenerateRelation(string triplet, map<char, int> &preRelation, map<char, int> &postRelation, size_t pos) {
    if (triplet.size() != tupleLength || pos >= tupleLength) {
	cerr << "Can not generate relation map because triplet or position wrong!" << endl;
	exit(66);
    }

    preRelation.clear();
    postRelation.clear();

    for (size_t i = 0; i < pos; ++i) {
	char c = triplet[i];
	if (!preRelation[c])
	    preRelation[c] = 1;
	else
	    ++preRelation[c];
    }
    for (size_t i = pos+1; i < triplet.size(); ++i) {
	char c = triplet[i];
	if (!postRelation[c])
	    postRelation[c] = 1;
	else
	    ++postRelation[c];
    }

    return;
}

vector<LetterEntry> letterTable(26);

int main() {
    ifstream infile("1");
    string triplet;
    map<char, int> preRelation;
    map<char, int> postRelation;
    while ((infile >> triplet).good()) {
	cout << triplet << endl;
	for (size_t i = 0; i < triplet.size(); ++i) {
	    char c = triplet[i];
	    GenerateRelation(triplet, preRelation, postRelation, i);
	    if (!letterTable[c-'a'].satisfyARelation(c, preRelation, postRelation)) {
		letterTable[c-'a'].updateMyRelation(c, preRelation, postRelation);
	    }
	    cout << letterTable[c-'a'] << endl;
	}
    }
    infile.close();
    return 0;
}
