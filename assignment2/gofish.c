#include <stdio.h>
#include <time.h>
#include "gofish.h"

int main(int args, char *argv[])
{
  srand(time(NULL));

  do // do guarantees to execute at least one time
  {
    gamestart();
    do
    {
      if (gameplay())
        break;
      current_player = next_player; // if the winner is found, game ends
    } while (1);
  } while ((gameend()));
  fprintf(stdout, "Exiting.\n");
  return 0;
}



/*Function: game_start*/
void gamestart()
{
  //h = human --> Player 1, c = computer --> Player 2
  reset_player(&user);
  reset_player(&computer);   // resestting both the players before starting a new game

  fprintf(stdout, "Shuffling deck...\n");
  shuffle();
  deal_player_cards(&user);
  deal_player_cards(&computer);    // dealing cards to both the cards

  if (user.book[0] != 0)
    fprintf(stdout, "Player 1's Book - %s", printrank(user.book[0]));
  if (computer.book[0] != 0)
    fprintf(stdout, "Player 2's Book - %s", printrank(computer.book[0]));  // printing the books of both the players

  current_player = &user;         // giving first play to the human
  next_player = &computer;
}
/*Function: game_end*/
int gameend()
{
  struct player *other = (current_player == &user) ? &computer : &user; // check whether the current player is human or not, if yes, return &c, else return &h

  int counter = 0;
  while (other->book[counter] != 0 && counter < 6)        
  {
    counter++;                                    // counting the number of books a player has 
  }
  // if human wins the game
  if (current_player == &user)
  {
    fprintf(stdout, "Player 1 Wins! 7-%d\n", counter);
  }
  else
  {
    fprintf(stdout, "Player 2 Wins! 7-%d\n", counter);
  }
  char ask[3] = "";       // an array for the input of the user
  int error = 0;
  do
  {
    if (error)
    {
      fprintf(stdout, "Please enter \"Y\" or \"N\"");
      fprintf(stdout, "\n");
    }

    fprintf(stdout, "\nDo you want to play again [Y/N]: ");
    scanf("%2s", ask);
    error = 1;

    while (getchar() != '\n');
    
    if (ask[1] != '\0')
      continue; // check for 1st str 
    if (ask[0] == 'Y')
      return 1;                               // checking whether the user entered Y or N and taking the respective decision
    else if (ask[0] == 'N')
      return 0;
    
    else

      continue;
     
  } while (1);
}

void printhand(struct player *target)
{
  if (target->hand_size == 0)
  {
    fprintf(stdout, "\n");               
    return;
  } 
  struct hand *temp = target->card_list;
  fprintf(stdout, "%s%c", printrank(temp->top.rank[0]), temp->top.suit);          
  int k;
  for (k = 1; k < target->hand_size; k++)
  {
    temp = temp->next;
    fprintf(stdout, " %s%c", printrank(temp->top.rank[0]), temp->top.suit);
  }
  fprintf(stdout, "\n");
}

void printbook(struct player *target)    // prints ranks of books in players hand 
{
  //cases where the book will be empty
  if (target == NULL || target->book[0] == 0 ) 
  {
    fprintf(stdout, "\n");
    return;
  }
  fprintf(stdout, "%s ", printrank(target->book[0]));
  int index = 1;
  while (index < 7 && target->book[index] != 0  )  
  {
    fprintf(stdout, "%s ", printrank(target->book[index++]));
  }
  fprintf(stdout, "\n");
}

// X is for ten
const char *printrank(char rank)
{
  if (rank == 'X')
  {
    static char ten[] = "10"; //global variable
    return ten;
  }
  static char ranks[2];
  ranks[0] = rank;
  ranks[1] = '\0';
  return ranks;
}

