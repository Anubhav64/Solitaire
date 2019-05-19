#include "cs136.h"
#include "spider.h"


void check_initialized(int initialized) {
    if (!initialized) {
      printf("You must initialize the game first.\n");
      printf("Stopping Program\n");
      exit(1);
    } 
}

int main(void) {


  // define commands
  const int INITCUSTOM = lookup_symbol("initcustom");
  const int INITRANDOM = lookup_symbol("initrandom");
  const int WON = lookup_symbol("won");
  const int QUIT = lookup_symbol("quit");
 
  // Q1
  const int PRINT = lookup_symbol("print");
  const int PRINTFACEUP = lookup_symbol("printfaceup");
  
  // Q2
  const int GETSTOCK = lookup_symbol("getstock");
  
  // Q3
  const int TESTFINDSEQ = lookup_symbol("testfindseq");
  
  // Q4
  const int MOVE = lookup_symbol("move");

  int initialized = 0;

  // create spider struct
  struct spider s = {};

  while (1) {

    int cmd = read_symbol();
    if (cmd == INITCUSTOM) {
      spider_init_custom(&s);
      ++initialized;
    } else if (cmd == INITRANDOM) {
      int seed = 0;
      scanf("%d", &seed);
      spider_init_random(&s, seed);
      spider_print(&s);
      ++initialized;
    } else if (cmd == WON) {
      check_initialized(initialized);
      bool won = spider_has_won(&s);
      if (won) {
        printf("Congratulations, you have won the game! Quitting...\n");
        break;
      } else {
        printf("You have not won the game yet. Keep going!\n");
      }
    } else if (cmd == PRINT) {
      check_initialized(initialized);
      // print
      spider_print(&s);
    } else if (cmd == PRINTFACEUP) {
      check_initialized(initialized);
      // print all cards face up
      spider_print_faceup(&s);
    } else if (cmd == GETSTOCK) { 
      check_initialized(initialized);
      // get stock cards
      spider_get_stock(&s);
      // print
      spider_print(&s);
    } else if (cmd == TESTFINDSEQ) {
      check_initialized(initialized);
      spider_test_find_seq();
    } else if (cmd == MOVE) {
      check_initialized(initialized);
      int src = 0;
      scanf("%d", &src);
      int card = 0;
      read_card(&card);
      int dest = 0;
      scanf("%d", &dest);

      // move
      spider_move(&s, src, card, dest);
      // print
      spider_print(&s);
    } else if (cmd == QUIT) {
      break;
    } else {
      printf("Invalid command = ");
      print_symbol(cmd);
      printf("\n");
      printf("Stopping Program\n");
      exit(1);
    }
  }

}