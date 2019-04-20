
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
using namespace std;
void ssort(string &s) {
	sort(s.begin(), s.end());
}

int main(int argc, char *argv[]) {
	unordered_map<string, vector <string>> jumble;
	ifstream file;
	string word;
	string key;
	int numWords = 0, numClasses = 0;
	string maxKey;
	int largestSize = 0;
	

	if (argc != 2) {
		std::cout << "usage:  ./jumble <filename>\n";
		return 1;
	}
	file.open(argv[1], ios::in);

	if (!file.is_open()) {
		std::cout << "Error: could not open file '" <<
			argv[1] << "'\n";
		return 1;
	}
	cout << "reading input file...\n";
	while (file >> word) {
		
		key = word;
		ssort(key);

		if (jumble.count(key) == 0)
		{
			numClasses++;
			jumble[key].push_back(word);
		}
		else {
			jumble[key].push_back(word);
			if (jumble[key].size() > largestSize)
			{
				largestSize = jumble[key].size();
				maxKey = key;
			}
		}
		numWords++;
	}

	std::cout << "> ";

	while (cin >> word) {
		key = word;
		ssort(key);
		if (jumble.count(key) == 0) {
			cout << "no anagrams found...try again" << endl;
		}
		else {
			cout << "English Anagrams Found: " << endl;
			for (int i = 0; i<jumble[key].size(); i++)
				cout << "  '" << jumble[key][i] << "'" << endl;
		}
		std::cout << "> ";
	}

	cout << "REPORT: \n" << endl;
	cout << "  num_words		:  " << numWords << endl;
	cout << "  num_classes		:  " << numClasses << endl;
	cout << "  size-of-largest-class	:  " <<largestSize <<endl;
	cout << "  largest-class key	:  " <<maxKey<< endl;
	cout << "  members of largest class:  \n" << endl;
	for (int i = 0; i<jumble[maxKey].size(); i++)
		cout << "  '" << jumble[maxKey][i] << "'" << endl;


	return 0;
}


