#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "player.h"

/*
 * Function: add_card
 * -------------------
 */
int add_card(struct player *target, struct card *new_card)
{
 
 
 if (target->hand_size == 0)        //case of first card bring added
 {
        

  target->card_list = (struct hand *)malloc(sizeof(struct hand));    
  target->card_list->top = *new_card;       // adding new card to the top of the hand
  target->card_list->next = NULL;
 }
 else
 {
  int counter = 0;
  struct hand *temp = target->card_list;       
  while (counter < target->hand_size - 1)
  {
   temp = temp->next;                   // going to next of last card 
   counter++;
  }
  
  if (counter != target->hand_size - 1) {return -1;}
  
  temp->next= (struct hand *)malloc(sizeof(struct hand));
  temp->next->top = *new_card;                // adding new card to the end
  temp->next->next = NULL;          
 }
 target->hand_size++;          // increasing the hand size


 check_add_book(target);        // checking whether it forms a book

 return 0;        
}  



/*
 * Function: check_add_book
 * ------------------------
 */
char check_add_book(struct player *target)
{
 if (target->card_list == NULL)
 {
  return 0;
 }

 struct hand *firsth;                      //first hand
 struct hand *secondh;                     //second hand
 struct hand *thirdh;                      // third hand
 struct hand *fourthh = target->card_list; // fourth hand
 int i = 0;
 int j = 0;
 while (i++ < target->hand_size && fourthh->next != NULL)
 {
  fourthh = fourthh->next;    
 }
 int count_hand = 1;
 struct card last_card = fourthh->top;  

 struct hand *current = target->card_list;

 while (j < target->hand_size - 1 && current != NULL)
 {
  if (current->top.rank[0] == last_card.rank[0])  // checking whether the current card's rank is equal to the newly added card's rank
  {
   if (count_hand == 1)         {firsth = current; }    
   else if (count_hand == 2)    {secondh = current;}
   else if (count_hand == 3)
   {
    thirdh = current;
    count_hand++;                // if count = 3 i.e there are three more cards of the same rank  other than the newly added one then break out of the loop
    break;
   }
   else
   {

    return 0;      // not equal to any then return 0
   }
   count_hand++;          // increasing the count if the cards match
  }
  j++;
  current = current->next;  // going to the next card in list
 }

 if (count_hand == 4)
 {
  char rank = firsth->top.rank[0];     

  for (i = 0; i < 7; i++)
  {
   if (target->book[i] == 0  )  
   {
    target->book[i] = rank;    // adding the rank to the books list
    break;
   }
  }

  remove_card(target, &firsth->top);
  remove_card(target, &secondh->top);
  remove_card(target, &thirdh->top);         // removing cards of the book from the list
  remove_card(target, &fourthh->top);

  return rank;
 }

 return 0;
}

/*
 * Function: search
 * ----------------
 */
int search(struct player *target, char rank)
{
 int i = 0;
 struct hand *current_hand = target->card_list;
 while (current_hand != NULL && i < target->hand_size)
 {
  if (current_hand->top.rank[0] == rank)             // iterating through the cards list and returning 1 if the rank matches
   return 1;

  current_hand = current_hand->next;
  i++;
 }

 return 0;
}


/*
 * Function: game_over
 * -------------------
 */
int game_over(struct player *target)
{
 int i = 0;
 while (i < 7)
 {
  if (target->book[i] == 0 )      
   return 0;          // out of 7 books if any book is zero then return 0
  i++;
 }

 return 1;
}

/*
 * Function: reset_player
 * ----------------------
 */
int reset_player(struct player *target)
{

 while (target->card_list != NULL)
 {
  remove_card(target, &target->card_list->top);   // removing all cards from the card list
 }

 if (target->hand_size != 0 || target->card_list != NULL)   {return -1; }
 int i = 0;
 while (i < 7)
 {
  target->book[i] = 0;    // making all books as 0
  i++;
 }
 if (target->hand_size != 0 || target->card_list != NULL)   {return -1; }
 if (i != 7 || target->hand_size != 0) {return -1;}
 return 0;
}


