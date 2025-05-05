#include "Player.hpp"
#include "Config.hpp"
#include "Zone.hpp"


Player::Player(int id) : id_{id}, field_zone_{ZoneType::FIELD}, battle_zone_{ZoneType::BATTLE}
{
  Config& config = Config::getInstance();
  deck_ = config.getDeck(id);

  //setting base values
  base_health_ = config.getBaseHealth();
  base_mana_ = config.getBaseMana();

  //setting current values to base values
  current_health_ = base_health_;
  current_mana_ = base_mana_;
}


void Player::changeCurrentHealth(int amount)
{
//TODO
}

void Player::changeCurrentMana(int amount)
{
//TODO
}

void Player::refillCurrentMana()
{
//TODO
}

void Player::increaseMaxMana()
{
//TODO
}

void Player::drawCard()
{
//TODO
}

void Player::printGraveyard() const
{
//TODO
}

void Player::printHand() const
{
//TODO
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

void Player::setRole(Role role)
{
  role_ = role;
}
