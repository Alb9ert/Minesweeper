//
// Minesweeper - Albert Hansen @Alb9ert (Github)
// 26-12-2023
// Demo -> Missing error handling and could optimize complexity
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

// Set to 1 to see placement of bombs
#define DEBUG 0

int NUMBERBOMBS;
int NUMBERCOLLUMNS;
int NUMBERROWS;
int NUMBERFIELDS;

enum difficulty
{
    easy = 1,
    medium,
    hard
};

// Struct for each field
typedef struct
{
    bool active;
    bool isBomb;
    int numPosition;
    char letPoisiton;
    char hidden[6];
    char shown[6];
    int hiddenPos;

} field;

field *GenerateTable();
void displayTable(field *);
char letter(int);
void inputPosition(int *, char *, int *);
void adjustTable(field *, int, char, int *);
void adjustFirstPick(field *, int);
int bombCount(field *, int);

int main(void)
{

    int play = 1;

    NUMBERBOMBS = 50;
    NUMBERCOLLUMNS = 12;
    NUMBERROWS = 10;
    NUMBERFIELDS = NUMBERCOLLUMNS * NUMBERROWS;

    int action = 1;
    int difficulty = 2;
    int settings;
    int tempDif = 2;
    int temp;
    int highscore = 0;

    //
    // Main menu/settings
    //

    while (1)
    {
        while (1)
        {
            system("clear");
            printf("\nMinesweeper --- Main Menu\n\n");
            printf("1. Start game\n");
            printf("2. Select difficulty\n");
            printf("3. Advanced settings\n");
            printf("4. Quit game\n\n");
            printf("Current highscore: %d\n\n", highscore);
            scanf(" %d", &action);

            // Start game
            if (action == 1)
            {
                break;
            }

            // Choose difficulty
            else if (action == 2)
            {
                difficulty = tempDif;

                while (difficulty != 4)
                {

                    system("clear");
                    printf("\nSelect difficulty\n\n");
                    if (difficulty == easy)
                    {
                        printf("1. Easy [X]\n");
                        printf("2. Medium [ ]\n");
                        printf("3. Hard [ ]\n");
                        printf("\n4. Main Menu\n\n");
                        tempDif = difficulty;
                        NUMBERBOMBS = 30;
                    }
                    else if (difficulty == medium)
                    {
                        printf("1. Easy [ ]\n");
                        printf("2. Medium [X]\n");
                        printf("3. Hard [ ]\n");
                        printf("\n4. Main Menu\n\n");
                        tempDif = difficulty;
                        NUMBERBOMBS = 60;
                    }
                    else if (difficulty == hard)
                    {
                        printf("1. Easy [ ]\n");
                        printf("2. Medium [ ]\n");
                        printf("3. Hard [X]\n");
                        printf("\n4. Main Menu\n\n");
                        tempDif = difficulty;
                        NUMBERBOMBS = 80;
                    }
                    else if (difficulty == 4)
                    {
                        action = 0;
                    }

                    scanf(" %d", &difficulty);
                }
            }

            // Advanced settings
            else if (action == 3)
            {
                while (settings != 4)
                {
                    system("clear");
                    printf("\nAdvanced Settings\n\n");
                    printf("1. Number of rows [%d]\n", NUMBERROWS);
                    printf("2. Number of collumns [%d]\n", NUMBERCOLLUMNS);
                    printf("3. Number of bombs [%d]\n\n", NUMBERBOMBS);
                    printf("4. Main Menu\n\n");

                    scanf(" %d", &settings);
                    system("clear");

                    if (settings == 1)
                    {
                        printf("\nInput new number of rows: ");
                        scanf(" %d", &temp);
                        NUMBERROWS = temp;
                        NUMBERFIELDS = NUMBERCOLLUMNS * NUMBERROWS;
                    }
                    else if (settings == 2)
                    {
                        printf("\nInput new number of collumns: ");
                        scanf(" %d", &temp);
                        NUMBERCOLLUMNS = temp;
                        NUMBERFIELDS = NUMBERCOLLUMNS * NUMBERROWS;
                    }
                    else if (settings == 3)
                    {
                        printf("\nInput new number of bombs: ");
                        scanf(" %d", &temp);
                        NUMBERBOMBS = temp;
                    }
                }
            }

            // Quit game
            else if (action == 4)
            {
                system("clear");
                return 0;
            }
        }

        //
        // The game itself
        //

        int intPosition;
        char charPosition;
        int score = 0;
        int pickedIndex;

        srand(time(NULL));

        system("clear");

        // Init table array
        field *fieldArr = GenerateTable();

        play = 1;

        while (play)
        {

            // Displays the table each turn
            displayTable(fieldArr);

            // Asks the user for a field position
            inputPosition(&intPosition, &charPosition, &score);

            // Adjusts each field according to user input
            adjustTable(fieldArr, intPosition, charPosition, &pickedIndex);

            // Adjusts according to it being the user's first pick
            if (score < 2)
            {
                adjustFirstPick(fieldArr, pickedIndex);
            }

            system("clear");

            printf("\tScore: %d\n", score);

            // Checks if the user hits a bomb
            for (int i = 0; i < NUMBERFIELDS; i++)
            {
                if (fieldArr[i].active && fieldArr[i].isBomb)
                {
                    system("clear");
                    printf("\n-----------------------------------------\n\t  HIT BOMB: GAME ERROR!!\t\n\t        Score: %d\t\n-----------------------------------------\n\n\n", score);
                    // Returns to main menu
                    printf("    Press 0 to return to main menu: ");
                    if (score > highscore)
                    {
                        highscore = score;
                    }
                    scanf(" %d", &play);
                }
            }
        }
    }

    return 0;
}

