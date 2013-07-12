#include "controller.h"

#include "objects/container.h"
#include "objects/weapon.h"
#include "objects/usable.h"
#include "objects/money.h"

#include "actors/visitor.h"
#include "actors/worker.h"
#include "actors/main_actor.h"

#include "environments/outdoor.h"
#include "environments/indoor.h"
#include "environments/taxi_queue.h"
#include "environments/queue.h"

#include "help_functions.h"
#include "creators.cpp"

namespace spel {
    
    Controller::~Controller() {
        clear_game();
    }
        
    void Controller::clear_game() {
        for (auto it = actors.begin(); it != actors.end(); ++it) {
            delete *it;
        }
        actors.clear();
        for (auto it = environments.begin(); it!= environments.end(); ++it) {
            delete *it;
        }
        environments.clear();
        delete player;
        player = nullptr;
        respawn_location = nullptr;
        taxi = nullptr;
    }    
    
    
    string read() {
        while (true) {
            cout << ">>";
            string command;
            getline(cin, command);
            if (command.size() > 0)
                return lowercase(command);
        }
    }

    void Controller::remove_character(Actor * a) {
        Environment * loc = a->get_location();
        loc->remove_visitor(a);
        auto p = actors.begin();
        for(auto it = actors.begin();it != actors.end();++it) {
            if (*it == a) {
                p = it;
            }
            if ((**it).enemy && (**it).enemy == a) {
                (**it).enemy == nullptr;
            }
        }
        cout << a->get_Name() << " gick hem!" << endl;
        delete a;
        actors.erase(p);
    }

