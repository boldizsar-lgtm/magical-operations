#include "Player.hpp"
#include "Config.hpp"
#include "Zone.hpp"


Player::Player(int id) : id_{id}, field_zone_{ZoneType::FIELD}, battle_zone_{ZoneType::BATTLE}, done_{false}
{
  Config& config = Config::getInstance();
  deck_ = config.getDeck(id);

  //setting base values
  base_health_ = config.getBaseHealth();
  mana_pool_ = config.getBaseMana();

  //setting current values to base values
  current_health_ = base_health_;
  current_mana_ = mana_pool_;

  //draw 6 hand cards from the deck
  for (int i = 0; i < 6; i++)
  {
    drawCard();
  }
}

void Player::changeCurrentHealth(int amount)
{
  current_health_ += amount;
}

void Player::changeCurrentMana(int amount)
{
  current_mana_ += amount;
}

void Player::refillCurrentMana()
{
  current_mana_ = mana_pool_;
}

void Player::increaseManaPool(int amount)
{
  mana_pool_ += amount;
}

bool Player::drawCard()
{
  Card* card = deck_->getPileTop();

  if (card == nullptr)
  {
    return false;
  }

  hand_.addCardEnd(card);
  deck_->removeTopCard();
  return true;
}

void Player::printPlayerPrompt() const
{
  std::cout << "\nP" << id_ << "> ";
}

void Player::printGraveyard() const
{
  std::cout << Config::getInstance().getMessage("D_BORDER_GRAVEYARD") << std::endl;
  graveyard_.printGraveyard();
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
}

void Player::printHand() const
{
  std::cout << Config::getInstance().getMessage("D_BORDER_HAND") << std::endl;
  hand_.printCardPile();
  std::cout << Config::getInstance().getMessage("D_BORDER_D") << std::endl;
}

void Player::printFieldZone() const
{
  for (int i = 0; i < 4; i++)
  {
    std::cout << "F   ";
    field_zone_.printZone(i);
    std::cout << "F" << std::endl;
  }
}

void Player::printBattleZone() const
{
  for (int i = 0; i < 4; i++)
  {
    std::cout << "B   ";
    battle_zone_.printZone(i);
    std::cout << "B" << std::endl;
  }
}

void Player::redrawHand()
{
//TODO
}

void Player::playCreature(Card* card, std::string field_slot)
{
  // user Binary Worrier, stackoverflow comment
  // from: https://stackoverflow.com/a/439589
  // begin
  int zone_column = field_slot.at(1) - '0';
  // end

  // Remove card from hand
  removeCardFromPile(PileType::HAND, card->getId());

  // Add card to field zone slot

  // *** STILL NEED TO MAKE IT SO WHEN A CREATURE ENTERS THE BOARD, IT'S CURRENT VALUES ARE SET TO ITS BASE VALUES, PROBABLY BEST DONE IN addCreature() ***
  field_zone_.addCreature(dynamic_cast<Creature*>(card), zone_column);

  // Print info
  std::string card_info_string = "I_" + card->getId();
  std::cout << Config::getInstance().getMessage(card_info_string) << std::endl; 

  // take away mana
  current_mana_ -= card->getManaCost();
}

void Player::removeCardFromPile(PileType pile_type, std::string id)
{
  if (pile_type == PileType::HAND)
    hand_.removeCard(id);
  else if (pile_type == PileType::DECK)
    deck_->removeCard(id);
  else if (pile_type == PileType::GRAVEYARD)
    graveyard_.removeCard(id);
}

bool Player::isFieldSlotOccupied(std::string field_slot)
{
  char zone_letter = field_slot.at(0);

  // See zone_column usage in playCreature() for credit
  int zone_column = field_slot.at(1) - '0';

  if (zone_letter == 'B')
    return battle_zone_.isOccupied(zone_column);
  else
    return field_zone_.isOccupied(zone_column);
}

std::string Player::getRoleString() const
{
  if (role_ == Role::ATTACKER)
    return "Attacker";
  else
    return "Defender";
}

int Player::getCurrentHealth() const
{
  return current_health_;
}

int Player::getCurrentMana() const
{
  return current_mana_;
}

int Player::getManaPool() const
{
  return mana_pool_;
}

int Player::getId() const
{
  return id_;
}

int Player::getDeckSize() const
{
  return deck_->getSize();
}


CardPile* Player::getDeck() const
{
  return deck_;
}

const CardPile& Player::getGraveyard() const 
{
  return graveyard_;
}

const CardPile& Player::getHand() const 
{
  return hand_;
}

bool Player::getDoneStatus() const
{
  return done_;
}

const Zone& Player::getZone(ZoneType type) const
{
  if (type == ZoneType::BATTLE)
    return battle_zone_;
  else if (type == ZoneType::FIELD)
    return field_zone_;
}

void Player::setRole(Role role)
{
  role_ = role;
}

void Player::switchRole()
{
  if (role_ == Role::ATTACKER)
    role_ = Role::DEFENDER;
  else
    role_ = Role::ATTACKER;
}


void Player::setDoneStatus(bool status)
{
  done_ = status;
}