field *GenerateTable()
{
    int bombPlacement[NUMBERBOMBS];

    for (int i = 0; i < NUMBERBOMBS; i++)
    {
        bombPlacement[i] = rand() % NUMBERFIELDS + 1;
    }

    field *table = (field *)malloc(sizeof(field) * NUMBERFIELDS);

    int z = 0;

    for (int i = 0; i < NUMBERFIELDS; i++)
    {
        table[i].active = false;

        // Calculate numPosition and letPoisiton
        table[i].numPosition = (i % NUMBERCOLLUMNS) + 1;
        table[i].letPoisiton = letter(i / NUMBERCOLLUMNS);

        table[i].hiddenPos = i + 1;
        strcpy(table[i].hidden, "[ ] ");
        strcpy(table[i].shown, "[ ] ");

        // Check for bomb placement
        for (int j = 0; j < NUMBERBOMBS; j++)
        {
            if (table[i].hiddenPos == bombPlacement[j])
            {
                table[i].isBomb = true;
                strcpy(table[i].shown, "[O] ");
            }
        }
    }

    return table;
}

void displayTable(field *fieldArr)
{
    int displayCharacter = 0;

    printf("\t ");

    for (int i = 0; i < NUMBERFIELDS; i++)
    {
        if (i % NUMBERCOLLUMNS == 0)
        {
            printf("\n");
            printf("\t %c| ", letter(displayCharacter));
            displayCharacter++;
        }

        if (DEBUG)
        {
            printf("%s", fieldArr[i].shown);
        }
        // If a certain field has been picked the active one is displayed
        else if (fieldArr[i].active)
        {
            printf("%s", fieldArr[i].shown);
        }
        // If it has not been picked so far
        else
        {
            printf("%s", fieldArr[i].hidden);
        }
    }

    printf("\n\t    -------------------------------------------------------");
    printf("\n\t     ");

    for (int i = 0; i < NUMBERCOLLUMNS; i++)
    {
        if (i > 8)
        {
            printf("%d  ", (i + 1));
            continue;
        }

        printf("%d   ", (i + 1));
    }

    printf("\n\n");
}

void inputPosition(int *intPosition, char *charPosition, int *score)
{

    printf("\tInput alphabetic position and then numeric position: ");
    while (getchar() != '\n')
        ;

    scanf("%c %d", charPosition, intPosition);

    *score += 1;
    // Clear input buffer

    // Turn inputted char to upper case
    *charPosition = toupper(*charPosition);
}

