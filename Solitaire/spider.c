#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "spider.h"

static const int TABLEAU_SIZE = 10;
static const int DECK_SIZE = 13;
static const int NUM_DECK = 8;

// spider_init_stacks(s) initializes all
// the stacks in the spider struct. 
// requires: s is a valid pointer.
// effects: mutates *s
static void spider_init_stacks(struct spider *s) {
  stack_init(&s->stock);
  for (int i = 0; i < TABLEAU_SIZE; ++i) {
    stack_init(&s->tableau_up[i]);
    stack_init(&s->tableau_down[i]);
  }
}

// get_random_card(card_count) repeatedly generates
// a random card value from 1 to 13 until 
// the number of cards left for that value is 
// positive according to card_count.
// requires: card_count is a valid pointer.
//           length of card_count is 13.
static int get_random_card(int card_count[]) {
  assert(card_count);

  int card_random = rand() % DECK_SIZE + 1;
  while (card_count[card_random - 1] == 0) {
    card_random = rand() % DECK_SIZE + 1;
  }
  return card_random;
}

// init_tableau(s,index,num_down,num_up,card_count)
// initialize the index-th tableau randomly by 
// putting num_down cards in the face-down pile and
// num_up cards in the face-up pile.
// requires: s and card_count are valid pointers.
//           0 <= index <= 9
//           4 <= num_down <= 5
//           num_up == 1
//           length of card_count is 13.
// effects: mutates *s
static void init_tableau(struct spider *s, 
                         int index, 
                         int num_down, 
                         int num_up, 
                         int card_count[]) {
  assert(s);
  assert(card_count);
  assert(0 <= index);
  assert(index <= 9);
  assert(num_down == 4 || num_down == 5);
  assert(num_up == 1);

  for (int j = 0; j <= num_down - 1; ++j) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->tableau_down[index]);
    --card_count[card_random - 1];
  }
  for (int j = 0; j <= num_up - 1; ++j) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->tableau_up[index]);
    --card_count[card_random - 1];
  }
}

void spider_init_random(struct spider *s, 
                        int seed) {
  assert(s);

  const int STOCK_SIZE_INITIAL = 50;

  int card_count[13];
  for (int i = 0; i < DECK_SIZE; ++i) {
    card_count[i] = 8;
  }

  srand(seed);

  // initialize the stacks
  spider_init_stacks(s); 

  // init stock pile
  s->stock_size = STOCK_SIZE_INITIAL;
  for (int i = 0; i < STOCK_SIZE_INITIAL; ++i) {
    int card_random = get_random_card(card_count);
    stack_push(card_random, &s->stock);
    --card_count[card_random - 1];
  }

  // init the tableaus
  for (int i = 0; i < 4; ++i) {
    init_tableau(s, i, 5, 1, card_count);
  }
  for (int i = 4; i <= TABLEAU_SIZE - 1; ++i) {
    init_tableau(s, i, 4, 1, card_count);
  }

  s->comp_decks = 0;
}

void spider_init_custom(struct spider *s) {
  assert(s);

  int card_read = -1;

  // initialize the stacks
  spider_init_stacks(s); 

  // init stock pile
  s->stock_size = 0;
  int r = read_card(&card_read);
  int count = 0;
  while (!r) {
    ++count;
    ++(s->stock_size);
    stack_push(card_read, &s->stock);
    r = read_card(&card_read);
  }

  // init the tableaus
  for (int i = 0; i <= TABLEAU_SIZE - 1; ++i) {

    int r = read_card(&card_read);
    while (!r) {
      stack_push(card_read, &s->tableau_down[i]);
      r = read_card(&card_read);
    }

    r = read_card(&card_read);
    while (!r) {
      stack_push(card_read, &s->tableau_up[i]);
      r = read_card(&card_read);
    }

  }

  scanf("%d", &s->comp_decks);
}

bool spider_has_won(const struct spider *s) {
  assert(s);

  return (s->comp_decks == NUM_DECK);
}

//=========================================================
// The function implemnetations above are provided.  
// Do not modify them.
//=========================================================

//=========================================================
// Implement the functions below.
//=========================================================

bool read_card(int *card_read) {
  // Replace the code below with your implementation.
  assert(card_read);
  
  char c1 = 0;
  char c2 = 0;
  scanf(" %c", &c1);
  if(c1 >= '2' && c1 <= '9') {
    *card_read = c1 - '0';
  } else if (c1 == 'A') {
    *card_read = 1;
  } else if (c1 == 'J') {
    *card_read = 11;
  } else if (c1 == 'Q') {
    *card_read = 12;
  } else if (c1 == 'K') {
    *card_read = 13;
  } else if (c1 == 'E') {
    return true;
  } else if (c1 == '1') {
    scanf("%c", &c2);
    if (c2 == '0') {
      *card_read = 10;
    } else {
      printf("Error reading in a card.\n");
      exit(EXIT_FAILURE);
    }
  } else {
    printf("Error reading in a card.\n");
    exit(EXIT_FAILURE);
  }
  return false;
}

