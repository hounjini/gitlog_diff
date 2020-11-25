#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct commit {
	int idx;
	string id;
	string title;
};

void openFile(ifstream& f, char* file) {
	cout << "Let's open " << file << endl;
	f.open(file, ios::in);
	if(f.fail()) {
		cout << "Cannot open file - " << file << endl;
		exit(0);
	}
}

void getMap(ifstream& f, unordered_map<string, struct commit>& l) {
	string line;
	int idx = 0;
	while(getline(f, line)) {
		struct commit _c;
		_c.idx = idx;
		_c.id = line.substr(0, 12);
		_c.title = line.substr(13);
		l.insert({_c.title, _c});
//		cout << idx << " : " << _c.title << endl;
		idx++;
	}
}

bool do_comp(struct commit& a, struct commit& b) {
	return a.idx > b.idx;
}

vector<struct commit> getResult_l1_minus_l2(unordered_map<string, struct commit>& l1, unordered_map<string, struct commit>& l2) {
	vector<struct commit> v;

	for(auto& a : l1) {
		if(l2.find(a.first) == l2.end()) {
			v.push_back(a.second);
		}
	}
	sort(v.begin(), v.end(), do_comp);
	return v;
}

int main(int argc, char** argv) {
	if(argc != 3) {
		cout << "git log --oneline ./ >>> 적당히 복사해서 log1.txt" << endl;
		cout << "git log --oneline ./ >>> 적당히 복사해서 log2.txt" << endl;
		cout << "./compare.out log1.txt log2.txt" << endl;
		return 0;
	}

	ifstream f1, f2;
	openFile(f1, argv[1]);
	openFile(f2, argv[2]);

	unordered_map<string, struct commit> l1;
	unordered_map<string, struct commit> l2;

	getMap(f1, l1);
	getMap(f2, l2);

	vector<struct commit> l1_l2;
	vector<struct commit> l2_l1;

	l1_l2 = getResult_l1_minus_l2(l1, l2);
	l2_l1 = getResult_l1_minus_l2(l2, l1);
	cout << "Commit only " << argv[1] << " have (" << l1_l2.size() << "개)" << endl;
	for(auto a : l1_l2) {
		cout << a.idx << " / " << a.id << " / " << a.title << endl;
	}

	cout << "==========================" << endl;

	cout << "Commit only " << argv[2] << " have (" << l2_l1.size() << " 개)" << endl;
	for(auto a : l2_l1) {
		cout << a.idx << " / " << a.id << " / " << a.title << endl;
	}
	return 0;
}
