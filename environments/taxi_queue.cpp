#include "taxi_queue.h"


namespace spel {

Taxi_Queue::Taxi_Queue(const string & n) : Outdoor(n) {
    Prep = "I";
}

void Taxi_Queue::save(ofstream & fs, int & obj_num, const int & env_num) const {
    string objects = Environment::save_objects(fs, obj_num);
    fs << "ENVIRONMENT" << env_num << ":" <<
        "TAXI_QUEUE" << ":" <<
        Name << ":" << 
        Prep << ":" << 
        Drop_location <<
        objects << 
        endl;
}


} //end of namespace