void spider_print(struct spider *s) {
  assert(s);
  printf("The number of cards left in the stock pile: %d\n",
         s->stock_size);
  printf("The number of completed decks: %d\n", s->comp_decks);
  
  struct stack *up;
  struct stack *down;
  struct stack copy_up;
  struct stack *copy = &copy_up;
  stack_init(copy);
  int top_card = 0;
  
  
  for(int i = 0; i <= 9; i++) {
    printf("%d:", i);
    up = &((s->tableau_up)[i]);
    down = &((s->tableau_down)[i]);
    
    
    while(!stack_is_empty(down)) {
      printf(" _");
      stack_push(stack_pop(down), copy);
    }
    
    while(!stack_is_empty(copy)) {
      stack_push(stack_pop(copy), down);
    }
    
    while(!stack_is_empty(up)) {
      stack_push(stack_pop(up), copy);
    }
    
    while(!stack_is_empty(copy)) {
      
      top_card = stack_top(copy);

      if(top_card >= 2 && top_card <= 10) {
        printf(" %d", top_card);
      } else if (top_card == 1) {
        printf(" %c", 'A');
      } else if (top_card == 11) {
        printf(" %c", 'J');
      } else if (top_card == 12) {
        printf(" %c", 'Q');
      } else if (top_card == 13) {
        printf(" %c", 'K');
      }
      
      stack_push(stack_pop(copy), up);
    }
    
    printf("\n");
  }
}

void spider_print_faceup(struct spider *s) {
  // Replace the code below with your implementation.
  printf("spider_print_faceup has not been implemented yet.\n");
}

// complete_deck(s, index) checks if the tableau with given
// index is complete (K to A in decreasing order, A being at
// the top) or not and clears the 13 cards from tableau and
// increments comp_decks by 1 if tableau is complete, and does
// nothing otherwise
// requires: s is a valid pointer and 0 <= index <= 9
// effects: may modify s
static void complete_deck(struct spider *s, int index) {
  assert(s);
  assert(0 <= index && index <= 9);
  struct stack temp_stack;
  struct stack *temp = &temp_stack;
  stack_init(temp);
  struct stack *face_up = &(s->tableau_up[index]);
  
  for(int i = 1; i <= 13; i++) {
    if (stack_is_empty(face_up) || stack_top(face_up) != i) {
      while(!stack_is_empty(temp)) {
        stack_push(stack_pop(temp), face_up);
      }
      return;
    }
    stack_push(stack_pop(face_up), temp);
  }
  s->comp_decks += 1;
}

// reveal(s, index) turns the topmost face down card to
// face up if there are no other cards in the tableau (with
// given index) which are face up, and does nothing otherwise
// requires: s is a valid pointer and 0 <= index <= 9
// effects: may modify s
static void reveal(struct spider *s, int index) {
  assert(s);
  assert(0 <= index && index <= 9);
  struct stack *face_up = &(s->tableau_up[index]);
  struct stack *face_down = &(s->tableau_down[index]);
  if(stack_is_empty(face_up) && !stack_is_empty(face_down)) {
    stack_push(stack_pop(face_down), face_up);
  }
}

void spider_get_stock(struct spider *s) {
  assert(s);
  
  struct stack *stock = &(s->stock);
  struct stack *up;
  //struct stack *down;
  
  bool tableaus_nonempty = true;
  for (int i = 0; i <= 9; i++) {
    up = &(s->tableau_up[i]);
    if (stack_is_empty(up)) {
      tableaus_nonempty = false;
      break;
    }
  }
  if(!tableaus_nonempty || s->stock_size < 10) {
    printf("Error getting cards from the stock.\n");
    return;
  }
  
  for (int i = 0; i <= 9; i++) {
    up = &(s->tableau_up[i]);
    stack_push(stack_pop(stock), up);
    s->stock_size--;
    complete_deck(s, i);
    reveal(s, i);
  }
}

