#include "Player.hpp"
#include "Config.hpp"
#include "Zone.hpp"


Player::Player(int id) : id_{id}, field_zone_{ZoneType::FIELD}, battle_zone_{ZoneType::BATTLE}
{
  Config& config = Config::getInstance();
  deck_ = config.getDeck(id);

  //setting base values
  base_health_ = config.getBaseHealth();
  mana_pool_ = config.getBaseMana();

  //setting current values to base values
  current_health_ = base_health_;
  current_mana_ = mana_pool_;
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
  Card* card = deck_.getPileEnd();

  if (card == nullptr)
    return false;

  hand_.addCard(card);
  deck_.removeTopCard();
  return true;
}

void Player::printGraveyard() const
{
//TODO
}

void Player::printHand() const
{
//TODO
}

void Player::printFieldZone() const
{
  for (int i; i <= 4; i++)
  {
    std::cout << "F  ";
    field_zone_.printZone(i);
    std::cout << "  F" << std::endl;
  }
}

void Player::printBattleZone() const
{
  for (int i; i <= 4; i++)
  {
    std::cout << "B  ";
    battle_zone_.printZone(i);
    std::cout << "  B" << std::endl;
  }
}

void Player::redrawHand()
{
//TODO
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

int Player::getId() const
{
  return id_;
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
