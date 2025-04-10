# Assignment 2 - Magical OOPerations

## Introduction
In Assignment 2 (A2) of the OOP1 practicals (KU) you will implement a strategy card game based on a multitude of different card games like _Magic the Gathering_, _Legends of Runeterra_, or _Hearthstone_. Please take care to follow the assignment description exactly, even if you know the original games, because Magical OOPerations uses different rules than the aforementioned games.

Assignment 2 consists of three parts. We recommend completing the parts in order and finishing one before starting the next, because each part builds on the previous one.
- [Design Document - Description of Approach](description/Design_Document.md)
- [Milestone 1 - Structure, Config Parsing and Printing](description/Milestone_1.md)
- [Milestone 2 - Game and Card Logic](description/Milestone_2.md)

## Learning Goals
- Classes and Objects
- Inheritance
- `const`
- Strings and Streams

## Specifications

Some general specifications must be followed for every assignment in this course. A list of these specifications can be found here: [Assessment of Assignments - Deductions](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-work-in-progress-00d2624846f4491391b389e6a48fa180?pvs=4#0af0c923deff4c4bb8740f5698aca451)

Please follow these specifications to avoid point deductions.

## Grading

Assignment 2 is worth **60 points**.

**Attention:** To receive a positive grade on the OOP1 KU your program must fulfill at least the following **minimum requirements** (among other conditions):
- At least **50 % of all test cases** for A2 have to be passed.
- Any test cases marked with **MINIMUM REQUIREMENT** in the test report have to be passed. Only public test cases will be part of the minimum requirements.

 If your program does not fulfill the minimum requirements by the A2 deadline, you will not be allowed to do A3, and will need to correct your A2 until the A3 deadline. Otherwise, after the A2 deadline, you can decide as a team whether you want to move on to A3 (A2 is graded immediately), or correct your A2 until the A3 deadline (A2 is graded after A3 deadline, no points for A3). To implement A3 without trouble, **we recommend to pass at least 70 % of all test cases for A2** by the A2 deadline, because A3 builds on A2. **Your A2 will only be graded once**. An overview image of the procedure can be found [here](https://www.notion.so/coding-tugraz/Guide-for-the-Practicals-OOP1-1aa30869373d80209e17cb2809cfa5d4?pvs=4#1ad30869373d803687d6de1698e5426c).

Information on how your assignment submission will be assessed can be found here: [Assessment of Assignments](https://coding-tugraz.notion.site/Assessment-of-Assignments-How-do-you-get-your-points-00d2624846f4491391b389e6a48fa180)

Here you can find an overview of how your total grade for the KU will be calculated: [Procedure and Grading](https://coding-tugraz.notion.site/Practicals-structure-grading-and-plagiarism-76127221ed43451abc7ffba7852595e3)

To get points on A2, you will need to attend an **assignment interview**.

### Points for Effective C++

Some of the points for this assignment can only be achieved if you use certain C++ concepts in your implementation.
Information about this can be found here: [Effective C++ Points](https://www.notion.so/coding-tugraz/Assessment-of-Assignments-How-do-you-get-your-points-00d2624846f4491391b389e6a48fa180?pvs=4#a94d582d5e6740859ecd613857bd0a36)

## Submission

### Design Document (**mandatory**)
- Push to your team's repository **on the `main` branch**
- Deliverables: a pdf of your [Design Document](description/Design_Document.md)
- **Deadline: 25.04.2025, 23:59 (Austrian time)**
- There will be discussion rounds with your tutor and other teams to discuss your design. Participating in a discussion round is **mandatory** to receive a positive grade. **Your team must sign up for a discussion round via the Teachcenter before the deadline on 25.04.2025.**

### Program (**mandatory**)

- Push to your team's repository **on a branch called `submission`**
- Deliverables: all .cpp and .hpp files needed to run your program
- **Deadline: 21.05.2025, 23:59 (Austrian time)**
- **Assignment 2 is team work!** It is submitted in teams of three people. Try to keep the division of labor as even as possible. If a team member does not contribute sufficiently to the project, they may get deductions. Every team member should make their own commits so that we are able to see who implemented what. In the artifacts downloaded from Gitlab, you can find a gitreport that shows your current statistics. 

## Assignment Description

### General Notes
The notes given in this section are important for the entire assignment description.

- `\n` shouldn't be printed as two separate characters, instead they represent the newline character.
- Note the leading and trailing spaces in the text fields. You can make them visible by selecting the text field.
- Words in uppercase letters in angle brackets (like `<CARD_ID>`) should be replaced by calculated or given data. They are *not* part of the output itself!
- Additional **whitespaces** should be ignored
  - before and after user input
  - between command and parameters
  - between parameters
- User inputs are **case-insensitive**. This means that `quit`, `Quit` and `QUIT` are all valid inputs.
- If `quit` or `EOF` is entered (End of File, not the string "EOF"), the program should terminate with the return value 0. This should be possible any time the program waits for user input.
- The program needs to be able to handle arbitrary input in any form (e.g. strings when integers are expected, input of any length).

### The Game

Magical OOPerations is a two player strategy card game, similar to _Magic the Gathering_, _Legends of Runeterra_ or _Hearthstone_. In Magical OOPerations, each player has health points and mana. Mana is the currency used to play cards, like creatures and spells. The goal of the game is to reduce the other player's health points to zero by attacking them using creatures. Alternatively, a game is also won if the opponent cannot draw, because they have no cards left on their draw pile.

There are two roles in the game (Attacker and Defender), which are filled depending on the round by Player 1 or Player 2. To protect themselves from attacks a Defender can set creatures to battle against the Attacker's creatures. You can find a detailed description of the game in [Milestone 1](description/Milestone_1.md) and  [Milestone 2](description/Milestone_2.md).

> **Note:** Please do not expect the game to be amazingly balanced, as this is a programming course and not a game design course. Nevertheless, we did our best to make it fun to play!
