//Header
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

//Structure for Card
typedef struct card_struct
{
    char suit;
    int face;
    struct card_struct *next;
} card;

//Structure for Player
typedef struct player_struct
{
    char name[25];
    int points;
    int winner_of_turn;
    int is_player;
    card *hand;
    struct player_struct *next;
} player;

//Creates a linked list of players
void player_list(player *player_head, int num);

//Creates a linked list of cards
void create_deck(card *card_head, int num);

//Initializes the deck of cards
void initialize_deck(card *deck_head);

//Prints cards
void print_cards(card *card_head);

//Sets up the game.
void begin(card **deck, player *player_head, int *numPlayers);

//Shuffles the deck of cards
void shuffle_deck(card **deck_head);

//Removes a card from a linked list and returns the removed card’s address
card *remove_card(card **deck_head, int position);

//Adds a card into a linked list
void add_card(card **card_head, card *selected, int position);

//Deals cards to the players
void deal_cards(card *deck, player *head, int numPlayers);

//Counts cards in a linked list
int count_cards(card *head);

//Moves pointer to point at starting player
void player_start(player **head, int turn, int numPlayers);

//Selects a card to play. Removes it from players hand and adds it to table linked list.
void select_card(player *head, card **table, int numPlayers, int *heart_played, int turn);

//Compares cards on the table. Winner is updated, points are given, and table is cleared.
void compare_cards(player **head, card **table, int numPlayers, int *heart_played);

//Sorts the cards in a given linked list
void sort_cards(card **card_head);

//Swaps cards
void swap_adjacent_cards(card **card_head, card *card1, card *card2);

//Looks for a specific card in a linked list based on face value and suit
int search_for_card(card *card_head, int face, char suit);

//Looks for the position of a particular card and returns it's postion
int position_of_card(card *head, card *find);

//Makes sure selected card is valid
int is_valid_selection(card *table, int heart_played, int playerChoice, card *playerHand, int turn, int is_player);

int main(void)
{
    player *player_head, *winner;
    card *deck, *table;
    int numPlayers, heart_played = 0, turn;
    char cont = '\n';

    while (cont != 'q')
    {
        turn = 1;
        player_head = (player *)malloc(sizeof(player));
        deck = (card *)malloc(sizeof(card));
        table = NULL;

        begin(&deck, player_head, &numPlayers);
        printf("Let's begin!! First Round Starts with 2 of clubs\n\n");
        printf("- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -\n\n");
        player_start(&player_head, turn, numPlayers); //Determines which player has the 2 of clubs

        while (count_cards(player_head->hand) != 0)
        {
            for (int i = 0; i < numPlayers; i++)
            {
                select_card(player_head, &table, numPlayers, &heart_played, turn);
                print_cards(table);
                player_head = player_head->next;
                turn++;
            }
            compare_cards(&player_head, &table, numPlayers, &heart_played);
        }
        winner = player_head;
        for (int i = 0; i < numPlayers; i++)
        {
            if (winner->points > player_head->points)
                winner = player_head;
            player_head = player_head->next;
        }
        printf("\n\nTHE WINNER IS %s WITH %d POINTS\n\n", winner->name, winner->points);

        for (int i = 0; i < numPlayers; i++)
        {
            winner = player_head->next;
            free(player_head);
            player_head = winner;
        }
        free(deck);
        cont = '\n';
        printf("Do you want to play again (q to quit): ");
        while (cont == '\n')
            scanf("%c", &cont);
    }
    return 0;
}

