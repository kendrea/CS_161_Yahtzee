/************************************
** Program: Asn 5 Yahtzee
** Author: Kendrea Beers
** Date: November 19, 2017
** Description: Plays the game of Yahtzee
** Input: dice to keep, play again, category
** Output: a game!
*************************************/

#include <iostream>
#include <cstdlib>
#include <string>
#include <string.h>
#include <stdio.h>

using namespace std;

int ** metaScoreCard (int);
int posInt (char *);
void swap (int *, int *);
void bubbleSort (int []); 
void printDice (int []);
void rollDice (int [], bool[]);
void printChooseDice (int [], bool[]);
int readInt (int, int);
bool isInt (string);
int score (int, int[], int[]);
int scoreSimple (int[], int);
int scoreOAK (int[], int);
int scoreFullHouse (int[], int);
int scoreStraight (int[], int);
int scoreYahtzee (int[], int, int[]);
int scoreChance (int[], int);
int chooseCategory (int[], int[]);
bool playTurn (int[], bool[], int[]);
void finalScores (int, int **);
int totalScore (int[]);
void printScoreCard (int[]);

int main (int argc, char *argv[]) {
	//Seed random.
	srand(time(NULL));
	//Make a mask array so that the player can choose which dice to keep.
	bool mask[5] = {0,0,0,0,0};
	//Make a dice array.
	int dice[5];
	do {
		//Initialize the dice array.
		rollDice(dice, mask);
		//Initialize the variable isDone to indicate if gameplay should finish.
		bool isDone = false;

		//If the command-line arg for number of players makes sense, accept the input.
		int num_players;
		if (argc == 2 && (num_players = posInt(argv[1])) > 0) {
			cout << "Starting a game of Yahtzee with " << num_players << " players!\n" << endl;
		}
		//Otherwise, error and quit the program.
		else {
			cout << "Try again with a command-line argument for the number of players." << endl;
			return 0;
		}
		//Create a 2D dynamic array to hold all the score cards.
		int ** metasc = metaScoreCard(num_players);

		do {
			for (int i=0; i<num_players; i++) {
				isDone = playTurn(dice, mask, metasc[i]);
			}
		} while (!isDone);

		finalScores(num_players, metasc);

		//Fix memory leak.
		for (int i=0; i<num_players; i++) delete [] metasc[i];
		delete [] metasc;

	} while (readInt(0,1));

	return 0;
}

/*************************************************************
 ** Function: metaScoreCard
 ** Description: makes 2D dynamic array
 ** Parameters: int num_players
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ************************************************************/
int ** metaScoreCard (int num_players) {
	//Create a 2D dynamic array to hold the score cards.
	int ** metasc = new int *[num_players];
	//Each score card has 16 categories, plus a 0 index for the player ID.
	for (int i=0; i<num_players; i++) {
		metasc[i] = new int[1 + 16];
		metasc[i][0] = i + 1;

		for (int j=1; j<17; j++)
			metasc[i][j] = -1;
	}

	return metasc;
}

/*************************************************************
 ** Function: posInt
 ** Description: Converts input string to positive integer, if possible.
 ** Parameters: char str[]
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ************************************************************/
int posInt (char str[]) {
	//Get length of string.
	int len = strlen(str);
	//For each element in the C-style string dice, return 0 if it's not a digit.
	for (int i=0; i < len; i++) {
		if (str[i] > '9' || str[i] < '0')
			return 0;
	} 
	//If it's a legit digit, convert it to an int.
	return atoi(str);
}

/*************************************************************
 ** Function: swap
 ** Description: Swap for bubble sort.
 ** Parameters: int *a, int *b
 ** Pre-Conditions: none
 ** Post-Conditions: none
 ************************************************************/
void swap (int *a, int*b) {
	//Use a temporary value while swapping.
	int temp = *a;
	*a = *b;
	*b = temp;
}

/*************************************************************
 ** Function: bubbleSort
 ** Description: sorts dice
 ** Parameters: int dice[]
 ** Pre-Conditions: array of dice
 ** Post-Conditions: none
 ************************************************************/
