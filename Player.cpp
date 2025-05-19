#include "Player.hpp"
#include "Config.hpp"
#include "Zone.hpp"

#define STARTING_HAND_SIZE (6)
#define ZONE_WIDTH (7)

Player::Player(int id) : id_{id}, field_zone_{}, battle_zone_{}, redraw_possible_{true},
                         done_{false}
{
  Config& config = Config::getInstance();
  deck_ = config.getDeck(id);

  //setting base values
  base_health_ = config.getBaseHealth();
  mana_pool_ = config.getBaseMana();

  //setting current values to base values
  current_health_ = base_health_;
  current_mana_ = mana_pool_;

  //draw 7 hand cards from the deck
  for (int i = 0; i < STARTING_HAND_SIZE; i++)
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
  for (int i = 1; i <= 4; i++)
  {
    std::cout << "F   ";
    field_zone_.printZone(i);
    std::cout << "F" << std::endl;
  }
}

void Player::printBattleZone() const
{
  for (int i = 1; i <= 4; i++)
  {
    std::cout << "B   ";
    battle_zone_.printZone(i);
    std::cout << "B" << std::endl;
  }
}

void Player::redrawHand()
{
  int num_cards_to_pull = hand_.getSize() - 1;

  //Discard player's hand
  while (hand_.getSize() != 0)
  {
    deck_->addCardEnd(hand_.getPileTop());
    hand_.removeTopCard();
  }

  //Draw one less card than the original hand
  while (hand_.getSize() < num_cards_to_pull)
  {
    drawCard();
  }
}

void Player::moveToGraveyard(char zone, int column)
{
  if (zone == 'B')
  {
    graveyard_.addCardTop(battle_zone_.getCreature(column));
    battle_zone_.removeCreature(column);
  }
  
  else
  {
    graveyard_.addCardTop(field_zone_.getCreature(column));
    field_zone_.removeCreature(column);
  }
}

void Player::moveToGraveyard(Creature* creature)
{
  // Add creature to graveyard
  graveyard_.addCardTop(creature);

  // Remove Creature from wherever it is
  int column = 0;
  column = field_zone_.searchCreature(creature);
  if (column == -1)
  {
    battle_zone_.searchCreature(creature);
    battle_zone_.removeCreature(column);
  }
  else
    field_zone_.removeCreature(column);
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

  // Make card a creature
  Creature* creature = dynamic_cast<Creature*>(card);

  // Add card to field zone slot
  field_zone_.addCreature(creature, zone_column);

  // Make card entered_this_turn true
  creature->setEnter(true);
  
  // take away mana
  current_mana_ -= creature->getManaCost();

  // Print info
  std::string card_info_string = "I_" + creature->getId();
  std::cout << Config::getInstance().getMessage(card_info_string) << std::endl; 
}

void Player::addToFirstEmptyField(Creature* creature)
{
  for (int slot = 0; slot < ZONE_WIDTH; slot++)
  {
    if (field_zone_.getSlots()[slot] == nullptr)
    {
      field_zone_.switchCreaturePlace(creature, slot + 1);
      return;
    }
  }
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

void Player::moveToBattle(std::string field_slot, std::string battle_slot)
{
  // See zone_column usage in playCreature() for credit
  int field_column = field_slot.at(1) - '0';
  int battle_column = battle_slot.at(1) - '0';

  // Find creature card
  Creature* creature = field_zone_.getCreature(field_column);

  // Add card to battle zone
  battle_zone_.switchCreaturePlace(creature, battle_column);

  // Remove from field zone
  field_zone_.removeCreature(field_column);
}

void Player::moveToField(int battle_column)
{
  //get the first free field zone and insert creature, if no slot is free add to graveyard
  for (int i = 1; i <= 7; i++)
  {
    if (!field_zone_.isOccupied(i))
    {
      field_zone_.switchCreaturePlace(battle_zone_.getCreature(battle_column), i);
      battle_zone_.removeCreature(battle_column);
      return;
    }
  }
  moveToGraveyard('B', battle_column);
}


void Player::moveToField(Creature* creature)
{
  for (int i = 1; i <= 7; i++)
  {
    if (!field_zone_.isOccupied(i))
    {
      //move card from graveyard to field zone
      field_zone_.addCreature(creature, i);
      creature->setEnter(true);
      graveyard_.removeCard(creature->getId());
      return;
    }
  }
}

void Player::moveToHand(Creature* creature)
{
  // Set values to default values
  *creature = *dynamic_cast<Creature*>(Config::getInstance().getCard(creature->getId()));

  hand_.addCardEnd(creature);
  graveyard_.removeCard(creature->getId());
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

bool Player::getRedrawPossible()
{
  return redraw_possible_;
}

bool Player::getDoneStatus() const
{
  return done_;
}

const Zone& Player::getZone(char type) const
{
  if (type == 'B')
    return battle_zone_;
  else
    return field_zone_;
}

const Zone& Player::getZone(std::string type) const
{
  Utils::toUpper(type);

  if (type == "BATTLE")
    return battle_zone_;
  else
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

void Player::turnAllEnteredOff()
{
  field_zone_.turnZoneEnteredOff();
  battle_zone_.turnZoneEnteredOff();
}

void Player::setRedrawNotPossible()
{
  redraw_possible_ = false;
}

void Player::setDoneStatus(bool status)
{
  done_ = status;
}
