#include <iostream>
#include <vector>
#include <array>
#include <cmath>
#include <random>
#include <iostream>



bool y_n = 0;                                                                                                                       // asking user for ai analysis
bool y_n2 = 0;                                                                                                                      // asking user if they want to choose moves 
bool users_turn = 0;

int users_wins = 0;                                                                                                                 //all the t_ are tournament stats
int t_battles = 0;
int t_turns = 0;
int t_critical_hits = 0;
int t_super_eff_hits = 0;
int t_status_eff_hits = 0;


int rando(int x, int y) {
    std::random_device rd; 
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<int> distrib(x, y);
    return distrib(gen);
}                                                                                                                                   //random number generator function

struct Move {
    std::string name;
    int move_power;
    std::string status_eff;
    
};                                                                                                                                  //special structure for player moves


class Bender {
    public:
    int hp,attack,defense,speed, maxhp;
    double type_multiplier = 1;
    int critical_count = 0, super_eff_count = 0, status_eff_count = 0;
    bool buried = 0, frozen = 0, burned = 0;
    bool buried_used = 0, frozen_used = 0, burned_used = 0, heal_used = 0;    
    int burned_count = 0;
    std::string name, type;
    std::array<Move,4> moves;
    Bender(std::string n, std::string t, int h, int a, int d, int s, std::array<Move,4> m) : name(n), type(t), hp(h), attack(a), defense(d), speed(s), moves(m), maxhp(h){}
    
    void attackmove(Bender& enemy, int move_index) {


        std::cout << name << " strikes back!" << std::endl;
        double base_damage = (double(attack) * (moves[move_index]).move_power)/enemy.defense;
        int critical_multiplier = 1;
        int critical_hit = rando(0,9);
        int frozen_prob = rando(0,1);
        if (critical_hit==7) critical_multiplier = 2;                                                                                                           //using the random number function to get a 1/10 probability
        int final_damage = std::round(base_damage*type_multiplier*critical_multiplier);
        if (final_damage==0) final_damage = 1;
        enemy.hp -= final_damage;
        if(enemy.hp < 0) enemy.hp = 0;
        
        std::cout << name << " used " << (moves[move_index]).name << "!" << std::endl;                                                                          // printing the attack move

        if(type_multiplier == 2) { std::cout << "Super Effective! (" << type << " is strong against " << enemy.type << ")" << std::endl; super_eff_count+=1;}   // effective hits based on elemental advantage
        else if(type_multiplier == 0.5) { std::cout << "Not very effective... (" << type << " is weak against " << enemy.type << ")" << std::endl;}
        if (critical_multiplier == 2) {std::cout << "Critical Hit!" << std::endl; critical_count +=1;} // critical hit based on random 
        
        if(((moves[move_index]).status_eff == "Buried") && !(buried_used)) { enemy.buried = 1; buried_used = 1;  std::cout << enemy.name << " is now buried!" <<std::endl << std::endl; status_eff_count += 1;} // checking if any buried effect used this turn
        else if(((moves[move_index]).status_eff == "Frozen") && !(frozen_used) && (frozen_prob)) { enemy.frozen = 1; frozen_used = 1; std::cout << enemy.name << " is now frozen!" <<std::endl << std::endl; status_eff_count += 1;} // checking if frozen effect used and worked this turn
        else if(((moves[move_index]).status_eff == "Frozen") && !(frozen_used) && !(frozen_prob)) { frozen_used = 1; std::cout << enemy.name << " dodged the ice blast!" <<std::endl << std::endl;} // checking if frozen effect used and didnt work thi turn
        else if(((moves[move_index]).status_eff == "Burned") && !(burned_used)) { enemy.burned = 1; burned_used = 1; status_eff_count += 1;} // checking if burned effect is used this turn
        
        std::cout << enemy.name << " took " << final_damage << " damage!" << std::endl;

        if((enemy.burned) && !((enemy.burned_count) >= 4)) { //burning damage for 4 moves
            if (enemy.burned_count == 0) { std::cout << enemy.name << " starts burning now!" <<std::endl; }
            std::cout <<  enemy.name << " is burning and suffers a further "<< std::round(0.1 * enemy.maxhp) << " damage!" << std::endl;
            enemy.burned_count += 1;
            enemy.hp -= std::round(0.1 * enemy.maxhp);
            if (enemy.hp < 0) enemy.hp = 0; 
        }
        if ((enemy.burned_count) >= 4) burned_used = 1;
        std::cout << enemy.name << "HP: " << enemy.hp << "/" << enemy.maxhp << std::endl << std::endl;
            
        
    }