void bubbleSort (int dice[]) {
	//For each element in the dice array...
	for (int i=0; i<4; i++) {
		//...swap adjacent elements if the first one is bigger.
		for (int j=0; j<4-i; j++) {
			if (dice[j] > dice [j+1])
				swap(&dice[j], &dice[j+1]);
		}
	}
}

/*************************************************************
 ** Function: printDice
 ** Description: prints dice
 ** Parameters: dice[]
 ** Pre-Conditions: dice array
 ** Post-Conditions: none
 ************************************************************/
void printDice (int dice[]) {
	cout << "\nDice: ";
	//For each element in the dice array...
	for (int i=0; i<5; i++) {
		//...print the value of each die with spaces inbetween.
		cout << dice[i] << " ";
	}
	cout << endl;
}

/*************************************************************
 ** Function: rollDice
 ** Description: sets non-masked dice to random; resets mask
 ** Parameters: int dice[], bool mask[]
 ** Pre-Conditions: dice array
 ** Post-Conditions: none
 ************************************************************/
void rollDice (int dice[], bool mask[]) {
	//For each element in the dice array...
	for (int i=0; i<5; i++) {
		//if the user does not want to keep the die,
		if (mask[i] == 0)
		//assign it a random integer between 1 and 6.
			dice[i] = 1 + rand() % 6; 
	}
	//Reset the mask.
	for (int i=0; i<5; i++) mask[i] = 0;
}

/*************************************************************
 ** Function: printChooseDice
 ** Description: prints dice; lets user choose dice to keep
 ** Parameters: int dice[], bool mask[]
 ** Pre-Conditions: dice array
 ** Post-Conditions: none
 ************************************************************/
void printChooseDice (int dice[], bool mask[]) {
	printDice(dice);	
	//For each element in the dice array...
	for (int i=0; i<5; i++) {
		//...ask if the user wants to keep that die.
		cout << "Do you want to keep the " << dice[i] << " in position " << i << " ? Type 1 for yes or 0 for no." << endl;
		//If the user wants to keep a die, assign 1 to its position in the mask.
		if (readInt(0, 1) == 1)
			mask[i] = 1;
	}
}

/*************************************************************
 ** Function: readInt
 ** Description: Keeps asking the user for input until they return a valid int between the min and max.
 ** Parameters: int min, int max
 ** Pre-Conditions: Inform user that they need to enter an int between the min and max.
 ** Post-Conditions: n/a
 ************************************************************/
int readInt (int min, int max) {
	int user_int; // output
	string str;

	//Keep asking the user for input until they return an int between the min and max.	
	do {
		//Keep asking the user for input until they return an integer.
		do {
			getline(cin, str);

		} while (!isInt(str));

		user_int = atoi(str.c_str());

	} while (user_int < min || user_int > max);

	return user_int;
}

/*************************************************************
 ** Function: isInt
 ** Description: Checks that a string represents an integer.
 ** Parameters: string str
 ** Pre-Conditions: Get a string from the user.
 ** Post-Conditions: Make sure that the integer is within the correct range.
 ************************************************************/
bool isInt (string str){
	
	//Ints are longer than 0 characters.
	if (str.length() == 0)
		return false;

	//Ints may begin with a minus sign or a digit.
	if (str.at(0) != '-' && !isdigit(str.at(0)))
		return false;

	//Other than the first character, ints only consist of digits.
	for (int i = 1; i < str.length(); i++) {
		if(!isdigit(str.at(i)))
			return false;
	}
	
	//If the input passes these tests, it's an int.
	return true;
}

/*************************************************************
 ** Function: score
 ** Description: scores dice in given category for given player
 ** Parameters: int dice[], int category, int score_card[]
 ** Pre-Conditions: dice array
 ** Post-Conditions: none
 ************************************************************/
int score (int category, int dice[], int score_card[]) {
	bubbleSort(dice);
	if (category >= 1 && category <= 6)
		return scoreSimple(dice, category);
	if (category == 7 || category == 8)
		return scoreOAK(dice, category);
	if (category == 9)
		return scoreFullHouse(dice, category);
	if (category == 10 || category == 11)
		return scoreStraight(dice, category);
	if (category == 12)
		return scoreChance(dice, category);
	if (category >= 13)
		return scoreYahtzee(dice, category, score_card);
	return 0;
}	

