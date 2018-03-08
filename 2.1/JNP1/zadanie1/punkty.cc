#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

using namespace std;


using identifier_t = string;
using students_t = set<identifier_t>;
using cooperations_t = multiset<identifier_t>;
using assigned_students_t = vector<identifier_t>;
using team_members_t = vector<identifier_t>;
using assigns_t = map<identifier_t, cooperations_t>;
using rx_iter_t = regex_iterator<string::iterator>;


students_t studentCollection;
assigns_t comembers;

const string matchIndexString = "(?:m-[0-9]{4}|[0-9]{6})";
const string matchIdString = "(?:[a-z]{2}" + matchIndexString + ")";
const string matchTeamString =
    "grupa[1-8]\\/zadanie[1-6]\\/((?:" + matchIdString + "\\+){2,3})";

const regex matchIndex(matchIndexString);
const regex matchId(matchIdString);
const regex matchTeam(matchTeamString);


void parseId(const int line, const string &s, char *filename);
void parseTeam(const int line, const string &s);
string getIndex(const identifier_t &a);
int compareIdsByIndexes(const identifier_t &a, const identifier_t &b);
long int countPenalty(const identifier_t &student);
assigned_students_t getAssignedStudents();


int main(int argc, char **argv) {
  char *filename;

  if (argc != 2) {
    cerr << "Usage: "<< argv[0] << " file\n";
    return 1;
  }

  filename = argv[1];

  ifstream file;
  file.open(filename);

  if (!file) {
    cerr << "Error: problem with file " << filename << "\n";
    return 1;
  }

  string id;
  for (int line = 1; getline(file, id); ++line) {
    if (!id.empty()) {
      parseId(line, id, filename);
    }
  }

  string team;
  for (int line = 1; getline(cin, team); ++line) {
    if (!team.empty()) {
      parseTeam(line, team);
    }
  }

  file.close();

  assigned_students_t students = getAssignedStudents();
  sort(students.begin(), students.end(), compareIdsByIndexes);

  for (const identifier_t &student : students) {
    long int penalty = countPenalty(student);
    if (penalty > 0) {
      cout << getIndex(student) << ";" << penalty << ";\n";
    }
  }

  return 0;
}

void parseId(const int line, const string &s, char *filename) {
  smatch idMatch;

  if (!(regex_match(s, idMatch, matchId)) || // only the whole string can match
      studentCollection.find(s) != studentCollection.end()) {
    cerr << "Error in " << filename << ", line " << line << ": " << s << "\n";
    return;
  }

  studentCollection.insert(s);
}

void parseTeam(const int line, const string &s) {
  smatch teamMatch;

  string tmpStr(s + '+'); // improves consistency of identifiers - we don't
                          // need to make exceptional check for last id
  if (!regex_match(tmpStr, teamMatch, matchTeam)) { // all or nothing matches
    cerr << "Error in cin, line " << line << ": " << s << "\n";
    return;
  }

  string teamStr(teamMatch[1]);
  team_members_t membersFound;

  rx_iter_t resultsEnd; // constructor defaults to end of results

  for (rx_iter_t resultsIterator(teamStr.begin(), teamStr.end(), matchId);
       resultsIterator != resultsEnd; ++resultsIterator) {

    identifier_t newMember = resultsIterator->str();
    if ((studentCollection.find(newMember) == studentCollection.end()) ||
        count(membersFound.begin(), membersFound.end(), newMember) > 0L) {
      cerr << "Error in cin, line " << line << ": " << newMember << "\n";
    } else {
      membersFound.push_back(newMember);
    }
  }

  for (identifier_t member : membersFound) {
    for (identifier_t comember : membersFound) {
      if (comember != member) {
        comembers[member].insert(comember);
      }
    }
  }
}

string getIndex(const identifier_t &a) {
  smatch index;
  regex_search(a, index, matchIndex);
  return index[0];
}

int compareIdsByIndexes(const identifier_t &a, const identifier_t &b) {
  return getIndex(a) < getIndex(b);
}

ptrdiff_t countPenalty(const identifier_t &student) {
  const cooperations_t &partners = comembers[student];

  ptrdiff_t sum = 0;
  for (identifier_t man : studentCollection) {
    if (man != student) {
      auto repeats = count(partners.begin(), partners.end(), man);
      sum += repeats * (repeats - 1) / 2;
    }
  }

  return sum;
}

assigned_students_t getAssignedStudents() {
  assigned_students_t out;
  for (auto keyval : comembers) {
    out.push_back(keyval.first);
  }
  return out;
}
