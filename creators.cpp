#include "old_creators.cpp"
#include "actors/salesman.h"
#include "action_lambdas.cpp"

namespace spel{

// Objects

void set_object_info(const vector<string> & vec, map<string, unique_ptr<Object>> & objects, unique_ptr<Object> & ret) {
    ret->set_type(vec[1], vec[2], vec[3]);
    ret->set_action_type(vec[4]);
    ret->set_Prep(vec[5]);
    ret->set_info(atof(vec[6].c_str()),atof(vec[7].c_str()),atof(vec[8].c_str()));
    ret->set_action(atof(vec[9].c_str()), atof(vec[10].c_str()));
}

unique_ptr<Object>  create_object(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    unique_ptr<Object> ret(new Object(vec[0]));        
    set_object_info(vec,objects,ret);
    return move(ret);
}

unique_ptr<Object>  create_usable(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    unique_ptr<Object> ret = nullptr;
                
    if (vec[11].size()>0) {
        ret = unique_ptr<Object>(new Usable(vec[0], objects[vec[11]]));
        objects.erase(objects.find(vec[11]));
    } else {
        ret = unique_ptr<Object>(new Usable(vec[0]));
    }    
    set_object_info(vec,objects,ret);    
    return move(ret);
}

unique_ptr<Object> create_container(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    unique_ptr<Container> ret(new Container(vec[0],atof(vec[11].c_str()),atof(vec[12].c_str())));               

    if (vec[13].size()>0) {
        vector<string> spl = split(vec[13],",");
        for_each (spl.begin(), spl.end(), [& ret, & objects] (const string & id) {
                ret->add_object(objects[id]);
                objects.erase(objects.find(id));
            });
    }

    unique_ptr<Object> cast_ret(static_cast<Object*>(ret.release()));
    ret.reset();
    set_object_info(vec,objects,cast_ret);        
    return move(cast_ret);
}

unique_ptr<Object>  create_weapon(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    unique_ptr<Object> ret(new Weapon(vec[0]));        
    set_object_info(vec,objects,ret);
    return move(ret);
}

unique_ptr<Object>  create_money(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    unique_ptr<Object> ret(new Money(atof(vec[8].c_str())));
    set_object_info(vec,objects,ret);
    return move(ret);
}

// Actors

void set_actor_info(const vector<string> & vec, map<string, unique_ptr<Object>> & objects, Actor* ret) {
    //Set info
    ret->set_type(vec[0]);
    ret->set_Prep(vec[2]);
    ret->set_hp(atof(vec[3].c_str()));
    ret->money = atof(vec[4].c_str());
    ret->angry = atoi(vec[5].c_str());
    ret->lifetime = atoi(vec[6].c_str());

    // Give objects

    // Hand object
    if (vec[7].size()>0) {
        unique_ptr<Object> new_hand(move(objects[vec[7]]));
        objects.erase(objects.find(vec[7]));
        ret->hand = move(new_hand);
    }

    // Pocket object
    unique_ptr<Container> new_pocket(static_cast<Container *> (objects[vec[8]].release()));
    objects[vec[8]].reset();
    objects.erase(objects.find(vec[8]));
    ret->pocket = move(*new_pocket);
    new_pocket.reset();        

    // Container object
    if (vec[9].size()>0) {
        unique_ptr<Container> new_carriable(static_cast<Container *>(objects[vec[9]].release()));
        objects[vec[9]].reset();
        objects.erase(objects.find(vec[9]));
        ret->carriable = move(new_carriable);
    }
    
    if (vec[10].size() > 0) {
        ret->answers = split(vec[10],",");    
    }    
}

Actor * create_visitor(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Visitor * ret = new Visitor(vec[1]);
    set_actor_info(vec,objects,ret);
    ret->set_promille(atof(vec[11].c_str()));
    ret->set_get_lambda(create_visitor_get_l(ret));
    return ret;
}

Actor * create_worker(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = new Worker(vec[1]);
    set_actor_info(vec,objects,ret);
    ret->set_action_lambda(create_worker_action_l(ret));
    ret->set_get_lambda(create_worker_get_l(ret));    
    return ret;   
}

Actor * create_salesman(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Salesman * ret = new Salesman(vec[1],objects[vec[11]],vec[12]);
    ret->set_action_lambda(create_worker_action_l(ret));    
    set_actor_info(vec,objects,ret);
    return ret;   
}

Actor * create_main_actor(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Main_Actor * ret = new Main_Actor(vec[1]);
    set_actor_info(vec,objects,ret);
    ret->set_promille(atof(vec[11].c_str()));
    return ret; 
}

Actor * create_kling(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = static_cast<Worker*>(create_worker(vec,objects));
    return ret;
}

Actor * create_klang(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = static_cast<Worker*>(create_worker(vec,objects));
    return ret;
}

Actor * create_borje(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = static_cast<Worker*>(create_worker(vec,objects));
    return ret;
}

Actor * create_fadde(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = static_cast<Worker*>(create_worker(vec,objects));
    function<void(void)> death_l = [ret]() { 
        if(typeid(*(ret->get_location())) == typeid(Queue)) { 
            Queue* q = static_cast<Queue*>(ret->get_location());
            q->open();            
        }
    };
    ret->set_death_lambda(death_l);
    
    // Get lambda choice:
    // 0: refuse
    // 1: accept and put in pocket
    // 2: accept and use
    
    function<int(unique_ptr<Object>&)> get_l = [ret](unique_ptr<Object>& obj) {
        if(obj->get_type() == "knark") { 
            cout << ret->get_Name() << ": Tackar. Välkommen in." << endl;
            if(typeid(*(ret->get_location())) == typeid(Queue)) { 
                Queue* q = static_cast<Queue*>(ret->get_location());
                q->open();            
            }
            return 1;
        }
        return 0;
    };
    ret->set_get_lambda(get_l);
    
    return ret; 
}

Actor * create_fedde(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Worker * ret = static_cast<Worker*>(create_worker(vec,objects));
    // Death
    function<void(void)> death_l = [ret]() { 
        if(typeid(*(ret->get_location())) == typeid(Queue)) { 
            Queue* q = static_cast<Queue*>(ret->get_location());
            q->open();            
        }
    };
    ret->set_death_lambda(death_l);
    
    // Get
    function<int(unique_ptr<Object>&)> get_l = [ret](unique_ptr<Object>& obj) {
        if(typeid(*obj) == typeid(Money)) { 
            if (obj->get_price() >= 500) {
                cout << ret->get_Name() << ": Schysst. Du är en riktig polare. Välkommen in." << endl;
                if(typeid(*(ret->get_location())) == typeid(Queue)) { 
                    Queue* q = static_cast<Queue*>(ret->get_location());
                    q->open();            
                }
                return 1;          
            }
            else {
                return 0;        
            }
        }        
        return 0;
    };
    ret->set_get_lambda(get_l);        
    return ret; 
}

Actor * create_ib(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Visitor * ret = static_cast<Visitor*>(create_visitor(vec,objects)); 
    ret->set_action_lambda(create_ib_action_l(ret));
    ret->add_answer("What's up?");
    ret->add_answer("Har ett bra jobb va.");
    ret->add_answer("Stek.");         
    return ret;
}

Actor * create_gf(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Visitor * ret = static_cast<Visitor*>(create_visitor(vec,objects));
    ret->set_action_lambda(create_gf_action_l(ret));
    ret->add_answer("Tjena JW! Vart ska du ikväll?");
    ret->add_answer("Har du något ladd?");        
    return ret;
}

Actor * create_bingo(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
    Visitor * ret = static_cast<Visitor*>(create_visitor(vec,objects));
    ret->set_action_lambda(create_bingo_action_l(ret));
    
    // Get
    function<int(unique_ptr<Object>&)> get_l = [ret](unique_ptr<Object>& obj) {
        if (obj->get_type() == "hamburgare") { 
            cout << ret->get_Name() << ": Tack som fan! Du anar inte hur hungrig" 
                                       <<  "jag var. Ta det här VIP-kortet!" << endl;
            return 3;          
        } else      
            return 0;
    };
    ret->set_get_lambda(get_l);        
    
    return ret;
}

// Environments

void set_environment_info(const vector<string> & vec, map<string, unique_ptr<Object>> & objects, Environment* ret) {
ret->set_Prep(vec[1]);
ret->set_Drop_location(vec[2]);
if (vec[3].size()>0) {
    vector<string> spl = split(vec[3],",");
    for_each (spl.begin(), spl.end(), [& ret, & objects] (const string & info) {
            vector<string> spl2 = split(info, ";");
            string id = spl2[0];
            string place = spl2[1];
            ret->add_object(objects[id], place);
            objects.erase(objects.find(id));
        });
}
}

Environment * create_outdoor(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
Outdoor * ret = new Outdoor(vec[0]);
set_environment_info(vec,objects,ret);
return ret;
}

Environment * create_indoor(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
Indoor * ret = new Indoor(vec[0]);
set_environment_info(vec,objects,ret);
return ret;
}

Environment * create_queue(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
Queue * ret = new Queue(vec[0]);
set_environment_info(vec,objects,ret);
ret->closed = atoi(vec[4].c_str());
return ret;
}

Environment * create_taxi_queue(const vector<string> & vec, map<string, unique_ptr<Object>> & objects) {
Taxi_Queue * ret = new Taxi_Queue(vec[0]);
set_environment_info(vec,objects,ret);
return ret;
}                
} // End of namespace
