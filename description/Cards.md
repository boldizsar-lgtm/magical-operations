# Card Overview

## Card Types

There are two types of cards in the game:
- Creature cards
- Spell cards

All cards that may be used in the game, as well as their features, are listed in this document.

## Mana Cost

All cards have a mana cost. This is the amount of mana a player needs to pay to play this card. Some cards have a variable mana cost, depending on how they are played (denoted as x in the below tables and printed as `XX` in the output).

## Creatures

Creature cards are used to attack the other player and defend from their attacks during battles.

In the following description, **board** refers to the combination of both players' Field and Battle Zones. It does not include hand cards, draw piles, or graveyards.

<details>
<summary><h4>Base Health & Current Health</h4></summary>

Creature cards have a current health and a base health. Base health is the number of health points a creature has when being played, before taking damage and without any additional effects. Base health does not change and is the same for all creatures of the same type. Current health is the number of health points a creature currently has. It may be higher, equal to, or lower than base health.

When a creature enters a player's hand (e.g. through drawing a card, or through a spell) or the board (e.g. through a `creature` command, or through a spell), its current health is set to its base health.

</details>

<details>
<summary><h4>Base Attack & Current Attack</h4></summary>

Creature cards have a current attack and a base attack. Base attack is the attack strength a creature has when being played, without any additional effects. Base attack does not change and is the same for all creatures of the same type. Current attack is the attack strength a creature currently has. It may be higher or equal to base attack.

When a creature enters a player's hand (e.g. through drawing a card, or through a spell) or the board (e.g. through a `creature` command, or through a spell), its current attack is set to its base attack.

</details>

<details>
<summary><h4>Damage & Excess Damage</h4></summary>

**Damage** refers to the amount of current health a creature loses from e.g. an attack (see [Milestone 2](Milestone_2.md) > Battle Phase) or a spell. 
**Excess damage** refers to the amount of damage that exceeds the current health of the creature.

A creature is said to **take damage** when its current health is reduced. A creature is said to **deal damage** when it causes another creature or a player to lose health. 

**Example 1**: Creature A (current attack 1) attacks creature B (current health 2). The damage A **deals** is 1. The damage B **takes** is 1. The **excess damage** is 0.

**Example 2**: Creature C (current attack 5) attacks creature D (current health 2). The damage C **deals** is 5. The damage D **takes** is 2. The **excess damage** is 3. 

</details>

<details>
<summary><h4>Base Traits & Current Traits</h4></summary>

In addition to mana cost, attack, and health, most creatures also have so-called **traits**. Traits grant the creature additional effects or abilities. In the output, they are always displayed in alphabetical order, either by their first letter in uppercase (see [Milestone 1](Milestone_1.md) > Game Board Printing > Card Printing) or by their name (see [Milestone 2](Milestone_2.md) > Command: info).

When and how traits should be handled is described in [Milestone 2](Milestone_2.md) in the sections where it is relevant.

| Letter | Name         | Effect                                                                                                                                                                                                                                                                                                     |
|--------|--------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| B      | Brutal       | If this creature reduces the current health of an opposing creature to zero, any excess damage is dealt to the opposing creature's owner.                                                                                                                                                                  |
| C      | Challenger   | When this creature is moved to a Battle Zone slot, the creature in the opponent's Field Zone slot in the same lane is moved to the Battle Zone slot. This only happens if the opponent has a creature in the corresponding Field Zone slot, and the opponent's corresponding Battle Zone slot is empty.    |
| F      | First Strike | When fighting another creature, this creature attacks first if it is the only one with First Strike. Two fighting creatures with First Strike attack in the standard attack order.                                                                                                                         |
| H      | Haste        | This creature can be moved to the Battle Zone in the same round it has entered the board.                                                                                                                                                                                                                  |
| L      | Lifesteal    | If this creature deals damage to another creature, its current health increases by 2.                                                                                                                                                                                                                      |
| P      | Poisoned     | If this creature is on the board, it loses 1 current health at the end of its owner's turn.                                                                                                                                                                                                                |
| R      | Regenerate   | Whenever its owner ends their turn in a round where the round counter is odd, if this creature is on the board and has less current health than base health, its current health is set to its base health.                                                                                                 |
| T      | Temporary    | If this creature is on the board, it is moved to the graveyard after all fights have been resolved in the Battle Phase of the current round.                                                                                                                                                               |
| U      | Undying      | If this creature is in the graveyard, it is placed in its owner's first empty Field Zone slot at the end of the Battle Phase of the current round. The creature then loses the Undying trait. If no empty Field Zone slot is available, the creature remains in the graveyard and does not lose the trait. |
| V      | Venomous     | If this creature deals damage to another creature, the opponent creature gains the Poisoned trait.                                                                                                                                                                                                         |

