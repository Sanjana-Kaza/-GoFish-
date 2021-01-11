#include <stdlib.h>
#include <string.h>
#include <stdio.h>  /* Needed for printf */
#include <unistd.h> /* Needed for read */
#include "deck.h"

/*
* Function: shuffle
* --------------------

*/
int shuffle()
{

    int suit_index, rank_index;
    int shuffle_val, shuffle_card;

    /* if the deck is empty */
    if (deck_instance.list[0].suit == '\0')
    {
        char filldeck_suit[4] = {'C', 'D', 'H', 'S'};
        char filldeck_rank[14] = {'2', '3', '4', '5', '6', '7', '8', '9', 'X', 'J', 'Q', 'K', 'A'};
        //int i = 0;
        for (suit_index = 0; suit_index < 4; suit_index++)
        {
            for (rank_index = 0; rank_index < 13; rank_index++)
            {

                struct card addedcard;

                addedcard.suit = filldeck_suit[suit_index];
                addedcard.rank[0] = filldeck_rank[rank_index];

                //addedcard.rank[i++] = filldeck_rank[rank_index]; // rank_index = 10 --> [10] = {'1', '0'}
                /////////////////////
                deck_instance.list[deck_instance.top_card++] = addedcard; // add card to deck
            }
        }
    }

    /*Shuffling part*/
    for (shuffle_card = 51; shuffle_card > 0; shuffle_card--)
    {
        shuffle_val = rand() % shuffle_card;                                // generating random shuffling
        struct card temp_card = deck_instance.list[shuffle_card];           //store the shuffled card to somewhere temporary
        deck_instance.list[shuffle_card] = deck_instance.list[shuffle_val]; //swap the position
        deck_instance.list[shuffle_val] = temp_card;                        // put the temp card to the previously swapped index
    }
    deck_instance.top_card = 52; // set the default full deck value
    return 0;
}

/*
* Function: size
* --------------

*/
size_t deck_size()
{
    //check the cases that the size == 0
    if (deck_instance.top_card < 0 || deck_instance.list[0].rank[0] == 0 || deck_instance.list[0].suit == 0)
    {
        return 0;
    }
    return deck_instance.top_card;
}

/*
* Function: next_card
* -------------------
*/
struct card *next_card()
{
    if (deck_instance.top_card <= 0)
    {
        return NULL;
    }
    return &deck_instance.list[--deck_instance.top_card]; // return the next element in the card list
}

/*
* Function: deal_player_cards
* ---------------------------
*/
int deal_player_cards(struct player *target)
{
    int delt;
    struct card *deltcard;
    for (delt = 0; delt < 7; delt++)
    {
        if (deck_size() <= 0)
        {
            return -1; // returns non-zero on error
        }

        deltcard = next_card();

        // based on the next_card() method that NULL is a possible return value
        if (deltcard == NULL)
        {
            return -1;
        }
        add_card(target, deltcard); // in player.c
    }
    if (delt != 7)
    {
        return -1;
    }
    return 0; // if there is no error
}