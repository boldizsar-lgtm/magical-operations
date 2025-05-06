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
  Card* card = deck_->getPileEnd();

  if (card == nullptr)
  {
    return false;
    std::cout << "Card is nullptr in drawCard()";
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
  hand_.printHand();
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


int Player::getDeckSize() const
{
  return deck_->getSize();
}


bool Player::getDoneStatus() const
{
  return done_;
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