//1 Premnaath
void begin(card **deck, player *player_head, int *numPlayers)
{
    char name[25];
    printf("\nEnter your name: ");
    scanf("%s", name);
    printf("\n%s, lets play Hearts!\n", name);
    printf("\nRules : At the end of each hand, players count the number of hearts they have taken as well as the queen of spades, if applicable.");
    printf("\nHearts count as one point each and the queen counts as 13 points. The player with the lowest points at the end wins.");
    printf("\nKey : C- Clubs   H- Hearts   D- Diamond   S- Spades   K- King   Q- Queen   J- Jack   A- Ace(Highest value)\n\n\n");

    do
    {
        printf("Enter the number of players(3/4): ");
        scanf("%d", numPlayers);
        printf("\n");
    } while (!(*numPlayers == 3 || *numPlayers == 4));

    create_deck(*deck, 52);
    initialize_deck(*deck);
    if (*numPlayers == 3) //Removes the 2D if there are only 3 players
        remove_card(deck, 41);
    shuffle_deck(deck);

    player_list(player_head, *numPlayers);
    deal_cards(*deck, player_head, *numPlayers);

    for (int i = 0; i < *numPlayers; i++)
    {
        sort_cards(&(player_head->hand));
        if (i == 0)
        {
            strcpy(player_head->name, name);
            player_head->is_player = 1;
            printf("%s's Deck\n", player_head->name);
            print_cards(player_head->hand);
        }
        else if (i == 1)
        {
            strcpy(player_head->name, "Computer 1");
            player_head->is_player = 0;
        }
        else if (i == 2)
        {
            strcpy(player_head->name, "Computer 2");
            player_head->is_player = 0;
        }
        else if (i == 3)
        {
            strcpy(player_head->name, "Computer 3");
            player_head->is_player = 0;
        }
        player_head->points = 0;
        player_head = player_head->next;
    }
}

void create_deck(card *card_head, int num)
{
    card *temp;
    temp = card_head;
    for (int i = 1; i <= num; i++)
    {
        if (i < num)
        {
            temp->next = (card *)malloc(sizeof(card));
            temp = temp->next;
        }
        else
        {
            temp->next = NULL;
        }
    }
}

void initialize_deck(card *deck_head)
{
    for (int i = 0; i < 4; i++) //4 Suits
    {
        for (int j = 0; j < 13; j++) //13 Face values
        {
            deck_head->face = j + 1;
            switch (i)
            {
            case 0:
                deck_head->suit = 'S';
                break;
            case 1:
                deck_head->suit = 'H';
                break;
            case 2:
                deck_head->suit = 'C';
                break;
            case 3:
                deck_head->suit = 'D';
                break;
            }
            deck_head = deck_head->next;
        }
    }
}

int count_cards(card *head)
{
    int i = 0;
    while (head != NULL)
    {
        head = head->next;
        i++;
    }
    return i;
}

void player_list(player *player_head, int num)
{
    player *temp = NULL;
    temp = player_head;
    for (int i = 1; i <= num; i++)
    {
        if (i < num)
        {
            temp->next = (player *)malloc(sizeof(player));
            temp = temp->next;
        }
        else
        {
            temp->next = player_head;
        }
    }
}

void shuffle_deck(card **deck_head)
{
    srand(time(NULL));
    int rand1, rand2;
    card *holder;
    for (int i = 0; i < 500; i++)
    {
        rand1 = (rand() % count_cards(*deck_head)) + 1; //Random number from 1 - 52
        rand2 = (rand() % count_cards(*deck_head)) + 1;
        holder = remove_card(deck_head, rand1);
        add_card(deck_head, holder, rand2);
    }
}

//2 Sparsh
card *remove_card(card **deck_head, int position)
{
    card *temp, *temp2;
    temp = *deck_head;
    if (position == 1) //Delete from beginning
    {
        *deck_head = temp->next;
        temp->next = NULL;
        return temp;
    }
    else if (position <= count_cards(*deck_head)) //Delete from any position
    {
        for (int i = 1; i < position - 1; i++)
            temp = temp->next;
        temp2 = temp->next;
        temp->next = temp2->next;
        temp2->next = NULL;
        return temp2;
    }
}