Base traits is the list of traits that a creature has without any additional effects added by spells (see Base Traits in the list of creature cards below). The list of base traits does not change and is the same for all creatures of the same type. Current traits is the list of traits a creature currently has, after optionally being affected by spells or other creatures. When a creature enters a player's hand (e.g. through drawing a card, or through a spell) or the board (e.g. through a `creature` command, or through a spell), its current traits are set to its base traits.

If a trait contradicts the general rules, the trait takes precedence. For example, the trait Challenger can move a creature into a Battle Zone slot in the same round it has entered the board, without causing an `E_CREATURE_CANNOT_BATTLE` error.

</details>

<details>
<summary><h4>List of Creature Cards</h4></summary>

This is a list of all creature cards.

| Mana Cost | ID     | Name             | Base Traits           | Base Attack | Base Health |
|-----------|--------|------------------|-----------------------|-------------|-------------|
| 1         | AGRAT  | Angry Rat        | Haste                 | 1           | 1           |
| 1         | CADET  | Cadet            | -                     | 2           | 1           |
| 1         | FARMR  | Farmer           | Haste                 | 1           | 2           |
| 1         | SQIRL  | Squirrel Soldier | First Strike          | 1           | 1           |
| 2         | FSHLD  | Floating Shield  | -                     | 0           | 8           |
| 2         | NITMR  | Nightmare        | Haste, Temporary      | 5           | 1           |
| 2         | SOLDR  | Soldier          | -                     | 3           | 4           |
| 2         | SNAKE  | Snake            | Venomous              | 2           | 1           |
| 2         | HWOLF  | Hungry Wolf      | Brutal                | 3           | 2           |
| 2         | ZOMBI  | Zombie           | Undying               | 2           | 2           |
| 3         | ASASN  | Assassin         | First Strike          | 5           | 2           | 
| 3         | CVLRY  | Cavalry          | Haste                 | 4           | 4           |
| 3         | GLDTR  | Gladiator        | Challenger            | 5           | 3           |
| 3         | KNGHT  | Knight           | Haste                 | 4           | 6           |
| 3         | VAMPS  | Vampire Soldier  | Lifesteal             | 4           | 3           |
| 4         | ALCHM  | Alchemist        | Venomous              | 4           | 6           |
| 4         | TUTOR  | Evil Tutor       | Challenger, Lifesteal | 5           | 4           |
| 4         | TURTL  | Giant Turtle     | -                     | 3           | 11          |
| 4         | NINJA  | Ninja            | First Strike, Haste   | 6           | 4           |
| 5         | GUARD  | Eternal Guardian | Haste, Undying        | 5           | 5           |
| 5         | RAPTR  | Raptor           | Brutal, First Strike  | 7           | 4           |
| 5         | WRLCK  | Warlock          | Lifesteal, Venomous   | 4           | 7           |
| 6         | GOLEM  | Golem            | Regenerate            | 5           | 12          |
| 6         | HYDRA  | Hydra            | Regenerate, Undying   | 7           | 6           |
| 6         | KINGV  | King V           | Challenger, Haste     | 6           | 11          |
| 7         | LLICH  | Likeable Lich    | Lifesteal, Undying    | 9           | 6           |
| 7         | T_REX  | T-Rex            | Brutal                | 13          | 9           |
| 7         | VAMPL  | Vampire Lord     | Challenger, Lifesteal | 10          | 7           |
| 8         | ANGEL  | Angel            | Haste                 | 9           | 14          |
| 8         | DRAGN  | Dragon           | Brutal, Challenger    | 13          | 10          |
| 8         | SLAYR  | Slayer           | First Strike, Haste   | 15          | 6           |
| 9         | D_GOD  | Demi-God         | Regenerate, Undying   | 15          | 15          |
| 9         | DEVIL  | Devil            | Brutal, First Strike  | 16          | 7           |

