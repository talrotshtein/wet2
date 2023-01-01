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
	return StatusType::SUCCESS;
}

StatusType world_cup_t::add_player(int playerId, int teamId,
                                   const permutation_t &spirit, int gamesPlayed,
                                   int ability, int cards, bool goalKeeper)
{
    Team temp = Team(teamId);
    if(playerId <= 0 || teamId <= 0 || !spirit.isvalid() || gamesPlayed < 0 || cards < 0)
        return StatusType::INVALID_INPUT;
    if((players != nullptr && players->DoesPlayerExist(playerId)) ||
    (teamsById != NULL && teamsById->find(temp, &CompareById) == NULL))
        return StatusType::FAILURE;
    try{
        Team* team = teamsById->find(temp, &CompareById);
        Player* newPlayer;
        if (players == NULL)
            players = new UnionFind();
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
    if (teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2){
        return StatusType::INVALID_INPUT;
    }
    if (teamsById == NULL)
        return StatusType::FAILURE;
    Team team1cpy = Team(teamId1);
    Team team2cpy = Team(teamId2);
    Team* team1 = teamsById->find(team1cpy, &CompareById);
    Team* team2 = teamsById->find(team2cpy, &CompareById);
    if (team1 == NULL || team2 == NULL)
        return StatusType::FAILURE;
    else if (team1->GetNumGoalkeepers() == 0 || team2->GetNumGoalkeepers() == 0)
        return StatusType::FAILURE;
    int winner;
    int team1Stats = team1->GetAbility() + team1->GetPoints();
    int team2Stats = team2->GetAbility() + team2->GetPoints();
    if (team1Stats > team2Stats){
        team1->SetPoints(team1->GetPoints() + 3);
        winner = 1;
    }else if (team1Stats == team2Stats){
        if (team1->GetStrength() > team2->GetStrength()){
            team1->SetPoints(team1->GetPoints() + 3);
            winner = 2;
        }else if (team1->GetStrength() == team2->GetStrength()){
            team1->SetPoints(team1->GetPoints() + 1);
            team2->SetPoints(team2->GetPoints() + 1);
            winner = 0;
        }else{
            team2->SetPoints(team2->GetPoints() + 3);
            winner = 4;
        }
    }else{
        team2->SetPoints(team2->GetPoints() + 3);
        winner = 3;
    }
    team1->SetGamesPlayed(team1->GetGamesPlayed() + 1);
    team2->SetGamesPlayed(team2->GetGamesPlayed() + 1);
    team1->GetLeader()->value->SetGamesPlayed(team1->GetLeader()->value->GetGamesPlayed()+1);
    team2->GetLeader()->value->SetGamesPlayed(team2->GetLeader()->value->GetGamesPlayed()+1);
	return output_t<int>{winner};
}

output_t<int> world_cup_t::num_played_games_for_player(int playerId)
{
    if (playerId <= 0){
        return output_t<int>{StatusType::INVALID_INPUT};
    }
    else if (players == NULL || !players->DoesPlayerExist(playerId)){
        return output_t<int>{StatusType::FAILURE};
    }
	return output_t<int>{players->NumPlayedGamesForPlayer(playerId)};
}

StatusType world_cup_t::add_player_cards(int playerId, int cards)
{
    if (playerId <= 0 || cards < 0){
        return StatusType::INVALID_INPUT;
    }
    else if (players == NULL || !players->DoesPlayerExist(playerId)){
        return StatusType::FAILURE;
    }
    Node<int, Player*>* team = players->find(playerId);
    if (team == NULL || team->value->GetTeam() == NULL || !team->value->GetTeam()->IsActive()){
        return StatusType::FAILURE;
    }
    Node<int, Player*>* player = players->get(playerId);
    player->value->SetCards(player->value->GetCards() + cards);
    return StatusType::SUCCESS;
}

output_t<int> world_cup_t::get_player_cards(int playerId)
{
	if (playerId <= 0){
        return StatusType::INVALID_INPUT;
    }
    else if (players == NULL || !players->DoesPlayerExist(playerId)){
        return StatusType::FAILURE;
    }
    Node<int, Player*>* player = players->get(playerId);
	return output_t<int>{player->value->GetCards()};
}

output_t<int> world_cup_t::get_team_points(int teamId)
{
    Team temp = Team(teamId);
	if (teamId <= 0){
        return StatusType::INVALID_INPUT;
    }else if (teamsById == NULL || teamsById->find(temp, &CompareById) == NULL){
        return StatusType::FAILURE;
    }
    Team* team = teamsById->find(temp, &CompareById);
	return output_t<int>{team->GetPoints()};
}

output_t<int> world_cup_t::get_ith_pointless_ability(int i)
{
    if (teamsByAbility == NULL){
        return StatusType::FAILURE;
    }else if (i < 0 || i >= teamsByAbility->getTreeSize()){
        return StatusType::INVALID_INPUT;
    }
    Team* ithTeam = teamsByAbility->getIthNode(i);
    if (ithTeam == NULL)
        return StatusType::FAILURE;
	return output_t<int>{ithTeam->GetId()};
}

output_t<permutation_t> world_cup_t::get_partial_spirit(int playerId)
{
    if (playerId <= 0){
        return StatusType::INVALID_INPUT;
    }else if (players == NULL || !players->DoesPlayerExist(playerId)){
        return StatusType::FAILURE;
    }
    Node<int, Player*>* team = players->find(playerId);
    if (team == NULL || team->value->GetTeam() == NULL || !team->value->GetTeam()->IsActive()){
        return StatusType::FAILURE;
    }
	return output_t<permutation_t>{players->getPartialSpirit(playerId)};
}

StatusType world_cup_t::buy_team(int buyerId, int boughtId)
{
	if (buyerId <= 0 || boughtId <= 0 || buyerId == boughtId){
        return StatusType::INVALID_INPUT;
    }
    Team temp1 = Team(buyerId);
    Team temp2 = Team(boughtId);
    if (teamsById == NULL || teamsById->find(temp1, &CompareById) == NULL ||
    teamsById->find(temp2, &CompareById) == NULL){
        return StatusType::FAILURE;
    }
    Team* buyer = teamsById->find(temp1, &CompareById);
    Team* bought = teamsById->find(temp2, &CompareById);
    if (buyer->GetNumOfPlayers() == 0){
        teamsById->remove(temp1, &CompareById);
        teamsByAbility->remove(temp1, &CompareByAbility);
        bought->SetId(buyer->GetId());
        delete buyer;
    }else if (
bought->GetNumOfPlayers() == 0){
        teamsById->remove(temp2, &CompareById);
        teamsByAbility->remove(temp2, &CompareByAbility);
        delete bought;
    }
    Node<int, Player*>* team1Root = buyer->GetLeader();
    Node<int, Player*>* team2Root = bought->GetLeader();
    players->unite(team1Root, team2Root);
    return StatusType::SUCCESS;
}
