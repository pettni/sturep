#include "visitor.h"

#include "../objects/usable.h"
#include "../environment.h"
#include "../environments/indoor.h"
#include "../environments/taxi_queue.h"
#include "main_actor.h"
#include "salesman.h"
#include "../objects/weapon.h"

namespace spel {

    Visitor::Visitor(string n) : Actor(n) {}

    Visitor::Visitor(string n, unique_ptr<Object> & o1, unique_ptr<Object> & o2) : Actor(n, o1, o2) {}

    string Visitor::description() const {
        stringstream s;
        s << main_description();
        string state;
        if (promille < 0.3)
            state = "nykter";
        else if (promille < 0.6)
            state = "salongsberusad";
        else if (promille < 1)
            state = "packad";
        else 
            state = "redlös";

        s << " " << Name << " är " << state << ".";
        return s.str();
    }

    void Visitor::go(Environment * new_location) {
        int min_lifetime = 5;
        if (Name != "JW" && lifetime <= min_lifetime) {
            if (typeid(*new_location) == typeid(Taxi_Queue)) {
                return;
            }
        }
        cout << Name << " gick från " << location->get_Name() << 
            " till " << new_location->get_Name() << endl;     
        location->remove_visitor(this);
        new_location->add_visitor(this);        
        location = new_location;
    }

    void Visitor::action() {
        ++lifetime;
        if (hp == 0)
            return;
        affect_promille(-0.01);

        if (typeid(*get_location()) == typeid(Taxi_Queue)) {
            go(get_location()->random_exit());
            return;
        }
        
        int choice = 0;
        if (action_lambda) {
            choice =  action_lambda();  
        } 
        //cout << Name << " valde " << choice << endl;
        
        switch (choice) {
        case 0 : { break; }
        case 1 : { 
            go(get_location()->random_exit());    
            break; 
        }
        case 2 : { 
            if(!(get_location()->get_Name().size() >= 3 && get_location()->get_Name().substr(0,3) == "bar")) {
                Environment* bar = get_location()->find_exit("baren");
                if(bar)
                    Visitor::go(bar);
                else
                    Visitor::go(get_location()->random_exit());
            }
            break;
        }
        case 3 : { 
            if(!(get_location()->get_Name().size() >= 9 && get_location()->get_Name().substr(0,9) == "McDonalds")) {
                Environment* donken = get_location()->find_exit("McDonalds");
                if(donken)
                    Visitor::go(donken);
                else
                    Visitor::go(get_location()->random_exit());
            }
            break; 
        }
        case 4 : { 
            if ( (!hand) || (typeid(*(hand))!=typeid(Weapon)) ) {
                look_for(typeid(Weapon));
            }   
            if (enemy && enemy->get_location() == location) {
                attack(enemy);
            }
            break; 
        }
        case 5 : {             
            look_for(typeid(Usable));
            if(hand && typeid(*hand) == typeid(Usable)) {
                use();
            }
            else {
                buy(typeid(Usable));
                look_for(typeid(Usable));
                use();
            }
            break; 
        }
        case 6 : {
            Actor * target = get_location()->random_other_visitor(this);
            if (target)
                attack(target);
            break; 
        }
        case 7 : {
            if(!(get_location()->get_Name().size() >= 8 && get_location()->get_Name().substr(0,8) == "dansgolv")) {
                Environment* df = get_location()->find_exit("dansgolvet");
                if(df)
                    Visitor::go(df);
                else
                    Visitor::go(get_location()->random_exit());
            } else {
                cout << Name << " svängde loss på " << get_location()->get_Name() << endl;
            }
            break;        
        }
        case 8: {
            // Go to random indoor location
            Environment * dest = get_location()->random_exit();
            if (typeid(*dest) == typeid(Indoor))
                Visitor::go(dest);
            
        }
        case 9: {                        
            unique_ptr<Object> mon = create_money(500);
            if(mon) {
                cout << Name << " mutade " << enemy->get_Name() << " med " << mon->get_price() << " spänn." << endl;
                give(mon,enemy);
                enemy = nullptr;     
            }
        }
        }
    }

    void Visitor::buy(const type_info & type) {
        vector<Actor *> sellers = get_location()->all_visitors_of_type(typeid(Salesman));
        
        for_each (sellers.begin(), sellers.end(), [this,&sellers,&type] (Actor * cand) {
                Salesman * seller = static_cast<Salesman *>(cand);
                if ( typeid(*(seller->look_at_merch())) == type )
                    buy(seller);
            });
    }

    void Visitor::buy(Salesman * seller) {
        if (seller && money >= seller->get_price()) {
            unique_ptr<Object> mon(create_money(seller->get_price()));
            Actor::give(mon, seller);
            cout << Name << " köpte en " << seller->look_at_merch()->get_type() << " av " 
                 << seller->get_type() << " " << seller->get_Name() << " " << location->get_prep() << " " 
                 << location->get_Name() << "." << endl;
        }
    }
    
    void Visitor::save(ofstream & fs, int & obj_num, const int &act_num) const {
        string objects = Actor::save_objects(fs, obj_num);

        fs << "ACTOR" << act_num <<
            ":VISITOR:"  <<
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