    void displaystats() {
        std::cout << name << " (" << type << ") - HP: " << hp << "/" << maxhp << ", Attack: " << attack << ", Defense: " << defense << ", Speed: " << speed << std::endl << "Moves: "; // displaying stats method when called
        for(Move x : moves) std::cout << x.name << " (" << x.move_power << "); ";
        std::cout << std::endl << std::endl;
    }

    bool is_fainted() {
        if(hp<=0) return 1;
        else return 0;
    }  
    
};


class Duel {
    public:
    Bender p1, p2;
    Bender winner = p1;
    Bender loser = p2;
    //bool yn; //for yes or no ai
    Duel(Bender player1, Bender player2) : p1(player1), p2(player2) {}
    Bender* attacker = &p1;
    Bender* defender = &p2;

    void start_duel() {
        std::array<std::string,4> elementlist = {"Water", "Fire", "Air", "Earth"};

        for(int x = 0; x < 4; x++) {                                                                                                                        // deciding the type multiplier based on bender type
            if (p1.type == elementlist[x]) {
                for(int y = 0; y<4; y++) {
                    if (p2.type == elementlist[y]) {
                        if ((x+1 == y) || ((x==3) && (y==0))) {
                            p1.type_multiplier = 2;
                            p2.type_multiplier = 0.5;
                        }
                        else if ((x == y+1) || ((x==0) && (y==3))) {
                            p1.type_multiplier = 0.5;
                            p2.type_multiplier = 2;
                        }
                    }
                }
            }
        }

        std::cout << "\t\t\t\t=== DUEL BEGINS! ===" << std::endl;
        std::cout << "\t\t" << p1.name << " (" << p1.type << ", HP: " << p1.hp << "/" << p1.maxhp << ") VS "; 
        std::cout << p2.name << " (" << p2.type << ", HP: " << p2.hp << "/" << p2.maxhp << ")" << std::endl << std::endl << std::endl;
        int turn = 1;
        int buried_turn_count = 0;                                                                                                                          // how many turns skipped while buried so far
        int buried_turns = rando(1,4);                                                                                                                      // choosing how many number of turns skipped for burry
        int frozen_turn_count = 0;                                                                                                                          // how many turns skipped while frozen so far
        
        
        if(p1.speed>p2.speed)  { attacker = &p1; defender = &p2; users_turn = 1; } // deciding who starts first
        else if(p2.speed>p1.speed)  { attacker = &p2; defender = &p1; users_turn = 0; }
        else {
            int rnum = rando(0,1);                                                                                                                          // again using the rng function to choose who starts when speeds tie
            switch(rnum) {
                case(0): {attacker = &p2; defender = &p1; break;}
                case(1): {attacker = &p1; defender = &p2; break;}
                default: break;
            }
        }

        std::cout << "Turn 1: " << (*attacker).name << " goes first! (Speed: " << (*attacker).speed << " vs " << (*defender).speed << ")" << std::endl;

        while(67) {
            int move_num = 0;

            if(y_n2){                                                                                                                                       // checking if user is playing as a player
                if(users_turn) { 
                std::cout << "[" << (*attacker).name << ": {";
                int i = 0;
                for(Move x : (*attacker).moves) { std::cout << x.name << "[" << i << "] (Power:" << x.move_power << "); " ; i++; } // displaying moves for user to play
                std::cout << "Heal[4];";
                std::cout << "} ]" << std::endl;
                std::cout << "Enter Move Number(0,1,2,3,4): ";
                std::cin >> move_num;
                }
                else {
                    if(y_n) { move_num = choose_move(); }                                                                                                   // ai analysis choosing move
                    else { move_num = rando(0,4); }                                                                                                         // random choosing move
                    }
                }

            else {
                if(y_n) { move_num = choose_move(); } 
                else { move_num = rando(0,4); }                                                                                                         
            }


            if (move_num == 4) { //healing move
                if (!(*attacker).heal_used) {
                (*attacker).hp += std::round(0.5*(*attacker).maxhp);
                if ((*attacker).hp>(*attacker).maxhp) (*attacker).hp = (*attacker).maxhp;
                (*attacker).heal_used = 1;
                std::cout << (*attacker).name << " healed for " << std::round(0.5*(*attacker).maxhp) << " health!" << std::endl << std::endl;
                if((*attacker).name==p1.name) {attacker = &p2; defender = &p1; users_turn = 0;}                                                             // changing turns between attacks if no burning or freezing effects present (after healing move)
                else if((*attacker).name==p2.name) {attacker = &p1; defender = &p2; users_turn = 1;}

                }
                else {
                    std::cout << "Healing did not work (already used)!" << std::endl;
                }
            }
            else if((move_num != 0) && (move_num != 1) && (move_num != 2) && (move_num != 3) && (move_num != 4)) std::cout << "ENTER A VALID INPUT" << std::endl;
            
            else {
            (*attacker).attackmove((*defender),move_num); // the attacking move

            if(((*attacker).is_fainted()) || ((*defender).is_fainted())) break;                                                                             // only condition to break the always-true while loop
            
            if(((*attacker).buried) || ((*defender).buried)) {                                                                                              // burried for the random number of turns
                turn += 1;

                std::cout << "Turn " << turn << ": " << (*defender).name << " is buried and cannot move!" << std::endl;
                std::cout << "Buried duration: " << buried_turns - buried_turn_count << " turns remaining" << std::endl << std::endl;
                buried_turn_count += 1;
                if (buried_turn_count == buried_turns) {
                    (*defender).buried = 0;
                }
            }
            else if(((*attacker).frozen) || ((*defender).frozen)) {                                                                                         //frozen for 3 turns
                turn += 1;

                std::cout << "Turn " << turn << ": " << (*defender).name << " is frozen in a giant solid block of ice!" << std::endl; 
                std::cout << "Frozen duration: " << 3 - frozen_turn_count << " turns remaining" << std::endl << std::endl;
                frozen_turn_count += 1;
                if (frozen_turn_count == 3) {
                    (*defender).frozen = 0;
                }
            }

            else {
                if((*attacker).name==p1.name) {attacker = &p2; defender = &p1; users_turn = 0;}                                                             // changing turns between attacks if no burning or freezing effects present
                else if((*attacker).name==p2.name) {attacker = &p1; defender = &p2; users_turn = 1;}
            }

            turn += 1;
            std::cout << "Turn " << turn << ": ";
            }
        
        }
        std::cout << (*defender).name << " fainted!" << std::endl;
        std::cout << "- " << (*attacker).name << " wins the duel!" << std::endl << std::endl;
        std::cout << "Duel Summary: " << std::endl << "- Winner: " << (*attacker).name << std::endl;
        std::cout << "- Turns: " << turn << std::endl;
        std::cout << "- Critical Hits: " << (*attacker).critical_count + (*defender).critical_count << std::endl;
        std::cout << "- Super Effective Hits: " << (*attacker).super_eff_count + (*defender).super_eff_count << std::endl << "------------------------" << std::endl << std::endl << std::endl;

        t_battles += 1; //updating this duels stats to total tournament stats
        t_turns += turn;
        t_critical_hits += (*attacker).critical_count + (*defender).critical_count;
        t_super_eff_hits += (*attacker).super_eff_count + (*defender).critical_count;
        t_status_eff_hits += (*attacker).status_eff_count + (*defender).critical_count;

        winner = (*attacker);
        loser = (*defender);

        if(winner.name == p1.name) users_wins += 1;
    
        winner.hp = (*attacker).maxhp;
        winner.type_multiplier = 1;
        winner.critical_count = 0; winner.super_eff_count = 0;
        winner.buried = 0; winner.frozen = 0; winner.burned = 0;
        winner.buried_used = 0; winner.frozen_used = 0; winner.burned_used = 0, winner.heal_used = 0;
        winner.burned_count = 0;
        users_turn = 0;                                                                                                                                     // winner stats reset for next match
        
    }