</details>


## Spells

Spell cards cause instantaneous effects. Afterward, they are destroyed. Spell cards are never moved to the graveyard.

There are three different subtypes of spells.

<details>
<summary><h4>List of General Spell Cards</h4></summary>

General Spells cause a general effect.

| Mana Cost | ID     | Name        | Effect                                                                                                                          |
|-----------|--------|-------------|---------------------------------------------------------------------------------------------------------------------------------|
| 3         | BTLCY  | Battle Cry  | All of the player's own creatures on the board gain the Haste and Temporary traits, and their current attack is increased by 3. |
| 4         | METOR  | Meteor      | All creatures on the board take 3 damage.                                                                                       |
| 5         | FIRBL  | Fireball    | All enemy creatures on the board take 2 damage.                                                                                 |

>**Attention**: For the Meteor and Fireball spells, damage is dealt first in the Battle Zone slots, and then the Field Zone slots, both by increasing ID. This defines the order in which creatures are moved to the graveyard, if multiple creatures die due to the spell.

</details>

<details>
<summary><h4>List of Target Spell Cards</h4></summary>

Target Spells always target a single creature on the board.

| Mana Cost | ID     | Name        | Effect                                                                                                                                                                                                                                                                                                                                                                          |
|-----------|--------|-------------|---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| x         | CLONE  | Clone       | A newly created creature of the same type as the target creature enters the board in the player's first empty Field Zone slot. The new creature's current attack, current health and current traits are set to the target creature's values, and it gains the Haste and Temporary traits. This spell's mana cost is equal to half the target creature's mana cost (rounded up). |
| x         | CURSE  | Death Curse | The target creature gains the Temporary trait. This spell's mana cost is equal to the target creature's mana cost plus 1.                                                                                                                                                                                                                                                       |
| 1         | SHOCK  | Shock       | The target creature's current health is reduced by 1.                                                                                                                                                                                                                                                                                                                           |
| 2         | MOBLZ  | Mobilize    | The target creature gains the Haste trait and its current attack is increased by 1.                                                                                                                                                                                                                                                                                             |
| 2         | RRUSH  | Rapid Rush  | The target creature gains the First Strike and Temporary traits and its current attack is increased by 2.                                                                                                                                                                                                                                                                       |
| 2         | SHILD  | Shield      | The target creature's current health is increased by 2.                                                                                                                                                                                                                                                                                                                         |
| 3         | AMPUT  | Amputate    | The target creature's first trait in alphabetical order is removed.                                                                                                                                                                                                                                                                                                             |
| 3         | FINAL  | Final Act   | The target creature gains the Brutal, Haste and Temporary traits, and its current attack is increased by 3.                                                                                                                                                                                                                                                                     |
| 3         | LYLTY  | Loyalty     | The target creature gains the Haste trait and its current health is increased by 1.                                                                                                                                                                                                                                                                                             |
| 4         | ZMBFY  | Zombify     | The target creature gains the Venomous and Undying traits.                                                                                                                                                                                                                                                                                                                      |
| 5         | BLOOD  | Bloodlust   | The target creature gains the Brutal and Lifesteal traits, but its current health is reduced to half its previous value (rounded up).                                                                                                                                                                                                                                           |

> **Attention**: If a player that has no empty Field Zone slot uses Clone, the spell card is still paid for and destroyed, but no creature is created.

</details>

<details>
<summary><h4>List of Graveyard Spell Cards</h4></summary>

Graveyard Spells interact with the player's graveyard.


| Mana Cost | ID      | Name           | Effect                                                                                                                                                                                                                        |
|-----------|---------|----------------|-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| x         | MEMRY   | Heroic Memory  | This spell moves a creature from the player's graveyard into their first empty Field Zone slot. It gains the Haste and Temporary traits. This spell's mana cost is equal to half the moved creature's mana cost (rounded up). |
| 2         | REVIV   | Revive         | This spell moves a creature from the player's graveyard into their hand.                                                                                                                                                      |

> **Attention**: If a player that has no empty Field Zone slot uses Heroic Memory, the spell card is still paid for and destroyed, but the creature remains in the graveyard and does not gain any traits.

</details>

