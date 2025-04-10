# Errors and Error Ranking

This is a list of all errors that must be handled during execution. All of these errors are also explained in the text where they are relevant; this list is just meant as an additional helpful resource.

## Terminating Errors

After one of these errors is detected, the program should print the error message, free all allocated resources and terminate with the specified return value. If multiple errors occur at the same time, only the one listed highest in the table should be used.

| Return Value  | Description                                                                               | Error Message                           |
|---------------|-------------------------------------------------------------------------------------------|-----------------------------------------|
| 0             | Game finished or was ended using a command.                                               | no error message                        |
| 1             | Memory allocation error.                                                                  | `[ERROR] Not enough memory!\n`          |
| 2             | Wrong number of command line parameters.                                                  | `[ERROR] Wrong number of parameters.\n` |
| 3             | Config file could not be opened for reading, or does not start with correct magic number. | `[ERROR] Invalid file (<FILE_PATH>).\n` |

The variable `<FILE_PATH>` should be replaced by the path passed in as an argument to the program (e.g. `configs/message_config.txt`). If both files encounter an error, the message should be printed only for the game config file.

## Command Errors

These errors must be handled when validating commands entered by the user. For each invalid command only print one error message. If several errors occur at the same time, only the one that is ranked earliest (lowest Rank number) should be printed. 

| Rank | Error Description                                                                                                    | Message Key                   | Relevant for Command                       |
|------|----------------------------------------------------------------------------------------------------------------------|-------------------------------|--------------------------------------------|
| 1    | the entered command is not known                                                                                     | `E_UNKNOWN_COMMAND`           | all user input                             |
| 2    | there are more or less parameters than the given command should have                                                 | `E_INVALID_PARAM_COUNT`       | all commands except spell                  |
| 3    | the `spell` command has zero parameters                                                                              | `E_MISSING_CARD`              | spell (all subtypes)                       |
| 4    | a given `<CARD_ID>` parameter does not match any of the card IDs in the card overview                                | `E_INVALID_CARD`              | info <br>creature <br>spell (all subtypes) |
| 5    | a given `<SLOT>` parameter does not match the slot regex pattern `O?(F\|B)[1-7]`                                     | `E_INVALID_SLOT`              | battle <br>creature                        |
| 6    | the player tries to use `redraw` after it has been disabled                                                          | `E_REDRAW_DISABLED`           | redraw                                     |
| 7    | the player tries to use `redraw` with less than two cards in their hand cards                                        | `E_REDRAW_NOT_ENOUGH_CARDS`   | redraw                                     |
| 8    | `<HAND_CARD_ID>` is not present in the player's hand cards                                                           | `E_NOT_IN_HAND`               | creature <br>spell (all subtypes)          |
| 9    | `<HAND_CARD_ID>` is not a creature                                                                                   | `E_NOT_CREATURE`              | creature                                   |
| 10   | `<HAND_CARD_ID>` is not a spell                                                                                      | `E_NOT_SPELL`                 | spell (all subtypes)                       |
| 11   | the `spell` command does not have exactly the amount of parameters needed for `<HAND_CARD_ID>`'s subtype             | `E_INVALID_PARAM_COUNT_SPELL` | spell (all subtypes)                       |
| 12   | `<FIELD_SLOT>` is not in the player's own field zone                                                                 | `E_NOT_IN_FIELD`              | battle <br>creature                        |
| 13   | `<FIELD_SLOT>` does not contain a creature                                                                           | `E_FIELD_EMPTY`               | battle                                     |
| 14   | the creature in `<FIELD_SLOT>` entered the board in the same round and does not have the Haste trait                 | `E_CREATURE_CANNOT_BATTLE`    | battle                                     |
| 15   | `<BATTLE_SLOT>` is not in the player's own battle zone                                                               | `E_NOT_IN_BATTLE`             | battle                                     |
| 16   | `<BATTLE_SLOT>` already contains a creature                                                                          | `E_BATTLE_OCCUPIED`           | battle                                     |
| 17   | `<FIELD_SLOT>` already contains a creature                                                                           | `E_FIELD_OCCUPIED`            | creature                                   |
| 18   | `<TARGET_SLOT>` does not match the slot regex pattern `O?(F\|B)[1-7]`                                                | `E_INVALID_SLOT_SPELL`        | spell (Target Spells)                      |
| 19   | `<TARGET_SLOT>` does not contain a creature                                                                          | `E_TARGET_EMPTY`              | spell (Target Spells)                      |
| 20   | `<GRAVEYARD_CARD_ID>` does not match any card ID in the player's graveyard                                           | `E_NOT_IN_GRAVEYARD`          | spell (Graveyard Spells)                   |
| 21   | not enough mana to play this card                                                                                    | `E_NOT_ENOUGH_MANA`           | creature <br>spell (all subtypes)          |
