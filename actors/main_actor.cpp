#include "main_actor.h"

#include "../environment.h"
#include "../objects/weapon.h"
#include "../objects/usable.h"
#include "../objects/money.h"
#include "../objects/container.h"
#include "../help_functions.h"

#include "salesman.h"

namespace spel {

//////////////////////// HELP FUNCTIONS ///////////////////

// Move objects from bag1 to bag2, drop in env if they don't fit
void change_bag(unique_ptr<Container> & bag1, unique_ptr<Container> & bag2, Environment * env) {
    while (bag1->number_of_objects()>0) {
        unique_ptr<Object> object = bag1->release_object(0);
        if (bag2->fits(object)) {
            cout << "Du flyttade " << object->get_definitive() <<
                " från " << bag1->get_definitive() << " till " <<
                bag2->get_definitive() << endl;
            bag2->add_object(object);
        } else {
            cout << "Det fanns ingen plats för " << object->get_definitive() <<
                " i " << bag2->get_definitive() << 
                " så den släpptes " << env->get_drop_location() << endl;
            env->add_object(object);
        }
    }  
}

bool is_in_hand(const unique_ptr<Object> & hand, const string & cmd) {
    if (hand) {
        if (cmd == hand->get_id() || cmd == hand->get_type() ) {
            return true;
        }
    }
    return false;
}

bool check_hand(const unique_ptr<Object> & hand, const string & cmd) {
    if (!hand) {
        cout << "Du har inget i handen" << endl;
        return true;
    } else if (!cmd.empty() && cmd != hand->get_id() && cmd != hand->get_type() ) {
        cout << "Du har ju " << hand->get_arttype() << " i handen." << endl;
        return true;
    }
    return false;
}

void look_for_object(Main_Actor * a, const string & cmd) {
    int pos;
    string empty;
    if (is_in_hand(a->hand,cmd)) {
        ; // do nothing
    } else if ((pos = a->pocket.find_object(cmd)) > -1) {
        if (a->hand) {
            a->drop(empty);
        }
        string object = a->pocket.look_at_object( pos )->get_id();
        a->pick_up(object);
    } else if (a->carriable && ((pos = a->carriable->find_object(cmd)) > -1) ) {
        if (a->hand) {
            a->drop(empty);
        }
        string object = a->carriable->look_at_object( pos )->get_id();
        a->pick_up(object);
    }

}

pair<string,string> divide_command(const unique_ptr<Object> & hand, const string & cmd) {
    string cmd1, cmd2;
    pair<string,string> split = split_at_first_space(cmd);
    if (is_in_hand(hand,split.first)) {
        cmd1 = split.first;
        cmd2 = split.second;
    } else {
        cmd2 = cmd;
    }
    return pair<string,string>(cmd1,cmd2);
}

/////////////////////////////////////////////////////////////

Main_Actor::Main_Actor(string n) : Visitor(n) {
    Type = "Stekaren";
}

bool Main_Actor::go(string & cmd) {
    strip(cmd,"till");
    cmd = format(cmd);

    if (cmd.empty()) {
        cout << "Vart vill du gå?" << endl;
        return true;
    }

    Environment * new_location = location->find_exit(cmd);
    if (!new_location) {
        cout << "Det går inte att gå " << cmd << ". " << endl;
        return true;
    } else {
        cout << string(80,'-') << endl;
        Visitor::go(new_location);
        return false;
    }
}

bool Main_Actor::pick_up(string & cmd) {
    string cmd_orig = cmd;
    
    strip(cmd,"upp");
    strip(cmd,"fram");
    strip(cmd,"ut");
    cmd = format(cmd);
    
    unique_ptr<Object> obj = nullptr;
    obj = location->release_object(cmd);     // Look on ground
    if (!obj) { // Look in pocket
        obj = pocket.release_object(cmd);
    }
    if (!obj && carriable) { // Look in bag
        obj =  carriable->release_object(cmd);
    }
    if (!obj && carriable && (cmd == "väska" || cmd == carriable->get_type())) {
        obj = move(carriable);   
        carriable = nullptr;
    }
    if (obj) {
        if (typeid(*obj) == typeid(Money)) {
            Actor::take_money(obj);
            cout << "Du tog upp en bunt pengar. Du har " << money << " kr." << endl;
            return true;
        }
        if (hand) {
            string old_obj = hand->get_arttype();
            Actor::put_in_pocket(hand);
            if(!hand) 
                cout << "Du la " << old_obj << " " << 
                "i fickan." << endl;
            else {
                cout << "Du släppte " << old_obj << " " << 
                     location->get_drop_location() << "." << endl;
                Actor::drop();
            }
        }
        Actor::pick_up(obj); 
        cout << "Du tog upp " << hand->get_arttype() << " i handen. " << endl;
    } else {
        cout << "Det gick inte att ta " << cmd_orig << "." << endl;
    }
    return true;
}

bool Main_Actor::put(string & cmd) {
    strip(cmd,"ner");
    strip(cmd,"i");
    
    string cmd1, cmd2;

    pair<string,string> split = divide_command(hand, cmd);
    cmd1 = split.first;
    cmd2 = format(split.second);
    
    if (check_hand(hand,cmd1))
        return true;
    // ok to put
    
    string object_name = hand->get_definitive();
    if (cmd2 == "ficka") {
        bool success = pocket.add_object(hand);
        if (success) {
            cout << "Du la " << object_name << " i fickan" << endl;
        } else {
            cout << "Inte tillräckligt med plats för " << 
             object_name << " i fickan" << endl;
        }
        return true;
    } 
    
    if (cmd2 == "väska" && !carriable) {
        cout << "Du har ingen väska." << endl;
        return true;
    }
    
    if (cmd2 == "väska" || (carriable && cmd2 == carriable->get_type() ) ) {       
        bool success = carriable->add_object(hand);
        if (success) {
            cout << "Du la " << object_name << " i " << 
            carriable->get_definitive()  << endl;
        } else {
            cout << "Inte tillräckligt med plats för " << 
             object_name << " i " << 
             carriable->get_definitive() << endl;
        }
        return true;
    }
    
    cout << "Det går inte att lägga " << object_name << " i " << cmd2 << endl;
    return true;
}

bool Main_Actor::look(string & cmd) const {
    strip(cmd, "på");
    strip(cmd, "i");
    strip(cmd, "mot");
    cmd = format(cmd);

    ////////// Look at locations /////////////
    if (cmd.empty() || cmd == "runt" || cmd == "omkring") {
        cout << location->description() << endl;
        return true;    
    }
    Environment * loc = location->find_exit(cmd);
    if (loc)
        cout << "Du tittar " << cmd << " och ser " << loc->get_Name() << ". " << 
             loc->short_description() << endl;
    
    ////////// Look at actors ////////////////
    Actor * act = location->find_visitor(cmd);
    if (act)
        cout << act->description() << endl;
    
    /////////// Look at objects //////////////
    if (cmd == "hand") {
        if (hand) {
            cout << hand->description() << endl;
        } else {
            cout << "Du har inget i handen" << endl;
        }
        return true;    
    }
    if (hand) {
        if (cmd == hand->get_type() || cmd == hand->get_id()) {
            cout << hand->description() << endl;
            return true;
        }
    }
    if (cmd == "ficka") {
        cout << pocket.description() << endl;
        return true;    
    }
    if (cmd == "väska") {
        if (carriable) {
            cout << carriable->description() << endl;
        } else {
            cout << "Du har ingen väska" << endl;
        }
        return true;    
    }
    
    // Look in pocket
    int obj_index = pocket.find_object(cmd);
    if (obj_index > -1) {
        cout << "I fickan finns " << pocket.look_at_object(obj_index)->get_arttype() << ". " 
             << pocket.look_at_object(obj_index)->description() << endl;
    }
    
    // Look in bag (if any)
    if (carriable) {
        obj_index = carriable->find_object(cmd);
            if (obj_index > -1) {
                cout << "I " << carriable->get_definitive() << " finns " << 
                        carriable->look_at_object(obj_index)->get_arttype() <<". "
                     << carriable->look_at_object(obj_index)->description() << endl;
            }
        if ( cmd == carriable->get_type()) {
            cout << carriable->description() << endl;
        }
    }
    
    // Look in current location
    obj_index = location->find_object(cmd);
    if (obj_index > -1) {
        cout <<  location->look_at_object_place(obj_index) << " ligger " <<
                 location->look_at_object(obj_index)->get_arttype() << ". " <<
                 location->look_at_object(obj_index)->description() << endl;
    }
    
    return true;
}

bool Main_Actor::drop(string & cmd) {
    if (check_hand(hand,cmd))
        return true;
    // ok to drop
    cout << "Du släppte " << hand->get_arttype() << " " << 
         location->get_drop_location() << endl;
    Actor::drop();
    return true;
}

bool Main_Actor::use(string & cmd) {
    strip(cmd, "på");
    strip(cmd, "mot");

    pair<string,string> split = split_at_first_space(cmd);
    look_for_object(this, split.first);

    split = divide_command(hand, cmd);
    string cmd1, cmd2;
    cmd1 = split.first;
    cmd2 = format(split.second);
    
    if (check_hand(hand,cmd1))
        return true;
    // ok to use

    if (typeid(*hand) == typeid(Container)) {
        unique_ptr<Container> new_bag(static_cast<Container *>(hand.release()));
        hand.reset();
        if (carriable) {
            change_bag(carriable, new_bag, location);
            unique_ptr<Object> old_bag(static_cast<Object *>(carriable.release()));
            carriable.reset();
            cout << "Du släppte " << old_bag->get_definitive() << 
                " " << location->get_drop_location() << endl;
            location->add_object(old_bag);
        }
        cout << "Du tog på dig " << new_bag->get_arttype() << endl;
        Actor::put_on(new_bag);
        return true;
    }

    if (typeid(*hand) == typeid(Weapon)) {
        if (cmd2.empty()) {
            cout << Name << " " << hand->get_action_type() << " med " 
                 << hand->get_article() << " "
                 << hand->get_Name()  << endl;
            return true;
        } else {
            Actor * target = location->find_visitor(cmd2);
            if (!target) {
                cout << cmd2 << " finns ej i rummet" << endl;
                return true;
            } else {
                cout << string(80,'-') << endl;
                Actor::attack(target);
                return false;
            }
        }
    }    
    
    if (typeid(*hand) == typeid(Usable)) {
        cout << string(80,'-') << endl;
        Visitor::use();
        cout << description() << endl;
        return false;
    }
}

bool Main_Actor::manual_attack(string & cmd) {
        strip(cmd, "på");
        Actor * target = location->find_visitor(format(cmd));
        if (target) {
            Actor::manual_attack(target);
            return false;
        } else {
            cout << "Personen finns inte i rummet" << endl;
        }
    }

bool Main_Actor::give(string & cmd) {

    strip(cmd, "till");
    strip(cmd, "kr");
    strip(cmd, "pengar");

    pair<string,string> split = split_at_first_space(cmd);
    string cmd1 = split.first;
    string cmd2 = format(split.second);

    if (is_number(cmd1)) {
        Actor * target = location->find_visitor(cmd2);
        if (target) {
            int amount = stoi(cmd1);
            if (amount > money) {
                cout << "Du har bara " << money << " kronor." << endl;
            }
            unique_ptr<Object> mon(create_money(amount));
            Actor::give(mon, target);
        } 
        else {
            cout << "Personen finns inte i rummet." << endl;
        }
    }
    else if (hand) {        
        if (cmd1 == hand->get_type() || cmd1 == hand->get_id()) {
            Actor * target = location->find_visitor(cmd2);
            if (target) {
                Actor::give(hand,target);
            }
            else {
                if (cmd2.empty())
                    cout << "Vem vill du ge " << hand->get_definitive() << " till?" << endl;
                else
                    cout << cmd2 << " är inte här." << endl;
            }
        }
        else {
            cout << "Du har ingen/inget " << cmd << " i handen." << endl;
        }                
    }
    else {
        cout << "Det gick inte att ge " << cmd << endl;
        return true;
    }    
}

bool Main_Actor::buy(string & cmd) {
        strip(cmd, "en");
        strip(cmd, "ett");
        strip(cmd, "av");
        
        bool success = false;

        pair<string,string> split = split_at_first_space(cmd);
        string cmd1 = split.first;
        vector<Actor *> sellers = get_location()->all_visitors_of_type(typeid(Salesman));
        for_each (sellers.begin(), sellers.end(), [this, &success, &cmd1] (Actor * cand) {
                Salesman * seller = static_cast<Salesman *>(cand);
                if (!success && (cmd1 == seller->look_at_merch()->get_type() || cmd1 == seller->look_at_merch()->get_id() )) {                                  
                    if ( money >= seller->look_at_merch()->get_price()) {
                        cout << seller->get_Name() << ": Här har du. Stick härifrån nu!" << endl;
                        Visitor::buy(seller);
                    }
                    else {
                        cout << seller->get_Name() << ": Du har för lite cash! Stick!" << endl;
                    }                                         
                    success = true;
                }             
            });
        if (!success)
            cout << "Det finns ingen som säljer det." << endl;
        return true;
    }

bool Main_Actor::talk_to(string & cmd) {
    string orig = cmd;
    strip(cmd, "med");
    Actor * target = location->find_visitor(format(cmd));    
    if(target)
        target->answer();
    else
        cout << orig << " är inte i rummet. " << endl;
    return true;
}
    
bool Main_Actor::check_win() const {
    return (hand && hand->get_id() == "vip-kort");
}

void Main_Actor::transport_to(Environment * env) {
    location->remove_visitor(this);
    env->add_visitor(this);        
    location = env;
}
    
void Main_Actor::death() {
    cout << "main actor death" << endl;
    money = money/2;
    Actor::drop();
}

void Main_Actor::save(ofstream & fs, int & obj_num, const int &act_num) const {
    string objects = Actor::save_objects(fs, obj_num);
    fs << "ACTOR" << act_num <<
        ":MAIN_ACTOR:"  <<
        Type << ":" <<
        Name << ":" <<
        Prep << ":" <<
        hp << ":" <<
        money << ":" <<
        angry << ":" <<
        lifetime << 
        objects << ":" <<
        join(answers) << ":" <<        
        promille << endl;;
}    
} // End of namespace