void print_cards(card *card_head)
{
    card *temp = card_head;
    printf("\n");
    //Card number
    for (int i = 0; i < count_cards(card_head); i++)
        printf("  %2d   ", i + 1);
    printf("\n");
    //Top of the card
    for (int i = 0; i < count_cards(card_head); i++)
        printf(" ----- ");
    printf("\n");
    //Top left of the card
    for (int j = 0; j < count_cards(card_head); j++)
    {
        while (temp != NULL)
        {
            switch (temp->face)
            {
            case 1:
                printf("|A%c   |", temp->suit);
                break;
            case 11:
                printf("|J%c   |", temp->suit);
                break;
            case 12:
                printf("|Q%c   |", temp->suit);
                break;
            case 13:
                printf("|K%c   |", temp->suit);
                break;
            case 10:
                printf("|10%c  |", temp->suit);
                break;
            default:
                printf("|%d%c   |", temp->face, temp->suit);
                break;
            }
            temp = temp->next;
        }
    }
    printf("\n");
    //Middle lines of the card
    for (int k = 0; k < 2; k++)
    {
        for (int i = 0; i < count_cards(card_head); i++)
            printf("|     |");
        printf("\n");
    }
    temp = card_head;
    //Bottom right of the card
    for (int j = 0; j < count_cards(card_head); j++)
    {
        while (temp != NULL)
        {
            switch (temp->face)
            {
            case 1:
                printf("|   A%c|", temp->suit);
                break;
            case 11:
                printf("|   J%c|", temp->suit);
                break;
            case 12:
                printf("|   Q%c|", temp->suit);
                break;
            case 13:
                printf("|   K%c|", temp->suit);
                break;
            default:
                printf("|  %2d%c|", temp->face, temp->suit);
                break;
            }
            temp = temp->next;
        }
    }
    printf("\n");
    //End of the card
    for (int i = 0; i < count_cards(card_head); i++)
        printf(" ----- ");
    printf("\n\n");
}

void add_card(card **card_head, card *selected, int position)
{
    card *temp, *temp2;
    temp = *card_head;
    temp2 = selected;

    if (position == 1) //Add to beginning
    {
        temp2->next = *card_head;
        *card_head = temp2;
    }
    else if (position <= count_cards(*card_head)) //Add to any middle position
    {
        for (int i = 1; i < position - 1; i++)
            temp = temp->next;
        temp2->next = temp->next;
        temp->next = selected;
    }
    else if (position == count_cards(*card_head) + 1) //Add to end
    {
        while (temp->next != NULL)
            temp = temp->next;
        temp->next = temp2;
        temp2->next = NULL;
    }
}

void deal_cards(card *deck, player *head, int numPlayers)
{
    for (int i = 0; i < numPlayers; i++)
    {
        head->hand = remove_card(&deck, 1);
        head = head->next;
    }
    while (count_cards(deck) != 0)
    {
        add_card(&(head->hand), remove_card(&deck, 1), count_cards(head->hand) + 1);
        head = head->next;
    }
}

void sort_cards(card **card_head)
{
    card *temp;
    int flag;
    do
    {
        temp = *card_head;
        flag = 1;
        while (temp->next != NULL && temp != NULL) //Bubble Sort
        {
            if (temp->face > (temp->next)->face)
            {
                swap_adjacent_cards(card_head, temp, temp->next);
                flag = 0;
            }
            if (temp->next != NULL)
                temp = temp->next;
        }
    } while (flag != 1);
}

void swap_adjacent_cards(card **card_head, card *card1, card *card2)
{
    int card1_pos = -1, card2_pos = -1;
    card1_pos = position_of_card(*card_head, card1);
    card2_pos = position_of_card(*card_head, card2);
    card *temp1;
    if (card1_pos != -1 && card2_pos != -1)
    {
        temp1 = remove_card(card_head, card2_pos);
        add_card(card_head, temp1, card1_pos);
    }
}

//3 Vinayak
int position_of_card(card *head, card *find)
{
    int i = 1;
    while (head != NULL)
    {
        if ((find->face == head->face) && (find->suit == head->suit))
            return i;
        i++;
        head = head->next;
    }
    return -1;
}