/*************************************************************
 ** Function: scoreSimple
 ** Description: scores simple category
 ** Parameters: int dice[], int category
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: none
 ************************************************************/
int scoreSimple (int dice[], int category) {
	int score = 0;
	for (int i=0; i<5; i++) 
		if (dice[i] == category)
			score = score + category; 
	return score;
}

/*************************************************************
 ** Function: scoreChance
 ** Description: scores chance category
 ** Parameters: int dice[], int category
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: none
 ************************************************************/
int scoreChance (int dice[], int category) {
	int score = 0;
	for (int i=0; i<5; i++)
		score = score + dice[i];
	return score;
}

/*************************************************************
 ** Function: scoreYahtzee
 ** Description: scores Yahtzee category
 ** Parameters: int dice[], int category, int score_card[]
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: score carefully
 ************************************************************/
int scoreYahtzee (int dice[], int category, int score_card[]) {
	//If not all the dice are identical, it's not a Yahtzee.
	for (int i=1; i<5; i++) {
		if (dice[i] != dice[i-1]) {
			return 0;
		}
	}
	//If it's an original Yahtzee, return 50.
	if (category == 13) {
		cout << "returning 50";
		return 50;
	}
	//If it's a bonus Yahtzee, but there's no score in the original category yet or if it's 0, return 0.
	if (score_card[13] <= 0)
		return 0;
	//Otherwise, it's a legit bonus Yahtzee.
	return 100;
}

/*************************************************************
 ** Function: scoreOAK
 ** Description: scores OAK category
 ** Parameters: int dice[], int category
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: none
 ************************************************************/
int scoreOAK (int dice[], int category) {
	int count = 1;
	//The number of of-a-kind elements is the category number minus 4.
	int oak = category - 4;
	//For each pair of elements in the array...
	for (int i=1; i<5; i++) {
		//If they're the same, increment count.
		if (dice[i] == dice[i-1]) {
			++count;
			//Once count reaches 3 or 4, the score is the total of all dice.
			if (count == oak)
				return scoreChance(dice, category);
		} else
			count = 1;
	}			
	return 0; 
}

/*************************************************************
 ** Function: scoreFullHouse
 ** Description: scores Full House category
 ** Parameters: int dice[], int category
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: none
 ************************************************************/
int scoreFullHouse (int dice[], int category) {
	if (((dice[0] == dice[1]) && (dice[3] == dice[4])) && ((dice[2] == dice[1]) || (dice[2] == dice[3])))
		return 25;
	return 0;
}

/*************************************************************
 ** Function: scoreStraight
 ** Description: scores straight category
 ** Parameters: int dice[], int category
 ** Pre-Conditions: dice array, category from user
 ** Post-Conditions: none
 ************************************************************/
int scoreStraight (int dice[], int category) {
	int count = 1;
	//The number of dice in a straight is the category number minus 6.
	int straight = category - 6;
	//For each pair of elements in the array...
	for (int i=1; i<5; i++) {
		//If they're one apart, increment count.
		if (dice[i]-1 == dice[i-1]) {
			++count;
			//If count reaches 4 or 5, return the appropriate score.
			if (count == straight) {
				if (straight == 4)
					return 30;
				if (straight == 5)
					return 40;
			}
		} else if (dice[i] != dice[i-1])
			count = 1;
	}
	return 0;
}

/*************************************************************
 ** Function: chooseCategory
 ** Description: has user choose category
 ** Parameters: int dice[], int score_card
 ** Pre-Conditions: dice array
 ** Post-Conditions: none
 ************************************************************/
