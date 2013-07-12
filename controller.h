#ifndef CONTRL_H
#define CONTRL_H

#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <memory>
#include <chrono>
#include <random>

using namespace std;

namespace spel {

class Environment;
class Actor;
class Main_Actor;
class Object;

typedef unique_ptr<Object> (*object_factory_ptr) (const vector<string> &, map<string, unique_ptr<Object>> &);
typedef Actor * (*actor_factory_ptr) (const vector<string> &, map<string, unique_ptr<Object>> &);
typedef Environment * (*environment_factory_ptr) (const vector<string> &, map<string, unique_ptr<Object>> &);

typedef pair<string, object_factory_ptr> object_pair;
typedef pair<string, actor_factory_ptr> actor_pair;
typedef pair<string, environment_factory_ptr> environment_pair;

class Controller {
public:
    Controller();
    ~Controller();
    int run_game();
    
void create_default_world();  
void create_default_world2();
void save_game(const string &) const;
void load_game(const string &);

   
private:
    vector<Environment *> environments;
    vector<Actor *> actors;
    Main_Actor * player;
    Environment * taxi;
    Environment * respawn_location;

    int env_number( Environment * const &) const;
    int act_number( Actor * const &) const;

    void save_wrapper() const;
    void clear_game();
    
    void respawn();

    bool handle_other_commands(string, string, string);
    void remove_character(Actor *);    
    void arrive_in_queue(mt19937);
    void leave_queue();
    
    map<string, bool (Main_Actor::*)(string &)> actions;
    
    map<string, object_factory_ptr> object_factories;
    map<string, actor_factory_ptr> actor_factories;            
    map<string, environment_factory_ptr> environment_factories;

    vector<string> ib_names;
    vector<string> gf_names;

    vector<string>::iterator ib_ptr;
    vector<string>::iterator gf_ptr;

    mt19937 gen;
};

}

#endif