void player_start(player **head, int turn, int numPlayers)
{
    if (turn == 1)
    {
        while (search_for_card((*head)->hand, 2, 'C') != 1) //Looks for 2 of Clubs (2C) in each of the players hands
            *head = (*head)->next;
    }
}

int search_for_card(card *card_head, int face, char suit)
{
    int flag = 0;
    for (int i = 0; i < count_cards(card_head); i++)
    {
        if (face == card_head->face && suit == card_head->suit)
            flag = 1;
        card_head = card_head->next;
    }
    return flag;
}

void select_card(player *head, card **table, int numPlayers, int *heart_played, int turn)
{
    int playerChoice, index = 1;
    card *playersCard;
    printf("%s: ", head->name);
    if (head->is_player == 1)
    {
        printf("Current Hand:\n");
        print_cards(head->hand);
        do
        {
            printf("Select a card(1 - %d): ", count_cards(head->hand));
            scanf("%d", &playerChoice);
        } while (playerChoice < 1 || playerChoice > count_cards(head->hand) || !is_valid_selection(*table, *heart_played, playerChoice, head->hand, turn, head->is_player));
        if (*table == NULL)
            *table = remove_card(&(head->hand), playerChoice);
        else
            add_card(table, remove_card(&(head->hand), playerChoice), count_cards(*table) + 1);
    }
    else
    {
        while (!is_valid_selection(*table, *heart_played, index, head->hand, turn, head->is_player))
            index++;
        if (*table == NULL)
            *table = remove_card(&(head->hand), index);
        else
            add_card(table, remove_card(&(head->hand), index), count_cards(*table) + 1);
    }
}

void compare_cards(player **head, card **table, int numPlayers, int *heart_played)
{
    card *first = *table, *others = *table, *temp = *table;
    player *players = *head, *winner = *head;
    int pen_points = 0;
    do
    {
        if ((first->suit == others->suit) && (first->face < others->face || others->face == 1))
        {
            winner = players;
            first = others;
            if (first->face == 1)
                break;
        }
        others = others->next;
        players = players->next;
    } while (others != NULL);
    *head = winner; //Sets player headnode to winner so they start the next hand

    for (int i = 0; i < numPlayers; i++)
    {
        temp = remove_card(table, 1);
        if (temp->suit == 'H')
        {
            pen_points++;
            *heart_played = 1;
        }
        else if ((temp->suit == 'S') && (temp->face == 12))
            pen_points += 13;
        free(temp);
    }
    (*head)->points += pen_points;
    printf("\n*%s won the trick and gained %d points*\n\n", (*head)->name, pen_points);
    *table = NULL;
}

int is_valid_selection(card *table, int heart_played, int playerChoice, card *playerHand, int turn, int is_player)
{
    int has_suit = 0, valid = 0;
    card *selected_card = playerHand, *temp = playerHand;
    for (int i = 0; i < playerChoice - 1; i++)
        selected_card = selected_card->next;
    if (turn == 1)
    {
        if (selected_card->face == 2 && selected_card->suit == 'C')
            valid = 1;
        else
        {
            if (is_player)
                printf("Card must be the 2%c\n", 'C');
            valid = 0;
        }
    }
    else if (table == NULL)
    {
        if (heart_played)
            valid = 1;
        else if (selected_card->suit != 'H')
        {
            valid = 1;
        }
        else
        {
            if (is_player)
                printf("%c cannot be played until hearts have been broken\n", 'H');
            valid = 0;
        }
    }
    else if (table != NULL)
    {
        while (temp != NULL)
        {
            if (temp->suit == table->suit)
                has_suit = 1;
            temp = temp->next;
        }
        if (has_suit)
        {
            if (selected_card->suit == table->suit)
                valid = 1;
            else
            {
                if (is_player)
                    printf("Card must match leading suit\n");
                valid = 0;
            }
        }
        else
            valid = 1;
    }
    else
        return -1;

    return valid;
}