int chooseCategory (int score_card[], int dice[]) {
	//Get the category the user wants to score in.
	cout << "Which category do you want to score in?\n"
	<< "1, 2, 3, 4, 5, 6: Total of That Number\n"
	<< "7: Three of a Kind\n"
	<< "8: Four of a Kind\n"
	<< "9: Full House\n"
	<< "10: Small Straight\n"
	<< "11: Large Straight\n"
	<< "12: Chance\n"
	<< "13: Yahtzee\n"
	<< "14, 15, 16: Bonus Yahtzee\n";

	//Make a boolean to hold the conditional.
	bool good = true;
	//Keep asking for a category while the input is not good.
	int category;
	do {
		//Trigger the error message only after the first time, because the input might be good the first time.
		if (!good)
			cout << "You can't score there. Pick a different category!\n";
		
		//Get a category int between 1 and 16. 
		category = readInt(1, 16);
		cout << "!!2";
		printDice(dice);
		//A category number is good if...
		good = 
			//the category hasn't been used yet and
			(score_card[category] == -1) &&
			//the player isn't trying to get a bonus Yahtzee if they haven't gotten a regular Yahtzee first or if they'd get a zero in the bonus category.
			//A zero causes harm because a player who gets one Yahtzee suddenly gets the advantage of three more slots in which to dispose of zeros.
			!((category >= 14) && !scoreYahtzee(dice, category, score_card));
	} while (!good);

	return category;
}

/*************************************************************
 ** Function: playTurn
 ** Description: workhorse that repeats for each player's turn
 ** Parameters: int dice[], bool mask[], int score_card[]
 ** Pre-Conditions: rest of game set up
 ** Post-Conditions: none
 ************************************************************/
bool playTurn (int dice[], bool mask[], int score_card[]) {
	system("clear");
	cout << "Player " << score_card[0] << "\'s Turn!\n";
	printScoreCard(score_card);
	//Roll the dice three times and have the user choose dice to keep twice.
	rollDice(dice, mask);
	printChooseDice(dice, mask); 
	rollDice(dice, mask);
	printChooseDice(dice, mask);
	rollDice(dice, mask);
	printDice(dice); cout << endl;
	//After the final roll, have the player choose a valid category.
	int category = chooseCategory(score_card, dice);
	//Put their score in their score card.
	score_card[category] = score(category, dice, score_card); 
	printScoreCard(score_card);
	//Reset the mask for the next player.
	for (int i=0; i<5; i++) mask[i] = 0;
	//Assume that the score card is full.
	int isDone = true;
	//If any of the categories in the score card (not counting bonus Yahtzees) aren't full, 
	for (int i=1; i<14; i++) {
		if (score_card[i] == -1)
			isDone = false;
	}

	return isDone;
}

/*************************************************************
 ** Function: finalScores
 ** Description: lists players' final scores and wraps up game
 ** Parameters: int num_players, int ** metasc
 ** Pre-Conditions: finish game
 ** Post-Conditions: none
 ************************************************************/
void finalScores (int num_players, int ** metasc) {
	for (int i=0; i<num_players; i++)
		cout << "Player " << i << "\'s Final Score: " << totalScore(metasc[i]) << endl;
	cout << "Would you like to play again? Type 1 for yes or 0 for no." << endl;
}
	
/*************************************************************
 ** Function: totalScore
 ** Description: tallies score for one player
 ** Parameters: int score_card[]
 ** Pre-Conditions: finish game
 ** Post-Conditions: none
 ************************************************************/
int totalScore (int score_card[]) {
	int total_score = 0;
	int total_upper = 0;
	//Sum the scores in the upper six categories.
	for (int i=0; i<7; i++) 
		total_upper += score_card[i];
	//If they add to at least 63, the player gets a 35-point bonus.
	if (total_upper >= 63)
		total_score = 35;
	for (int j=1; j<17; j++)
		total_score += score_card[j];
	return total_score;
}

/*************************************************************
 ** Function: printScoreCard
 ** Description: prints score card
 ** Parameters: int score_card[]
 ** Pre-Conditions: have score card set up
 ** Post-Conditions: none
 ************************************************************/
void printScoreCard (int score_card[]) {
	cout << "Score Card: ";
	for (int i=1; i<17; i++) 
		cout << score_card[i] << " ";
	cout << endl;
}