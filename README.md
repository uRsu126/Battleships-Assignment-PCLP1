//Urse Andrei 312CB

BATTLESHIPS

STEP I: Reading and Placing the Ships

I defined a ship struct that stores the data for each ship (type, length, origin and end coordinates, as well as orientation) to allow faster access later during the attack phase.

For this step, I dynamically allocated three matrices:

    One to store the lengths of the ships;

    One to store the index corresponding to the ship's position in the ships array (for faster data lookups during attacks);

    One to record the attacks carried out later on.

I also allocated two arrays: one storing each ship along with its details, and the other tracking the current ship count for each player.

Finally, the players' boards—namely the matrices containing the ship lengths—are displayed.

STEP II: Executing the Attacks

During the attack phase, I check that the targeted cell is valid. For a successful hit, the targeted cell in both the length matrix and the index matrix must be non-zero, while in the attack tracking matrix it must be zero.

Meanwhile, for the statistics required in Task 3, I record the total number of ships, total attacks, and successful hits.

As ships are destroyed, the array tracking each player's remaining ship count is decremented until one value reaches zero; the player whose count remains greater than zero wins.

Once each match concludes, the memory allocated for the five aforementioned data structures is freed.

STEP III: Statistics

After all matches are finished, I compute the requested statistics based on the parsed message.

I dynamically allocate two strings: one for the key and the other for the message itself, extracting the required details into separate variables.

Because there are eight distinct calculation scenarios in total, I designed one function to compute the statistics for an individual match and another for all matches combined. I utilized functions from the math.h library to obtain exact results.

For single-game statistics, I calculated either the number of additional hits a player would have needed to achieve the target accuracy, or the number of shots they could have missed while still maintaining an accuracy above the desired threshold.

For aggregate statistics across all games, I used a Greedy approach: for Case I, I selected the games yielding the greatest gain, while for Case III, I selected those resulting in the greatest loss.