    int choose_move() {
        if ((*attacker).hp < (0.3*(*attacker).maxhp) && !(*attacker).heal_used){std::cout << "AI CHOOSES: HEALING MOVE " << std::endl;  return 4; }         // 4 means healing move

        else if (((*defender).hp > (0.7*(*attacker).maxhp)) && ((!(*attacker).burned_used) && (!(*attacker).frozen_used) && (!(*attacker).buried_used))) {
            std::cout << "AI CHOOSES: STATUS MOVE" << std::endl;
            for(int i=0;i<4;i++) {
            if(!((*attacker).moves[i].status_eff == "")) { return i; break; }
            } // i returns the move index of the status effect move
        }

        else if ((*defender).hp < (0.25*(*defender).maxhp)) {
            std::cout << "AI CHOOSES: STRONGEST MOVE" << std::endl;
            int maxpower_index = 0;
            for(int i=0;i<4;i++) {
            if((((*attacker).moves[i]).move_power > ((*attacker).moves[maxpower_index]).move_power)) maxpower_index = i;
            }
            return maxpower_index; 
        } // i returns move index of the move with highest move power
        else {
            std::cout << "AI CHOOSES: EFFICIENT MOVE" << std::endl;
            int maxpower_index = 0;
            for(int i=0;i<4;i++) {
            if((((*attacker).moves[i]).move_power > ((*attacker).moves[maxpower_index]).move_power)) maxpower_index = i;
            }
            return maxpower_index;
        }
        return 0;                                                                                                                                           //most effective move is also the strongest move because same elemental advantage for all 4 moves
    }

};