void adjustTable(field *tableArr, int intPosition, char charPosition, int *firstPicked)
{
    int bombsAround = 0;
    char bombCount[5];

    for (int i = 0; i < NUMBERFIELDS; i++)
    {

        if (tableArr[i].numPosition == intPosition && tableArr[i].letPoisiton == charPosition)
        {
            tableArr[i].active = true;

            if (!tableArr[i].isBomb)
            {

                if ((tableArr[i - 1].isBomb) && (i - 1) < NUMBERFIELDS)
                {
                    bombsAround++;
                }
                if ((tableArr[i + 1].isBomb) && (i + 1) < NUMBERFIELDS)
                {
                    bombsAround++;
                }
                if ((tableArr[i - NUMBERCOLLUMNS].isBomb) && (i - NUMBERCOLLUMNS) > 0)
                {
                    bombsAround++;
                }
                if ((tableArr[i + NUMBERCOLLUMNS].isBomb) && (i + NUMBERCOLLUMNS) < NUMBERFIELDS)
                {
                    bombsAround++;
                }
                if ((tableArr[(i + NUMBERCOLLUMNS) - 1].isBomb) && ((i + NUMBERCOLLUMNS) - 1) < NUMBERFIELDS)
                {
                    bombsAround++;
                }
                if ((tableArr[(i + NUMBERCOLLUMNS) + 1].isBomb) && ((i + NUMBERCOLLUMNS) + 1) < NUMBERFIELDS)
                {
                    bombsAround++;
                }
                if ((tableArr[(i - NUMBERCOLLUMNS) + 1].isBomb) && ((i - NUMBERCOLLUMNS) + 1) > 0)
                {
                    bombsAround++;
                }
                if ((tableArr[(i - NUMBERCOLLUMNS) - 1].isBomb) && ((i - NUMBERCOLLUMNS) - 1) > 0)
                {
                    bombsAround++;
                }

                sprintf(bombCount, "[%d] ", bombsAround);

                strcpy(tableArr[i].shown, bombCount);

                *firstPicked = i;

                break;
            }
        }
    }
}

void adjustFirstPick(field *fieldArr, int pickedIndex)
{
    char bombsCounted[5];
    int bombs;

    if (!fieldArr[pickedIndex + 1].isBomb)
    {
        bombs = bombCount(fieldArr, pickedIndex + 1);
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[pickedIndex + 1].shown, bombsCounted);
        fieldArr[pickedIndex + 1].active = true;
    }

    if (!fieldArr[pickedIndex - 1].isBomb)
    {
        bombs = bombCount(fieldArr, pickedIndex - 1);
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[pickedIndex - 1].shown, bombsCounted);
        fieldArr[pickedIndex - 1].active = true;
    }

    if (!fieldArr[pickedIndex - NUMBERCOLLUMNS].isBomb)
    {
        bombs = bombCount(fieldArr, pickedIndex - NUMBERCOLLUMNS);
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[pickedIndex - NUMBERCOLLUMNS].shown, bombsCounted);
        fieldArr[pickedIndex - NUMBERCOLLUMNS].active = true;
    }

    if (!fieldArr[pickedIndex + NUMBERCOLLUMNS].isBomb)
    {
        bombs = bombCount(fieldArr, pickedIndex + NUMBERCOLLUMNS);
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[pickedIndex + NUMBERCOLLUMNS].shown, bombsCounted);
        fieldArr[pickedIndex + NUMBERCOLLUMNS].active = true;
    }

    if (!fieldArr[(pickedIndex + NUMBERCOLLUMNS) - 1].isBomb)
    {
        bombs = bombCount(fieldArr, ((pickedIndex + NUMBERCOLLUMNS) - 1));
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[(pickedIndex + NUMBERCOLLUMNS) - 1].shown, bombsCounted);
        fieldArr[(pickedIndex + NUMBERCOLLUMNS) - 1].active = true;
    }

    if (!fieldArr[(pickedIndex + NUMBERCOLLUMNS) + 1].isBomb)
    {
        bombs = bombCount(fieldArr, ((pickedIndex + NUMBERCOLLUMNS) + 1));
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[(pickedIndex + NUMBERCOLLUMNS) + 1].shown, bombsCounted);
        fieldArr[(pickedIndex + NUMBERCOLLUMNS) + 1].active = true;
    }

    if (!fieldArr[(pickedIndex - NUMBERCOLLUMNS) - 1].isBomb)
    {
        bombs = bombCount(fieldArr, ((pickedIndex - NUMBERCOLLUMNS) - 1));
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[(pickedIndex - NUMBERCOLLUMNS) - 1].shown, bombsCounted);
        fieldArr[(pickedIndex - NUMBERCOLLUMNS) - 1].active = true;
    }

    if (!fieldArr[(pickedIndex - NUMBERCOLLUMNS) + 1].isBomb)
    {
        bombs = bombCount(fieldArr, ((pickedIndex - NUMBERCOLLUMNS) + 1));
        sprintf(bombsCounted, "[%d] ", bombs);
        strcpy(fieldArr[(pickedIndex - NUMBERCOLLUMNS) + 1].shown, bombsCounted);
        fieldArr[(pickedIndex - NUMBERCOLLUMNS) + 1].active = true;
    }
}

