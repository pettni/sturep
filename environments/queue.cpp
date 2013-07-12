#include "queue.h"

namespace spel {

Queue::Queue(const string & s1) : Outdoor(s1), entrance(nullptr) {
    Prep = "I";
    closed = true;
}

Queue::Queue(const string & s1, Environment * entrance) : Outdoor(s1) {
    this->entrance = entrance;
    Prep = "I";
    closed = true;
}

Environment * Queue::find_exit(const string & s) const {
    Environment * ret = nullptr;
    for (auto it = neighbors.begin(); it!= neighbors.end(); ++it) {
        if (s == it->second->get_id() || s == format(it->first) ) {
            ret = it->second;
            break;
        }
    }
    if (entrance && (ret == entrance) && closed) {
        cout << "Vakten: Sorry, det är fullt ikväll. " << endl;
        return nullptr;
    }
    return ret;
}

void Queue::save(ofstream & fs, int & obj_num, const int & env_num) const {
    string objects = Environment::save_objects(fs, obj_num);
    fs << "ENVIRONMENT" << env_num << ":" <<
        "QUEUE" << ":" <<
        Name << ":" << 
        Prep << ":" << 
        Drop_location <<
        objects << ":" <<
        closed <<
        endl;
}


} // end of namespace