std::vector<Bender> winners {};
std::vector<Bender> available {};
std::vector<Bender> losers {};

struct player_wins {
    Bender p;
    int wins;
};

class Tournament {
    public: 
    std::vector<Bender> players;
    bool yesorno; //for yes or no ai
    Tournament(std::vector<Bender> participants) : players(participants) {}
    void start() {
        Bender first_place = players[0];
        Bender second_place = players[0];
        Bender third_place1 = players[0];
        Bender third_place2 = players[0];
        std::vector<Bender> third;
        std::vector<player_wins> p_wins;

        for(int i = 0; i<players.size(); i++) {                                                                                                             // initializing the wins vector
            p_wins.push_back({players[i], 0});
        }

        for(Bender player : players) {available.push_back(player);}
        for(int x = (players.size()); x>1; x/=2) {                                                                                                          // each set of rounds, half of the people get eliminated
            while(available.size()>1) {
                int rnum = rando(0,(available.size()-1));                                                                                                   // randomizing players from the available set to pair up
                int rnum2 = rando(0,(available.size()-1));
                while(rnum2 == rnum) {                                                                                                                      // cant be the same player playing itself
                    if (rnum2 != rnum) break;
                    rnum2 = rando(0,(available.size()-1));
                }
                Duel duel(available[rnum], available[rnum2]);
                duel.start_duel();

                for(int i = 0; i < players.size(); i++) {                                                                                                   // adding the win to the wins vector
                    if(p_wins[i].p.name == duel.winner.name) {
                        p_wins[i].wins += 1;
                    }
                }
                if(available[rnum].name == (duel.winner).name) {winners.push_back(available[rnum]); losers.push_back(available[rnum2]);}                    // adding to the winners and losers arrays
                else if (available[rnum2].name == (duel.winner).name) {winners.push_back(available[rnum2]); losers.push_back(available[rnum]);}
                if (rnum>rnum2) { available.erase((available.begin()) + rnum); available.erase(available.begin() + rnum2);  }                               // we have to remove the higher index element first becuase index of the higher index element shifts down by 1 if we remove the lower one first
                else if (rnum2>rnum) { available.erase((available.begin()) + rnum2); available.erase(available.begin() + rnum); }
            }
            if(winners.size() != 1) {
                std::cout << std::endl << std::endl << std::endl << std::endl << "\t\t\t\t  Players moving on to next round are : ";
            for(Bender player : winners) {available.push_back(player); std::cout << player.name << ", ";}
                winners = {};
                std::cout << std::endl << std::endl << std::endl << std::endl << std::endl;
            
            }
            
            else { // when 1 winner left
                std::cout << std::endl << std::endl << std::endl << std::endl << "\t\t\t\t "<< winners[0].name << " HAS WON THE TOURNAMENT!" << std::endl<< std::endl << std::endl << std::endl;
                winners[0].displaystats();
                std::cout << std::endl << std::endl << "=== TOURNAMENT STATISTICS ===" << std::endl;

                std::cout << "Champion: " << ((winners[0]).name) << std::endl;
                std::cout << "Total Battles: " << t_battles << std::endl;
                std::cout << "Total Turns: " << t_turns << std::endl;
                std::cout << "Critical Hits: " << t_critical_hits << std::endl;
                std::cout << "Status Effects: " << t_status_eff_hits << std::endl;
                std::cout << "Super Effective Hits: " << t_super_eff_hits << std::endl << std::endl;

                int i = 0;
                for(int n = players.size(); n > 1; n/=2) { i += 1; }                                                                                            // i will be the number of games played by each player (log(n) (base 2))
                
                for(player_wins x  : p_wins) {
                    if (x.wins == i) first_place = x.p;                                                                                                         //first player wins all games played
                    else if (x.wins == i-1) second_place = x.p;                                                                                                 //second player only loses one game (the last one)
                    else if (x.wins == i-2) third.push_back(x.p);                                                                                               //third place has 2 players where both lost only 2 games (if number of players is >= 4)
                }
                std::cout << "Final Standings:" << std::endl;
                std::cout << "1st: " << first_place.name << std::endl;
                std::cout << "2nd: " << second_place.name << std::endl;
                if(players.size()>=4) std::cout << "3rd: " << (third[0]).name << " & " << (third[1]).name << " (tied)" << std::endl << std::endl;               // condition necessary because it wont work in a tournament with 2 people

                if(y_n2) {
                    std::cout << "You won in " << users_wins << " battles!" << std::endl;
                }
            }
            
    }

    }
};


