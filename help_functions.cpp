#include "help_functions.h"

#include <fstream>

using namespace std;

void spel::strip(string & orig, const string & remove) {
  size_t rl = remove.size();
        
  string rem1 = remove + " ";
  string rem2 = " " + remove;

  while (orig.size()>=remove.size()+1 && orig.substr(0,rl+1) == rem1) {
    orig.erase(0,rl+1);
  } 
        
  while (orig.size()>=remove.size()+1 && orig.substr(orig.size()-1-rl,string::npos) == rem2) {
    orig.erase(orig.size()-1-rl,string::npos);
  } 
  string rem3 = " " + remove + " ";
  size_t pos = orig.find(rem3);
  while (pos != string::npos) {
    orig.replace(pos,rl+2," ");
    pos = orig.find(rem3);
  }
}

string spel::format(const string & s) {
  string ret = s;
  ret.erase(remove_if(ret.begin(), ret.end(), ::isspace), ret.end());
  transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
  return ret;
}

string spel::lowercase(const string & s) {
  string ret = s;
  transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
  return ret;
}

string spel::uppercase(const string & s) {
  string ret = s;
  transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
  return ret;
}
// if no space, second return is empty.
pair<string,string> spel::split_at_first_space(const string & s) {
  size_t first_space = s.find_first_of(" ");
  string cmd_root = s.substr(0, first_space);
    
  string cmd_rest;
  if (first_space != string::npos)
    cmd_rest = s.substr(first_space+1);    
  return pair<string,string>(cmd_root,cmd_rest);
}

int spel::count_spaces(const string & s) {
  return count_if(s.begin(), s.end(),
		  [](unsigned char c){ return isspace(c); });

}

bool spel::is_number(const std::string& s) {
  return !s.empty() && find_if(s.begin(), 
			       s.end(), [](char c) { return !std::isdigit(c); }) == s.end();
}

string spel::join(const vector<int> & v) {
  stringstream ss;
  for_each(v.begin(), v.end(), [&ss] (int i) {
      ss << "OBJECT" << i << ",";
    } );
  return ss.str().substr(0,ss.str().size()-1);
}

string spel::join(const vector<string> & v) {
  stringstream ss;
  for_each(v.begin(), v.end(), [&ss] (string i) {
          ss << i << ",";
      });
 return  ss.str().substr(0,ss.str().size()-1);
}

string spel::join(const vector<pair<int,string>> & v) {
  stringstream ss;
  for_each(v.begin(), v.end(), [&ss] (pair<int,string> grej) {
      ss << "OBJECT" << grej.first << ";" << grej.second << ",";
    } );
  return ss.str().substr(0,ss.str().size()-1);
}

vector<string> spel::split(const string & str, const string & del) {
  vector<string> ret;
  string copy = str;
  size_t pos = copy.find(del);
  while((pos=copy.find(del)) != string::npos) {
    ret.push_back(copy.substr(0,pos));
    copy.erase(0,pos+del.size());

  }
  ret.push_back(copy);
  return ret;
}

bool spel::file_exists(string filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        return false;
    }
    return true;
}