int bombCount(field *tableArr, int i)
{
    int bombsAround = 0;

    if ((tableArr[i - 1].isBomb) && (i - 1) < NUMBERFIELDS)
    {
        bombsAround++;
    }
    if ((tableArr[i + 1].isBomb) && (i + 1) < NUMBERFIELDS)
    {
        bombsAround++;
    }
    if ((tableArr[i - NUMBERCOLLUMNS].isBomb) && (i - NUMBERCOLLUMNS) > 0)
    {
        bombsAround++;
    }
    if ((tableArr[i + NUMBERCOLLUMNS].isBomb) && (i + NUMBERCOLLUMNS) < NUMBERFIELDS)
    {
        bombsAround++;
    }
    if ((tableArr[(i + NUMBERCOLLUMNS) - 1].isBomb) && ((i + NUMBERCOLLUMNS) - 1) < NUMBERFIELDS)
    {
        bombsAround++;
    }
    if ((tableArr[(i + NUMBERCOLLUMNS) + 1].isBomb) && ((i + NUMBERCOLLUMNS) + 1) < NUMBERFIELDS)
    {
        bombsAround++;
    }
    if ((tableArr[(i - NUMBERCOLLUMNS) + 1].isBomb) && ((i - NUMBERCOLLUMNS) + 1) > 0)
    {
        bombsAround++;
    }
    if ((tableArr[(i - NUMBERCOLLUMNS) - 1].isBomb) && ((i - NUMBERCOLLUMNS) - 1) > 0)
    {
        bombsAround++;
    }

    return bombsAround;
}

// ---------------------------------------------------------------
// Letter convert
// --------------------------------------------------------------

char letter(int i)
{
    switch (i)
    {
    case 0:
        return 'A';
        break;
    case 1:
        return 'B';
        break;
    case 2:
        return 'C';
        break;
    case 3:
        return 'D';
        break;
    case 4:
        return 'E';
        break;
    case 5:
        return 'F';
        break;
    case 6:
        return 'G';
        break;
    case 7:
        return 'H';
        break;
    case 8:
        return 'I';
        break;
    case 9:
        return 'J';
        break;
    case 10:
        return 'K';
        break;
    case 11:
        return 'L';
        break;
    case 12:
        return 'M';
        break;
    case 13:
        return 'N';
        break;
    case 14:
        return 'O';
        break;
    case 15:
        return 'P';
        break;
    case 16:
        return 'Q';
        break;
    case 17:
        return 'R';
        break;
    case 18:
        return 'S';
        break;
    case 19:
        return 'T';
        break;

    default:
        break;
    }
}