int main() {
    char yesorno;
    char yesorno2;
    
    std::vector<Bender> players = {
        Bender("Ignis", "Fire", 120, 82, 70, 95, {Move{"Inferno Slash", 60}, Move{"Flame Dash", 38}, Move{"Ember Guard", 0}, Move{"Volcanic Burst", 80}}),
        Bender("Kestra", "Water", 128, 72, 85, 68, {Move{"Tidal Crush", 65}, Move{"Ice Shard", 45}, Move{"Mist Shield", 0}, Move{"Maelstrom", 85}}),
        Bender("Terrak", "Earth", 135, 88, 90, 45, {Move{"Stone Avalanche", 50}, Move{"Quake Punch", 48}, Move{"Bulwark", 0}, Move{"Mountain's Wrath", 38}}),
        Bender("Squall", "Air", 105, 65, 55, 100, {Move{"Thunder Gale", 58}, Move{"Razor Wind", 35}, Move{"Updraft", 0}, Move{"Tempest Strike", 72}}),
        Bender("Nadia", "Fire", 85, 48, 60, 72, {Move{"Wave Crash", 35}, Move{"Splash Kick", 25}, Move{"Guard", 0}, Move{"Riptide", 50}}),
        Bender("Talon", "Water", 70, 100, 60, 74, {Move{"Oblivion Wing", 38}, Move{"Astral Barrage", 28}, Move{"Medkit", 0}, Move{"Atomic Bomb", 48}}),
        Bender("Mira", "Earth", 130, 38, 40, 92, {Move{"Blue Flare", 35}, Move{"Glaive Rush", 25}, Move{"Riot Shield", 0}, Move{"Riptide", 50}}),
        Bender("Kael", "Air", 90, 52, 55, 72, {Move{"Gale Strike", 58}, Move{"Wind Cutter", 38}, Move{"Updraft", 0}, Move{"Draco Meteor", 80}})
    };

    std::vector<std::string> status_eff_list {"Burned", "Frozen", "Buried", "None"};                                                                            // assigning random status effect to each bender
    for(int i = 1; i < players.size(); i++) { 
        int a = rando(0,3);
        (players[i].moves)[2].status_eff = status_eff_list[a];
    }

    int n = players.size();                                                                                                                                     //making sure that number of players is power of 2
    while(n%2 == 0)  {
        n/=2;
    }
    if(n!=1) {std::cout << "Number of players in the tournament must be a power of 2" << std::endl; return 0;}
    
    std::cout << "Do you want to play as a player? (Y/N) : ";                                                                                                   //asking if player wants to choose moves
    std::cin >> yesorno2;
    if((yesorno2 == 'Y') || (yesorno2 == 'y')) y_n2 = 1;
    else if((yesorno2 == 'N') || (yesorno2 == 'n')) y_n2 = 0;
    else { std::cout <<  "INVALID INPUT" << std::endl; return 0; }

    std::cout << "AI Analysis? (Y/N) : ";                                                                                                                       //asking for ai analysis
    std::cin >> yesorno; 
    if((yesorno == 'Y') || (yesorno == 'y')) y_n = 1;
    else if((yesorno == 'N') || (yesorno == 'n')) y_n = 0;
    else { std::cout <<  "INVALID INPUT" << std::endl; return 0; }

    
    Tournament game(players);
    game.start();
    return 0;
}