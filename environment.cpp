#include "environment.h"

#include "actor.h"

namespace spel {

    Environment::Environment(const string & s) : Name(s), id(s) {
        id = format(Name);
        Prep = "På"; //Default
    }

    string Environment::visitors_description() const {
        string s;
        s = "Du är " + get_prep() + " " + Name + ". ";
        // Other actors
        if (visitors.size() == 2) {
            s += "Där finns ";
            for (auto it = visitors.begin(); it != visitors.end(); ++it) {
                if ((*it)->get_Name()!="JW") {
                    s += (*it)->get_type() + " " + (*it)->get_Name() + ". ";
                }
            }
        }
        else if (visitors.size() > 2) {
            s += "Där finns ";
            auto it = visitors.begin();
            auto end = visitors.end();                        
            --end;
        
            if ((*end)->get_Name() == "JW") {
                --end;
                for (; it != end; ++it) {
                    s += (*it)->get_type() + " " + (*it)->get_Name() + ", ";            
                }
                s.erase(s.length()-2,2);  
                s += " och " + (*it)->get_type() + " " + (*it)->get_Name() + ". ";
            }
            else {       
                for (; it != end; ++it) {
                    if ((*it)->get_Name() != "JW") {
                        s += (*it)->get_type() + " " + (*it)->get_Name() + ", ";
                    }
                }
                s.erase(s.length()-2,2);  
                s += " och " + (*it)->get_type() + " " + (*it)->get_Name() + ". ";           
            }            
        }
        return s;
    }

    string Environment::objects_description() const {
        string s = "";                
    
        // Objects
        if (objects.size() == 1) {
            auto it = objects.begin();
            s+= it->second + " " + it->first->get_prep() + " " + it->first->get_arttype() + ". ";
        }
        else if (objects.size() > 1) {
            auto it = objects.begin();
            auto end = objects.end();
            auto p1 = it + 1;
            auto p2 = it + 2;
        
            s += it->second + " " + it->first->get_prep() + " ";
                    
            for (; p2 != end; ++it,++p1,++p2) {
                s += it->first->get_arttype();
                if (it->second == p1->second && it->second == p2->second) {
                    s += ", ";
                }
                else if(it->second == p1->second) {
                    s += " och ";
                }
                else {
                    s += ". " + p1->second + " " + p1->first->get_prep() + " ";
                }
            }
            if (it->second == p1->second) {
                s += it->first->get_arttype() + " och " + p1->first->get_arttype() + ". ";    
            }
            else {
                s += it->first->get_arttype() + ". " + p1->second + " " + 
                    p1->first->get_prep() + " " + p1->first->get_arttype() + ". ";    
            }
        }
        return s;
    }

    string Environment::exits_description() const {   
        // Relative locations
        string s = "";
        if (neighbors.size() > 1) {
            s += "Du kan gå till ";
            auto it = neighbors.begin();
            auto end = neighbors.end();
            --end;
            for (; it != end; ++it)
                s = s + it->first + ", ";
            s.erase(s.length()-2,2);  
            s += " eller " + it->first;
            s += ". ";
        } else if (neighbors.size() == 1) {
            s += "Du kan gå " + neighbors.begin()->first;
            s += ". ";
        }
        return s;
    } 

    string Environment::description() const {
         stringstream ss;
         ss << visitors_description() << objects_description() << 
               exits_description();
        return ss.str();
    }

    string Environment::short_description() const {
        stringstream s;
        s << Prep << " " << Name << " finns " << visitors.size() << " personer och " <<
            objects.size() << " föremål.";
        return s.str();
    }

    void Environment::add_visitor(Actor * a) {
        if (find(visitors.begin(), visitors.end(), a) == visitors.end())
            visitors.push_back(a);   
    }

    Actor * Environment::find_visitor(const string & s) const {
        Actor * ret = nullptr;
        for (auto it = visitors.begin(); it != visitors.end(); ++it) {
            if (s == (**it).get_id()) {
                ret = *it;
                break;
            }
        }        
        return ret;
    }

    void Environment::remove_visitor(Actor * a) {
        auto it = find(visitors.begin(), visitors.end(), a);
        if (it == visitors.end()) {
            throw out_of_range("Actor not found in visitors list");
        }
        visitors.erase(it);
    }

    Actor * Environment::random_visitor() const {
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen (seed1);  
        uniform_int_distribution<int> dist(0,visitors.size()-1);
    
        auto it = visitors.begin();
        advance(it, dist(gen));
        return *it;
    }

