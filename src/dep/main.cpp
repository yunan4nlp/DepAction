#include <vector>
#include <string>
#include <assert.h>
#include <fstream>
#include <iostream>

using namespace std;
#include "Instance.h"

void getInst(const vector<string> &vecInfo, Instance &inst);
void split_bychar(const string& str, vector<string>& vec, const char separator);
void readInstance(const char *path, const char *out_path);
void writeInst(const Instance &inst, ofstream &outf);


int main(int argc, char* argv[])
{
	readInstance(argv[1], argv[2]);
	return 0;
}

void readInstance(const char *in_path, const char *out_path) {
	ifstream inf(in_path);
	ofstream outf(out_path);
	if (inf.is_open() && outf.is_open()) {
		string line;
		vector<string> vecLines;
		while (getline(inf, line)) {
			if (line == "") {
				Instance inst;
				getInst(vecLines, inst);
				writeInst(inst, outf);
				vecLines.clear();
			}
			else
				vecLines.push_back(line);
		}
		if (vecLines.size() > 0) {
			Instance inst;
			getInst(vecLines, inst);
			writeInst(inst, outf);
		}
		inf.close();
		outf.close();
	}
	else
		cout << "open file error. " << endl;
}

void getInst(const vector<string> &vecInfo, Instance &inst) {
	int max_size = vecInfo.size();
	inst.resize(max_size);
	vector<string> fields;
	for (int idx = 0; idx < max_size; idx++) {
		split_bychar(vecInfo[idx], fields, '\t');
		assert(fields.size() == 10);
		inst.words[idx] = fields[1];
		inst.heads[idx] = atoi(fields[6].c_str());
		inst.relations[idx] = fields[7];
	}
}


void split_bychar(const string& str, vector<string>& vec, const char separator) {
	//assert(vec.empty());
	vec.clear();
	string::size_type pos1 = 0, pos2 = 0;
	string word;
	while ((pos2 = str.find_first_of(separator, pos1)) != string::npos) {
		word = str.substr(pos1, pos2 - pos1);
		pos1 = pos2 + 1;
		if (!word.empty())
			vec.push_back(word);
	}
	word = str.substr(pos1);
	if (!word.empty())
		vec.push_back(word);
}

void writeInst(const Instance &inst, ofstream &outf) {
	Instance a(inst);
	a.checkRoot();
	if (!a.checkProjective()) {
		cout << "non projective, skip" << endl;
		a.show();
	}
	else {
		int max_size = inst.size;
		for (int idx = 0; idx < max_size; idx++) {
			outf << inst.words[idx] << "_" << inst.heads[idx] << "_" << inst.relations[idx] << " ";
		}
		outf << endl;
	}
}