// spider_find_seq(s_src_up,temp,card) attempts
// to find a descending sequence of cards starting 
// with the provided card on top of s_src_up.  
// If such a sequence does not exist, 
// this function finds the longest descending sequence
// of cards on top of s_src_up.  After finding
// such a sequence, the function moves this sequence
// to temp in reverse order.  Then, the function
// returns true if the descending sequence ends with 
// the provided card.  It returns false otherwise.
// requires: s_src_up is non-empty.
//           s_src_up and temp are valid pointers.
//           1 <= card <= 13
static bool spider_find_seq(struct stack *s_src_up, 
                            struct stack *temp,
                            int card) {
  assert(s_src_up);
  assert(temp);
  assert(!stack_is_empty(s_src_up));
  assert(1 <= card && card <= 13);
  
  int top_card = -1;
  while(!stack_is_empty(s_src_up) && top_card != card) {
    if(stack_top(s_src_up) > top_card) {
      top_card = stack_pop(s_src_up);
      stack_push(top_card, temp);
    } else {
      return false;
    }
  }
  return (top_card == card);
}

void spider_test_find_seq(void) {
  struct stack src = {};
  stack_init(&src);
  struct stack temp = {};
  stack_init(&temp);
  int card = 0;

  // Marmoset public test
  stack_push(1, &src);
  card = 1;
  bool found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 1);
  assert(stack_is_empty(&src));
  assert(stack_is_empty(&temp));

  // The first example
  stack_push(5, &src);
  stack_push(4, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 2;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 2);
  assert(stack_is_empty(&temp));
  assert(stack_pop(&src) == 3);
  assert(stack_pop(&src) == 4);
  assert(stack_pop(&src) == 5);
  assert(stack_is_empty(&src));

  // The second example
  stack_push(5, &src);
  stack_push(4, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 5;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 5);
  assert(stack_pop(&temp) == 4);
  assert(stack_pop(&temp) == 3);
  assert(stack_pop(&temp) == 2);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));
  
  // My test 1
  found = spider_find_seq(&src, &temp, card);
  assert(!found);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));
  
  // My test 2
  stack_push(5, &src);
  stack_push(1, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 5;
  found = spider_find_seq(&src, &temp, card);
  assert(!found);
  assert(stack_pop(&temp) == 3);
  assert(stack_pop(&temp) == 2);
  assert(stack_pop(&src) == 2);
  assert(stack_pop(&src) == 3);
  assert(stack_pop(&src) == 1);
  assert(stack_pop(&src) == 5);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));
  
  // My test 3
  stack_push(5, &src);
  stack_push(4, &src);
  stack_push(3, &src);
  stack_push(2, &src);
  card = 6;
  found = spider_find_seq(&src, &temp, card);
  assert(!found);
  assert(stack_pop(&temp) == 5);
  assert(stack_pop(&temp) == 4);
  assert(stack_pop(&temp) == 3);
  assert(stack_pop(&temp) == 2);
  assert(stack_pop(&src) == 2);
  assert(stack_pop(&src) == 3);
  assert(stack_pop(&src) == 4);
  assert(stack_pop(&src) == 5);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));
  
  // My test 4
  stack_push(13, &src);
  stack_push(11, &src);
  stack_push(7, &src);
  stack_push(6, &src);
  card = 11;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 11);
  assert(stack_pop(&temp) == 7);
  assert(stack_pop(&temp) == 6);
  assert(stack_pop(&src) == 13);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));
  
  // My test 5
  stack_push(6, &src);
  stack_push(4, &src);
  stack_push(6, &src);
  stack_push(2, &src);
  card = 6;
  found = spider_find_seq(&src, &temp, card);
  assert(found);
  assert(stack_pop(&temp) == 6);
  assert(stack_pop(&temp) == 2);
  assert(stack_pop(&src) == 4);
  assert(stack_pop(&src) == 6);
  assert(stack_is_empty(&temp));
  assert(stack_is_empty(&src));

}


void spider_move(struct spider *s, 
                 int src, int card, int dest) {
  assert(s);
  assert(0 <= src && src <= 9);
  assert(0 <= dest && dest <= 9);
  assert(1 <= card && card <= 13);
  if (src == dest) {
    return;
  }
  struct stack temp_stack;
  struct stack *temp = &temp_stack;
  stack_init(temp);
  struct stack *face_up = &(s->tableau_up[src]);
  struct stack *dest_up = &(s->tableau_up[dest]);
  
  bool valid_card = spider_find_seq(face_up, temp, card);
  
  if (!valid_card || (!stack_is_empty(dest_up) && 
                      stack_top(dest_up) - 1 != stack_top(temp))) {
    printf("Invalid move.\n");
    while(!stack_is_empty(temp)) {
      stack_push(stack_pop(temp), face_up);
    }
    return;
  }
  
  while(!stack_is_empty(temp)) {
    stack_push(stack_pop(temp), dest_up);
  }
  
  reveal(s, src);
  complete_deck(s, dest);
}


