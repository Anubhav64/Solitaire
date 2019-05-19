#include "stack.h"

struct spider {
  struct stack stock;
  int stock_size;
  struct stack tableau_down[10];
  struct stack tableau_up[10];
  int comp_decks;
};

// spider_init_random(s, seed) randomly initializes
// a valid beginning of a spider game by mutating *s
// The seed is for the random number generator. 
// Given a seed, the initialized game is deterministic.
// effects: modifies *s
void spider_init_random(struct spider *s, int seed);

// spider_init_custom(s) initializes the game by mutating *s
// puts 50 random cards in the stock pile
// puts the appropriate number of cards in each tableau pile
// effects: modifies *s
void spider_init_custom(struct spider *s);

// spider_has_won(s) returns true if the player has won the game
// i.e. the foundation piles contain 8 complete decks of 13 cards
// and returns false otherwise.
bool spider_has_won(const struct spider *s);


// read_card(card_read) reads at most two characters representing
// the value of a playing card.  If the first character is E,
// then it returns true.  Otherwise, it returns false and
// mutates *card_read to be the integer value of the card.
// A is read as 1, J is read as 11, 
// Q is read as 12 and K is read as 13.
// If an error occurs reading in the value, the function
// prints out an error message and terminates the program.
// effects: reads input and modifies *card_read
bool read_card(int *card_read);

// spider_print(s) prints out the statue of the game including
// the number of cards left in the stock pile
// the face-down and face-up cards in each tableau pile
// the number of complete decks in the foundation piles
// effects: produces output
void spider_print(struct spider *s);

// This function is for your own testing and debugging.  
// We will not test this function.
// spider_print_faceup(s) prints out the status of 
// the game with all the cards face up.
// all the cards in the stock pile are printed out.
// the cards in each tableau are printed out face up.
// effects: produces output
void spider_print_faceup(struct spider *s);

// spider_get_stock(s) removes 10 cards from the stock pile
// and places 1 card face-up on top of each tableau pile.
// effects: may produce output and may mutate *s
void spider_get_stock(struct spider *s);

// spider_test_find_seq() tests the find seq helper function
// using assertion testing
void spider_test_find_seq(void);

// spider_move(s, src, card, dest) attempts to move a sequence
// of cards from the src tableau pile to the dest tableau pile.
// The bottom card in the sequence must have the provided card
// number.  Otherwise, it displays an error message.
// effects: produces output and may mutate *s
void spider_move(struct spider *s, int src, int card, int dest);