    Actor * Environment::random_other_visitor(Actor * a) const {
        if(find(visitors.begin(),visitors.end(),a) != visitors.end()) {
            Actor * ret = nullptr;
            if(visitors.size() == 1)
                return ret;
            else {
                ret = random_visitor();
                while (ret == a)
                    ret = random_visitor();
                return ret;
            }        
        }
        else
            return random_visitor();    
    }

    vector<Actor *> Environment::all_visitors_of_type(const type_info & type) const {
        vector<Actor *> candidates;
        for_each (visitors.begin(), visitors.end(), [&type,&candidates] (Actor * actor) {
                if (typeid(*actor) == type)
                    candidates.push_back(actor);
            });
        return candidates;
    }

    void Environment::add_object(unique_ptr<Object> & o) {
        objects.push_back(pair<unique_ptr<Object>, string>(move(o), Drop_location));
        o = nullptr;
    }

    void Environment::add_object(unique_ptr<Object> & o, const string & loc) {
        objects.push_back(pair<unique_ptr<Object>, string>(move(o), loc));
        o = nullptr;
    }

    int Environment::number_of_objects() const {
        return objects.size();
    }

    int Environment::find_object(const string & s) const {
        int ret = -1;
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            if (it->first->get_id() == s || it->first->get_type() == s) {
                ret = it - objects.begin();
                break;
            }
        }
        return ret;
    }

    int Environment::find_type(const type_info & type) const {
        int ret = -1;
        for (auto it = objects.begin(); it != objects.end(); ++it) {
            if (type == typeid(*(it->first))) {
                ret = it - objects.begin();     
                break;       
            }
        }
        return ret;
    }

    const unique_ptr<Object> & Environment::look_at_object(int obj_index) const {
        if (obj_index < 0 || obj_index >= objects.size())
            throw out_of_range("Object index out of bounds");
        return objects[obj_index].first;
    }

    const string & Environment::look_at_object_place(int obj_index) const {
        if (obj_index < 0 || obj_index >= objects.size())
            throw out_of_range("Object index out of bounds");
        return objects[obj_index].second;
    }

    unique_ptr<Object> Environment::release_object(const string & s) {
        int index = -1;
        auto it = objects.begin();
        for (; it != objects.end(); ++it) {
            if (it->first->get_id() == s || it->first->get_type() == s) {
                index = it-objects.begin();
                break;
            }
        }
        if (index != -1) {
            return move(release_object(index));
        } else {
            return nullptr;
        }
    }

    unique_ptr<Object> Environment::release_object(int obj_index) {
        if (obj_index < 0 || obj_index >= objects.size())
            throw out_of_range("Object index out of bounds");
            
        unique_ptr<Object> ret = move(objects[obj_index].first);
        objects.erase(objects.begin()+obj_index);
        return move(ret);
    }

    Environment * Environment::find_exit(const string & s) const {
        Environment * ret = nullptr;
        for (auto it = neighbors.begin(); it!= neighbors.end(); ++it) {
            if (s == it->second->get_id() || s == format(it->first) ) {
                ret = it->second;
                break;
            }
        }
        return ret;
    }

    Environment * Environment::random_exit() const {
        unsigned seed1 = std::chrono::system_clock::now().time_since_epoch().count();
        std::mt19937 gen (seed1);  
        uniform_int_distribution<int> dist(0,neighbors.size()-1);
    
        auto it = neighbors.begin();
        advance(it, dist(gen));
        return it->second;
    }

    string Environment::save_objects(ofstream & fs, int & obj_num) const {
        vector<pair<int, string>> objs;
        for_each (objects.begin(), objects.end(), 
                  [& fs, &obj_num, & objs] (const pair<unique_ptr<Object>, string> & grej) {
                      grej.first->save(fs, obj_num); 
                      pair<int,string> ny_grej(obj_num-1,grej.second);
                      objs.push_back(ny_grej);
                  } );
        string ret = ":" + join(objs);
        return ret;
    }

    void Environment::save(ofstream & fs, int & obj_num, const int & env_num) const {
        string objects = Environment::save_objects(fs, obj_num);
        fs << "ENVIRONMENT" << env_num << ":" <<
            "ENVIRONMENT" << ":" <<
            Name << ":" << 
            Prep << ":" << 
            Drop_location << 
            objects << 
            endl;
    }

    void connect(Environment * p1, string dir1, Environment * p2, string dir2) {    
        p1->neighbors.insert(pair<string, Environment *>(dir1, p2));
        p2->neighbors.insert(pair<string, Environment *>(dir2, p1)); 
    }

    void connect_oneway(Environment * p1, string dir1,  Environment * p2) {
        p1->neighbors.insert(pair<string, Environment *>(dir1, p2));
    }

    void connect(Environment * p, Actor * a) {
        p->add_visitor(a);
        a->location = p; 
    }

} // End of idspace

