#include "worldcup23a2.h"

bool CompareById(const Team& team1, const Team& team2){
    return team1.GetId() < team2.GetId();
}

bool CompareByAbility(const Team& team1, const Team& team2){
    if (team1.GetAbility() < team2.GetAbility()){
        return true;
    }else if (team1.GetAbility() == team2.GetAbility() &&
    team1.GetId() < team2.GetId()){
        return true;
    }
    return false;
}

void DeletePlayerNode(Node<int, Player*>* node){
    delete node->value;
    delete node;
}

world_cup_t::world_cup_t() : teamsById(nullptr), teamsByAbility(nullptr), players(nullptr) {}



world_cup_t::~world_cup_t()
{
    players->MakeEmpty(&DeletePlayerNode);
    delete players;
    delete teamsById;
    delete teamsByAbility;
}

StatusType world_cup_t::add_team(int teamId)
{
    Team tempTeam = Team(teamId);
    if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }else if (this->teamsById != NULL && this->teamsById->find(tempTeam, &CompareById) != nullptr){
        return StatusType::FAILURE;
    }
    try{
        Team* newTeam = new Team(teamId);
        if (this->teamsById == nullptr){
            this->teamsById = new AVL_Rank<Team>();
        }
        if (this->teamsByAbility == NULL){
            this->teamsByAbility = new AVL_Rank<Team>();
        }
        this->teamsById->insert(newTeam, &CompareById);
        this->teamsByAbility->insert(newTeam, &CompareByAbility);
    }catch(const std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

StatusType world_cup_t::remove_team(int teamId)
{
    Team tempTeam = Team(teamId);
	if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }else if (this->teamsById == nullptr ||
    this->teamsById->find(tempTeam, &CompareById) == nullptr){
        return StatusType::FAILURE;
    }
    try{
        Team* team = this->teamsById->find(tempTeam, &CompareById);
        team->SetActive(false);
        this->teamsById->remove(*team, &CompareById);
        this->teamsByAbility->remove(*team, &CompareByAbility);
    }catch(...){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::FAILURE;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    Team temp = Team(teamId);
    if(playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    if((players != nullptr && players->DoesPlayerExist(playerId)) || (teamsById != NULL && teamsById->find(temp, &CompareById) == NULL))
        return StatusType::FAILURE;
    try{
        Team* team = teamsById->find(temp, &CompareById);
        Player* newPlayer;
        if (team != NULL && team->GetNumOfPlayers() == 0){
            newPlayer = new Player(playerId, gamesPlayed, ability, cards, goalKeeper, team, spirit);
            Node<int, Player*>* newNode = players->makeset(*newPlayer, teamId);
            team->SetLeader(newNode);
        } else  if (team != NULL && team->GetNumOfPlayers() > 0){
            newPlayer = new Player(playerId, gamesPlayed, ability, cards, goalKeeper, NULL, spirit);
            players->addPlayerToTeam(*newPlayer, teamId);
            ///TODO: MAKE SURE THAT THE REQUIRED UPDATES FOR THE TEAM WILL OCCUR.
        }
    }catch(const std::bad_alloc&){
        return StatusType::ALLOCATION_ERROR;
    }
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::play_match(int teamId1, int teamId2)
{

	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
	// TODO: Your code goes here
	return 22;
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
	// TODO: Your code goes here
	return 30003;
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
	// TODO: Your code goes here
	return 12345;
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
	// TODO: Your code goes here
	return permutation_t();
}

StatusType world_cup_t::buy_team(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	return StatusType::SUCCESS;
}