    Controller::Controller() {

        ib_names = {"Sara Ferrari","Wang Wang","Stephen Schwartz","Alan Dente","James Cameron","Bryan Ryan","Alan Fresco","Billy Rubin","Frank Enstein"};
        gf_names = {"Anastasia","Daria","Elena","Eugenia","Irina","Katja","Ksenia","Masha","Milla","Nataliya","Tatiana"};

        ib_ptr = ib_names.begin();
        gf_ptr = gf_names.begin();    

        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "ta", &Main_Actor::pick_up ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "gå", &Main_Actor::go ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "titta", &Main_Actor::look_wrapper) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "använd", &Main_Actor::use ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "lägg", &Main_Actor::put ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "släpp", &Main_Actor::drop ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "ge", &Main_Actor::give ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "köp", &Main_Actor::buy ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "slå", &Main_Actor::manual_attack ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "prata", &Main_Actor::talk_to ) );
        
                
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "skjut", &Main_Actor::use ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "hugg", &Main_Actor::use ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "attackera", &Main_Actor::use ) );
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "snacka", &Main_Actor::talk_to ) );                       
        actions.insert(pair<string, bool (Main_Actor::*)(string &)>
                       ( "tala", &Main_Actor::talk_to ) );                       

        object_factories.insert(object_pair("OBJECT", &create_object));
        object_factories.insert(object_pair("CONTAINER", &create_container));
        object_factories.insert(object_pair("MONEY", &create_money));
        object_factories.insert(object_pair("WEAPON", &create_weapon));
        object_factories.insert(object_pair("USABLE", &create_usable));

        actor_factories.insert(actor_pair("VISITOR", &create_visitor));
        actor_factories.insert(actor_pair("WORKER", &create_worker));
        actor_factories.insert(actor_pair("SALESMAN", &create_salesman));
        actor_factories.insert(actor_pair("MAIN_ACTOR", &create_main_actor));
                             
        actor_factories.insert(actor_pair("WORKER_FADDE", &create_fadde));
        actor_factories.insert(actor_pair("WORKER_FEDDE", &create_fedde));        
        
        actor_factories.insert(actor_pair("WORKER_KLING", &create_kling));
        actor_factories.insert(actor_pair("WORKER_KLANG", &create_klang));        
        actor_factories.insert(actor_pair("WORKER_BÖRJE", &create_borje));
        
        actor_factories.insert(actor_pair("VISITOR_FOTOGRAFEN", &create_bingo));
        actor_factories.insert(actor_pair("VISITOR_INVESTMENT BANKERN", &create_ib));        
        actor_factories.insert(actor_pair("VISITOR_TJEJPOLAREN", &create_gf));
                
        environment_factories.insert(environment_pair("OUTDOOR",&create_outdoor));
        environment_factories.insert(environment_pair("INDOOR",&create_indoor));        
        environment_factories.insert(environment_pair("QUEUE",&create_queue));
        environment_factories.insert(environment_pair("TAXI_QUEUE",&create_taxi_queue));                       
    }

    void Controller::create_default_world2() {
        
        Outdoor * stureplan = new Outdoor("Stureplan");
        environments.push_back(stureplan);
        Main_Actor * jw = new Main_Actor("JW");                           
        connect(stureplan, jw);
        respawn_location = stureplan;
        player = jw;
        
        unique_ptr<Object> ol = create_beer("Heineken");
        stureplan->add_object(ol);            
    }

    void Controller::create_default_world() {  
        Outdoor * kungsgatan = new Outdoor("Kungsgatan");
        Outdoor * stureplan = new Outdoor("Stureplan");
    
        Indoor * donken = new Indoor("McDonalds");
        Indoor * donken_toilet = new Indoor("toaletten på McDonalds");
    
        Indoor * polisvan = new Indoor("polisvanen vid Svampen");
        polisvan->set_Prep("I");
    
        Indoor * golden = new Indoor("dansgolvet på Golden Hits");
        Queue * golden_ko = new Queue("kön till Golden Hits",golden);
        Indoor * golden_bar = new Indoor("baren på Golden Hits");
        golden_bar->set_Prep("I");
        golden_bar->set_Drop_location("Under en barstol");
        Indoor * golden_toilet = new Indoor("toaletten på Golden Hits");
        
        Indoor * spybar = new Indoor("dansgolvet på Spy Bar");
        Queue * spybar_ko = new Queue("kön till Spy Bar",spybar);
        Indoor * spybar_bar = new Indoor("baren på Spy Bar");
        spybar_bar->set_Prep("I");
        spybar_bar->set_Drop_location("Under en barstol");
        Indoor * spybar_toilet = new Indoor("toaletten på Spy Bar");
        
    
        Taxi_Queue * taxiko = new Taxi_Queue("taxikön vid Stureplan");
        taxi = taxiko;
    
        environments.push_back(kungsgatan);
        environments.push_back(stureplan);
        environments.push_back(donken);
        environments.push_back(donken_toilet);
        environments.push_back(polisvan);
        environments.push_back(golden_ko);
        environments.push_back(golden);
        environments.push_back(golden_bar);
        environments.push_back(golden_toilet);
        environments.push_back(spybar_ko);
        environments.push_back(spybar);
        environments.push_back(spybar_bar);
        environments.push_back(spybar_toilet);
        environments.push_back(taxiko);

        connect(kungsgatan, "Stureplan", stureplan, "Kungsgatan");
        connect(kungsgatan, "McDonalds", donken, "ut");
    
        connect(donken, "toaletten", donken_toilet, "ut");
    
        connect(stureplan, "polisen", polisvan, "ut");
    
        connect(golden_ko, "Kungsgatan", kungsgatan, "Golden Hits");
        connect_oneway(golden_ko, "in", golden);
        connect_oneway(golden, "ut", kungsgatan);
        connect(golden, "baren", golden_bar, "dansgolvet");
        connect(golden, "toaletten", golden_toilet, "dansgolvet");
        connect(golden_bar, "toaletten", golden_toilet, "baren");

        connect(spybar_ko, "Stureplan", stureplan, "Spy Bar");
        connect_oneway(spybar_ko, "in", spybar);
        connect_oneway(spybar, "ut", stureplan);
        connect(spybar, "baren", spybar_bar, "dansgolvet");
        connect(spybar, "toaletten", spybar_toilet, "dansgolvet");
        connect(spybar_bar, "toaletten", spybar_toilet, "baren");
    
        connect(stureplan,"taxikön",taxiko,"Stureplan");
        
        Visitor * ib1 = create_ib("Eli Cash");
        ib1->add_answer("What's up?");
        ib1->add_answer("Har ett bra jobb va.");
        ib1->add_answer("Stek.");                
        connect(spybar_bar,ib1);

        Visitor * ib2 = create_ib("LLoyd");
        ib2->add_answer("What's up?");
        ib2->add_answer("Har ett bra jobb va.");
        ib2->add_answer("Stek.");        
        connect(golden_toilet,ib2);
    
        Worker * polis1 = create_cop("Kling");   
        polis1->add_answer("Hur var det här då?");
        polis1->add_answer("Är det lugnt ikväll eller?");
        
        connect(polisvan, polis1);
        Worker * polis2 = create_cop("Klang");    
        polis2->add_answer("Hur var det här då?");
        polis2->add_answer("Är det lugnt ikväll eller?");
        
        connect(polisvan, polis2);
        Worker * polis3 = create_cop("Börje");    
        polis3->add_answer("Hur var det här då?");
        polis3->add_answer("Är det lugnt ikväll eller?");
                
        connect(kungsgatan, polis3);
    
        Worker * spybar_doorman = create_doorman("Fadde");
        spybar_doorman->add_answer("Skulle vilja ha nåt som livar upp vardagen.");
        connect(spybar_ko,spybar_doorman);
        Worker * golden_doorman = create_doorman("Fedde");
        golden_doorman->add_answer("Ser rätt fullt ut ikväll. Men jag skulle gärna spela på hästar imorn om du förstår...");
        connect(golden_ko,golden_doorman);
    
        //Bartenders
        
        unique_ptr<Object> ol1 = create_beer("Heineken");
        unique_ptr<Object> ol2 = create_beer("stor stark");                 
            
        Salesman * spybar_bartender = new Salesman("Chris",ol1,"På bardisken");
        Salesman * golden_bartender = new Salesman("Amber",ol2,"På den smutsiga bardisken");
        
         
        spybar_bartender->set_type("Bartendern");
        golden_bartender->set_type("Bartendern");
                
        connect(spybar_bar,spybar_bartender);
        connect(golden_bar,golden_bartender);
        
        //Kassörska
        
        unique_ptr<Object> burgare(new Usable("cheeseburgare"));
        burgare->set_type("en", "hamburgare", "Hamburgaren");
        burgare->set_info(0.2, 0.3, 40);
        burgare->set_action(40, -0.2);
        burgare->set_action_type("käkade");            
        
        Salesman * kassorska = new Salesman("Lina",burgare,"på en bricka");    
        kassorska->set_type("Kassörskan");
        connect(donken, kassorska);
        
        //Drug dealer
        
        unique_ptr<Object> knark(new Usable("fultjack"));
        knark->set_type("två tabletter", "knark", "Knarket");
        knark->set_action_type("svalde");
        knark->set_info(0.05, 0.1, 500); 
        knark->set_action(-80, 4);
        knark->set_Prep("ligger");

        Salesman * drugdealer = new Salesman("Ricardo",knark,"på handfatet");
        drugdealer->set_type("Knarklangaren");
        drugdealer->add_answer("Jag säljer den bästa skiten i stan.");
        drugdealer->add_answer("Skaru ha nåt eller?");        
        connect(golden_toilet,drugdealer);  
        

        //Girlfriends

        Visitor * tp1 = create_girlfriend("Natasha");
        tp1->set_promille(0.5);
        tp1->add_answer("Tjena JW! Vart ska du ikväll?");
        tp1->add_answer("Har du något ladd?");         
        connect(spybar_toilet, tp1);

        Visitor * tp2 = create_girlfriend("Anita");
        tp2->set_promille(1.0);
        tp2->add_answer("Jag har hört att Fadde på Spy Bar är svag för tabletter.");
        tp2->add_answer("Har du något ladd?");         

        connect(golden_toilet, tp2);
    
        Visitor * tp3 = create_girlfriend("Ivana");
        tp3->add_answer("Tjena JW! Vart ska du ikväll?");
        tp3->add_answer("Har du träffat Bingo JW?");           
        tp3->set_promille(0.4);
        connect(stureplan, tp3);
    
        //Bingo
        unique_ptr<Object> kort(new Object("VIP-kort"));
        unique_ptr<Object> bingo_h = nullptr;
        kort->set_type("ett","all-accesskort till Spy Bars VIP-lounge","VIP-kortet");
        
        Visitor * Bingo = new Visitor("Bingo Rimér", bingo_h, kort);
        Bingo->set_type("Fotografen");
        Bingo->add_answer("Jag är så jävla hungrig asså.");
        Bingo->add_answer("Jag är så hungrig att jag inte kan tänka!");        
        connect(spybar, Bingo);

        actors = { ib1, ib2, tp1, tp2, tp3, polis1, polis2, polis3, 
                   spybar_doorman, golden_doorman,
                   spybar_bartender, golden_bartender,
                   kassorska, Bingo, drugdealer }; 

        std::unique_ptr<Object> batong(new Weapon("kraftig teleskopbatong"));
        batong->set_type("en","batong","Batongen");
        batong->set_action_type("slog");
        batong->set_action(30,0);
        batong->set_info(0.8, 2, 800);
        polisvan->add_object(batong, "På bordet");
    
        unique_ptr<Object> sten(new Weapon("tung gatsten"));
        sten->set_type("en","gatsten","Gatstenen");
        sten->set_action_type("attackerade");
        sten->set_action(8,0);
        sten->set_info(2, 2, 0);
        kungsgatan->add_object(sten);
              
//         unique_ptr<Object> knark1(new Usable("peruanskt kokain"));
//         knark1->set_type("en påse", "knark", "Knarket");
//         knark1->set_action_type("snortade");
//         knark1->set_info(0.05, 0.1, 500); 
//         knark1->set_action(100, 1);
//         knark1->set_Prep("ligger");
//         stureplan->add_object(knark1);                
    
//         unique_ptr<Object> knark2(new Usable("fultjack"));
//         knark2->set_type("två tabletter", "knark", "Knarket");
//         knark2->set_action_type("svalde");
//         knark2->set_info(0.05, 0.1, 500); 
//         knark2->set_action(-20, 1);
//         knark2->set_Prep("ligger");
//         golden_toilet -> add_object(knark2);
    
        // unique_ptr<Object> gun(new Weapon("44 Magnum"));
        // gun->set_type("en","pistol","Pistolen");
        // gun->set_action_type("sköt");
        // gun->set_info(1, 0.5, 1500); 
        // gun->set_action(60, 0);
        // gun->set_Prep("ligger");
        // stureplan->add_object(gun);
    
        unique_ptr<Object> bag(new Container("en rymlig sportbag", 20, 20));
        bag->set_type("en", "bag", "Sportbagen");
        bag->set_info(2, 20, 400); 
        polisvan->add_object(bag);
    
        unique_ptr<Object> mon1(new Money(1000));
        polisvan->add_object(mon1);
        
        unique_ptr<Object> mon2(new Money(350));
        donken_toilet->add_object(mon2);
 
        Main_Actor * jw = new Main_Actor("JW");                           
        connect(stureplan, jw);
    
        respawn_location = donken_toilet;
        player = jw;
    }

    int Controller::env_number( Environment * const & env) const {
        for (auto it = environments.begin(); it != environments.end(); ++it)
            if (*it == env)
                return (it - environments.begin());
        return -1;
    }

    int Controller::act_number( Actor * const & a) const {
        for (auto it = actors.begin(); it != actors.end(); ++it)
            if (*it == a)
                return (it - actors.begin());
        return -1;
    }
    
    void Controller::save_game(const string & filename) const {
        ofstream fs (filename);
        int obj_num = 0;
        int act_num = 0;
        int env_num = 0;
        for (auto it = actors.begin() ;it != actors.end(); ++it) {
            (**it).save(fs, obj_num, act_num);
            ++act_num;
        }
        player->save(fs, obj_num, -1);
    
        for_each (environments.begin(), environments.end(), 
                  [&fs, &obj_num, &env_num] (Environment * env) {
                      env->save(fs, obj_num, env_num);
                      ++env_num;
                  });
        
        fs << endl << endl;
    
        for (int i = 0; i != environments.size(); ++i) {
            for_each (environments[i]->neighbors.begin(), environments[i]->neighbors.end(),
                      [this, &fs, &i] (const pair<string, Environment *> & grej) {
                          fs << "CONNECTION:ENVIRONMENT" << i << ":"
                             << "ENVIRONMENT" << env_number(grej.second) << ":"
                             << grej.first << endl;
                      });
            if (typeid(*environments[i]) == typeid(Queue)) {
                Queue * queue = static_cast<Queue *>(environments[i]);

                fs << "QUEUECONNECTION:ENVIRONMENT" << i << ":"
                   << "ENVIRONMENT" << env_number(queue->entrance) << endl;
            }
        }
    
        for (int i = 0; i != environments.size(); ++i) {
            for_each (environments[i]->visitors.begin(), environments[i]->visitors.end(),
                      [this, &fs, &i] ( Actor * const & a) {
                          fs << "CONNECTION:ENVIRONMENT" << i << ":"
                             << "ACTOR" << act_number(a) << endl;
                      });
        }
    
        fs << endl;
    
        for (int i = 0 ; i != actors.size(); ++i) {
            if (actors[i]->enemy)
                fs << "ENEMY:" << "ACTOR" << i << ":ACTOR" << act_number(actors[i]->enemy)
                   << endl; 
        }

        fs << "TAXI:ENVIRONMENT" << env_number(this->taxi) << endl;
        fs << "RESPAWN:ENVIRONMENT" << env_number(this->respawn_location) << endl;
        fs.close();
    }
    
    void Controller::load_game(const string & filename) {
        clear_game();                        
        map<string,Actor *> new_actors;
        map<string, unique_ptr<Object>> new_objects;
        map<string, Environment *> new_environments;

        ifstream fs (filename);
        string line;
        while (getline(fs, line)) {

            if (line.size() == 0)
                continue;

            vector<string> spl = split(line,":");
            string id = spl[0];
            string arg1 = spl[1];

            spl.erase(spl.begin());
            spl.erase(spl.begin());
            if (id.size() >=6 && id.substr(0,6) == "OBJECT" ) {
                unique_ptr<Object> new_object(object_factories[arg1](spl,new_objects));
                new_objects.insert(pair<string, unique_ptr<Object>>(id, move(new_object)));
                continue;
            }

            if (id.size() >=5 && id.substr(0,5) == "ACTOR" ) {
                string identifier_1,identifier_2,identifier_3;
                identifier_1 = uppercase(arg1 + "_" + spl[1]);
                identifier_2 = uppercase(arg1 + "_" + spl[0]);
                identifier_3 = uppercase(arg1);          
//                 cout <<  identifier_1 << identifier_2 << identifier_3 << endl;                       
                Actor * new_actor = nullptr;
                
                if(actor_factories.find(identifier_1) != actor_factories.end())
                    new_actor = actor_factories[identifier_1](spl,new_objects);
                else if (actor_factories.find(identifier_2) != actor_factories.end())
                    new_actor = actor_factories[identifier_2](spl,new_objects);                    
                else                
                    new_actor = actor_factories[identifier_3](spl,new_objects);
                
                new_actors.insert(pair<string,Actor*>(id,new_actor));                  
                continue;        
            }          
            
            if (id.size() >= 11 && id.substr(0,11) == "ENVIRONMENT" ) {
                Environment * new_environment = environment_factories[arg1](spl,new_objects);
                new_environments.insert(pair<string,Environment*>(id,new_environment));
                continue;        
            }

            if (id.size() >= 10 && id.substr(0,10) == "CONNECTION" ) {
                string arg2 = spl[0];
                if (arg2.size() >= 5 && arg2.substr(0,5) == "ACTOR") {
                    connect(new_environments[arg1], new_actors[arg2]);
                }
                if (arg2.size() >= 10 && arg2.substr(0,11) == "ENVIRONMENT") {
                    connect_oneway(new_environments[arg1],spl[1], new_environments[arg2]);
                }
            }

            // QUEUE connections
            if (id.size() >= 15 && id.substr(0,15) == "QUEUECONNECTION" ) {
                string arg2 = spl[0];
                Queue * queue = static_cast<Queue *> (new_environments[arg1]);
                queue->entrance = new_environments[arg2];         
            }

            if (id.size() >= 5 && id.substr(0,5) == "ENEMY") {
                new_actors[arg1]->enemy = new_actors[spl[0]];
            }

            
            // Taxi variable
            if (id.size() >= 4 && id.substr(0,4) == "TAXI") {
                this->taxi = new_environments[arg1];
            }
            // Respawn location variable
            if (id.size() >= 4 && id.substr(0,7) == "RESPAWN") {
                this->respawn_location = new_environments[arg1];
            }

        }
        fs.close();
        // Put environments in a vector
        for_each (new_environments.begin(), new_environments.end(), [this] (pair<const string, Environment *> & pair) {
                this->environments.push_back(pair.second);
            });

        // Take care of main player
        this->player = static_cast<Main_Actor *>(new_actors["ACTOR-1"]);
        new_actors.erase(new_actors.find("ACTOR-1"));

        // Put other players in a vector
        for_each (new_actors.begin(), new_actors.end(), [this] (pair<const string, Actor *> & pair) {
                this->actors.push_back(pair.second);
            });

    }

    void print_start_message() {
        cout << string(80,'-') << endl;
        cout << string(80,'-') << endl;
        cout << "JW är en androgyn stekare i Stockholms nattliv. " <<
            "För tillfället är vännerna lätträknade och JW kommer knappt in på " <<
            "ålderdomshemmet Golden Hits; inte så stekigt. " << 
            "Det finns inget JW hellre vill än att bli stammis på " <<
            "Spy Bars dubbel-VIP. För att komma dit är JW beredd " <<
            "att gå långt, mycket långt... " << endl;
        cout << string(80,'-') << endl;
        cout << string(80,'-') << endl;        
        cout << "Kommandon du kan använda: ta, gå, titta, prata, använd, lägg, släpp, ge, köp, slå. "
             << "Om du har ett vapen i handen kan du även attackera, skjuta och hugga. " << endl;
        cout << "Objekt kan användas på dig själv, andra eller både och. "
             << "Du har en ficka du kan lägga saker i, och om du hittar en väska kan du använda den för att få mer plats." 
             << endl << endl;
        cout << "Exempel: köp öl, prata med xx, titta runt, titta i ficka, titta på A, ta väska, använd väska, lägg A i väska. " << endl << endl;       
        cout << "Om du har något i handen kan du använda det eller försöka ge det till någon annan. " << endl << endl;
        cout << "Exempel: använd A på B, ge A till B. " << endl << endl;
        cout << "Hint: Om du har problem kan du testa att ge pengar till någon. " << endl << endl;  
        cout << "Hint: Hitta din tjejpolare Anita, hon brukar hänga på Golden Hits och kan säkert hjälpa dig. " << endl;         
        cout << string(80,'-') << endl;
    }

    bool Controller::handle_other_commands(string cmd, string cmd_root, string cmd_rest) {
        if (cmd_root == "ladda") {
            if (!file_exists(cmd_rest + ".sav"))
                cout << "Hittade ingen sparning" << endl;
            else {
                load_game(cmd_rest + ".sav");
                cout << string(80,'-') << endl;
                cout << "Du laddade ett sparat spel" << endl;
                cout << string(80,'-') << endl;
                cout << player->get_location()->description() << endl;
            }
        } else if (cmd_root == "avsluta") {
            return true;
        } else {                    
            cout << "Felaktigt kommando: " << cmd << endl;
        }
        return false;
    }

    int check_win(Main_Actor * player) {
        if (player->check_win()) {
            cout << "Du kan nu komma in på VIP:en på VIP:en " << 
                "på Spy Bars dubbel-VIP när du vill; Grattis" << endl;
            return 1;                    
        }
        else 
            return 0;  
                
    }

    void Controller::save_wrapper() const {
        cout << "En taxi stannar på gatan. Vill du spara? (ja/nej)" << endl;
        string answer = read();
        if (answer == "ja") {
            cout << "Ange namn på sparning:" << endl;
            answer = read();
            save_game(answer + ".sav");
            cout << string(80,'-') << endl;
            cout << "Spelet " << answer << " sparades och kan laddas med kommandot 'ladda " << answer << "'" << endl;
            cout << string(80,'-') << endl;
        }
    }
    
    void Controller::respawn() {
        cout << string(80,'-') << endl;
        cout << "Det svartnar för ögonen! Du vaknar utan minne liggandes på stans " 
             << "sunkigaste toalett, bra jobbat JW!" << endl;
        player->transport_to(respawn_location);
        player->set_hp(80);
    }

    void Controller::leave_queue() {
        while (taxi->visitors.size() > 0 && !(taxi->visitors.size() == 1 && taxi->visitors.front() == player)) {
            Actor * d;
            (taxi->visitors.front() != player) ? 
                d = taxi->visitors.front() : d = taxi->visitors.back();
            remove_character(d);
        }
    }

    void Controller::arrive_in_queue(mt19937 gen) {
        if (actors.size() < 25) {
            discrete_distribution<int> dist {8,1,1};
            int type = dist(gen);
            switch (type) {
            case 0: { break; }
            case 1: {
                if (ib_ptr == ib_names.end())
                    ib_ptr = ib_names.begin();
                Actor* ib = create_ib(*ib_ptr++);
                actors.push_back(ib);
                connect(taxi,ib);
                cout << ib->get_Name() << " kom till " << taxi->get_Name() << "." << endl;
                break;
                        
            }
            case 2: {
                if (gf_ptr == gf_names.end())
                    gf_ptr = gf_names.begin();  
                Visitor* gf = static_cast<Visitor*>(create_girlfriend(*gf_ptr++));
                gf->set_action_lambda(create_gf_action_l(gf));
                actors.push_back(gf);
                connect(taxi,gf);
                cout << gf->get_Name() << " kom till " << taxi->get_Name() << "." << endl;
                break;
            }
            }            
        }
    }
    
    int Controller::run_game() {  
        print_start_message();
        unsigned seed = chrono::system_clock::now().time_since_epoch().count();
        gen = mt19937 (seed);
         
        while (true) {
            cout << player->get_location()->description() << endl;
            bool same_turn = true;
            while (same_turn) {
                same_turn = true;
                string command = read();
                pair<string,string> split = split_at_first_space(command);
                string cmd_root = split.first;
                string cmd_rest = split.second;
                
                if (actions.find(cmd_root) != actions.end())
                    same_turn = (player->*actions.at(cmd_root)) (cmd_rest);
                else if (handle_other_commands(command,cmd_root,cmd_rest))
                    return 0;
            }
            
            if (player->get_hp()==0)
                respawn();
            
            cout << string(80,'-') << endl;

            if (check_win(player))            
                return 0;
            
            if (player->get_location() == taxi)
                save_wrapper();
            
            // Player actions
            for (auto it = actors.begin(); it != actors.end(); ++it)
                (**it).action();      
            
            if (player->get_hp()==0)
                respawn();
            
            leave_queue();
            arrive_in_queue(gen);
                
            cout << string(80,'-') << endl;
        }
    }
} //end of namespace
