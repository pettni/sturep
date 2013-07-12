#ifndef HELPF_H
#define HELPF_H

#include <string>
#include <algorithm>
#include <iostream>
#include <iterator>

using namespace std;

namespace spel {

  void strip(string & orig, const string &);

  void strip2(string & orig, const string &);

  string format(const string &);

  string lowercase(const string &);

  string uppercase(const string &);

  pair<string,string> split_at_first_space(const string &);

  int count_spaces(const string &);

  bool is_number(const string &);

  string join(const vector<int> &);

    string join(const vector<string> &);

    string join(const vector<pair<int,string>> &);

  vector<string> split(const string &, const string &);

    bool file_exists(string);
} // end of namespace

#endif