/*
 * Function: transfer_cards
 * ------------------------
 */
int transfer_cards(struct player *src, struct player *dest, char rank)
{
 int i = 0;
 int transferred = 0;
 struct hand *srchand = src->card_list; 
 while (srchand != NULL)
 {
  if (srchand->top.rank[0] == rank)
  {
   add_card(dest, &srchand->top);
   remove_card(src, &srchand->top);        // if the rank matches the rank of any cards of source player then add card to destination player and remove it from the dource's list
   srchand = srchand->next;                
   transferred++;          
  }
  else
  {
   srchand = srchand->next;           
  }
  i++;
 }

 if (i != src->hand_size)
 {
  return -1;
 }

 return transferred;
}


/*
 * Function: computer_play
 * -----------------------
 */
 
char computer_play(struct player *target)
{
 int i;
 int rand_in = rand() % target->hand_size;    // using rand() to get a random rank from computer's hand 
 struct hand *current_hand = target->card_list;    
 for (i = 0; i < rand_in; i++)
 {
  current_hand = current_hand->next;         
 }

 if (current_hand->top.rank[0] == 'X')
  fprintf(stdout, "Player 2's turn, enter a Rank: 10\n"); // the rank is 10
 else
  fprintf(stdout, "Player 2's turn, enter a Rank: %c\n", current_hand->top.rank[0]);   

 return current_hand->top.rank[0];
}

/*
 * Function: user_play
 * -------------------
 */
 
char user_play(struct player *target)
{
 char rank;
 do
 {
  fprintf(stdout, "Player 1's turn, enter a Rank: ");
  char user_intput[3] = "";          // getting user's rank input in an array to accomodate 10
  scanf("%3s", user_intput);
  while (getchar() != '\n');
  
  if (user_intput[0] == '1' && user_intput[1] == '0' && user_intput[2] == '\0')
   rank = 'X';                        // case where the entered rank is 10, then store rank as X
  else if (user_intput[1] == '\0')
   rank = user_intput[0];              
  else
  {
   fprintf(stdout, "Error - must have at least one card from rank to play\n");       // cases other than 10 and single char rank
   continue;
  }

  if (search(target, rank) && user_intput[0] != 'X')   { break;}               // if the entered rank is there in the card list then break and return the rank

  fprintf(stdout, "Error - must have at least one card from rank to play\n");   // if it's none of the above cases then the entered rank is not there in the card list
 } while (1);     // to re prompt the user to enter another rank

 return rank;
}

/*
 * Function: remove_card
 * ---------------------
 */

int remove_card(struct player *target, struct card *old_card)
{
 //check whether hand is empty or not
 if (target->hand_size <= 0)
  return -1; // return non-zero becuase error

 int found = 0; //if found
 int counter;
 struct hand *previous = NULL;
 struct hand *current = target->card_list;
 for (counter = 0; counter < target->hand_size; counter++)
 {
  if (current != NULL &&
      (current->top.suit == old_card->suit &&
       current->top.rank[0] == old_card->rank[0]))     
  {
   found = 1;
   break;                    // case where the entered rank is found in the card list
  }
  previous = current;
  current = current->next;   // updating previous and current card
 }

 if (found == 0)
 {
  return -1; // return non-zero for error
 }

 if (previous == NULL || previous->top.suit == 0 || &(previous->top.suit) == NULL   ) 
 {
  target->card_list = target->card_list->next; //remove the card by changing the linking
  free(current);                                          
  target->hand_size--; //decrement the hand_size

  return 0;
 }
 else if (current == NULL || &(current->top.suit) == NULL)  
 {
  return -1; // return non-zero for error
 }
 else
 {

  previous->next = current->next;       
  free(current);
  target->hand_size--;

  return 0;
 }
 return -1;
}