int gameplay()         
{
  int i;
  fprintf(stdout, "\n");

  fprintf(stdout, "Player 1's Hand - ");
  printhand(&user);
  fprintf(stdout, "Player 1's Book - ");
  printbook(&user);
  fprintf(stdout, "Player 2's Book - ");
  printbook(&computer);

  struct player *other = (current_player == &user) ? &computer : &user;   

  if (game_over(current_player))    // if the current player has all 7 books then return 1
  {
    return 1;
  }
  if (game_over(other))
  {
    current_player = other;          // if other player has all 7 books then change the current player to other player and return 1 

    return 1;
  }

  char rank_guess;
  if (current_player->hand_size > 0)
  {
    if (current_player == &user)
    {
      rank_guess = user_play(current_player);
    }
    else
    {
      rank_guess = computer_play(current_player);
    }
  }
  else
    rank_guess = 'Z';     // case where there is nothing in the players hand 

  if (search(other, rank_guess) == 0)      // case where the other player doesn't have the rank the current player asks for
  {
    if (rank_guess != 'Z')        
      fprintf(stdout, "    - Player %d has no %s's\n", ((current_player == &user) ? 2 : 1), printrank(rank_guess));     

    struct card *drawncard = next_card();
    int nextbook = 0;
    if (drawncard != NULL)
    {
      if (current_player == &user)
        fprintf(stdout, "    - Go Fish, Player 1 draws %s%c\n", printrank(drawncard->rank[0]), drawncard->suit);
      else
      {
        if (drawncard->rank[0] == rank_guess)
        {
          fprintf(stdout, "    - Go Fish, Player 2 draws %s%c\n", printrank(drawncard->rank[0]), drawncard->suit);
        }
        else
        {
          fprintf(stdout, "    - Go Fish, Player 2 draws a card\n");
        }
      }

      for (i = 0; current_player->book[nextbook] != 0; i++)    // getting the index of the empty position in book array
      {
        nextbook++;
      }

      add_card(current_player, drawncard);   // adding the drawncard to the cardlist of the player
      if (current_player->book[nextbook] != 0)
        fprintf(stdout, "    - Player %d books %s\n", ((current_player == &user) ? 1 : 2), printrank(drawncard->rank[0]));  
    }

    if (drawncard != NULL && (drawncard->rank[0] == rank_guess || current_player->book[nextbook] != 0 ))    
    {
      next_player = current_player;
      fprintf(stdout, "    - Player %d gets another turn\n", ((current_player == &user) ? 1 : 2));    // player gets another turn if the drawn card is the rank that the player wants
    }
    else
    {
      next_player = other;
      fprintf(stdout, "    - Player %d's turn\n", ((next_player == &user) ? 1 : 2));
    }
  }
  else
  {
    struct player *player0 = current_player;
    int i;
    for (i = 0; i < 2; i++)
    {
      if ( player0 == &computer && current_player == &computer ) 
      {
        player0 = (current_player == &user) ? &computer : &user;      // looping and printing through the cards of the rank entered in both players hands
        continue;
      }
      fprintf(stdout, "    - Player %d has ", ((player0 == &user) ? 1 : 2));   // Switching players
      int j;
      struct hand *temp = player0->card_list;
      int rank_count = 0;
      for (j = 0; j < player0->hand_size && temp != NULL; j++)    
      {
        if (temp->top.rank[0] == rank_guess)
        {
          if (rank_count++ > 0)                            
            fprintf(stdout, ", ");
          fprintf(stdout, "%s%c", printrank(rank_guess), temp->top.suit);
        }

        temp = temp->next;
      }
      fprintf(stdout, "\n");
      player0 = (current_player == &user) ? &computer : &user;
    }

    int nextbook = 0;
    while (current_player->book[nextbook] != 0)
    {
      nextbook++;
    }
    transfer_cards(other, current_player, rank_guess);      // transferring cards from one player to the other
    if (current_player->book[nextbook] != 0)
      fprintf(stdout, "    - Player %d books %s\n", ((current_player == &user) ? 1 : 2), printrank(rank_guess));    // printing books of players

    if (current_player->book[nextbook] != 0)
    {
      next_player = current_player;
      fprintf(stdout, "    - Player %d gets another turn\n", ((current_player == &user) ? 1 : 2));     // another turn when a player gets a book
    }
    else
    {
      next_player = other;
      fprintf(stdout, "    - Player %d's turn\n", ((next_player == &user) ? 1 : 2));       
    }
  }
  return 0;
}
