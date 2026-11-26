
#include <stdio.h>
#include <string.h>
#define MAX 100

int tname[MAX], total[MAX], avail[MAX], tcount = 0;
char name[MAX][50], source[MAX][30], dest[MAX][30], time[MAX][10];
float fare[MAX];

// BOOKING ARRAYS
int pnr[MAX], bt[MAX], seat[MAX], status[MAX], bcount = 0;
char passenger[MAX][50];
int nextPNR = 1001;

// 1 = Confirmed, 2 = Waiting, 3 = Cancelled

// Search Train
int findTrain(int n)
{
    for (int i = 0; i < tcount; i++)
        if (tname[i] == n) return i;
    return -1;
}

// Find Available Seat
int findSeat(int t)
{
    for (int s = 1; s <= total[t]; s++)
    {
        int used = 0;
        for (int i = 0; i < bcount; i++)
            if (bt[i] == tname[t] && seat[i] == s &&
                status[i] == 1)
                used = 1;

        if (!used) return s;
    }
    return -1;
}

// Display Train
void displayTrains()
{
    for (int i = 0; i < tcount; i++)
        printf("\n%d %s %s-%s %s Seats:%d Fare:%.2f",
               tname[i], name[i], source[i], dest[i],
               time[i], avail[i], fare[i]);
}

// MODULE 1:
void addTrain()
{
    if (tcount == MAX) return;

    printf("Train Number: ");
    scanf("%d", &tname[tcount]);

    if (findTrain(tname[tcount]) != -1)
    {
        printf("Train already exists!\n");
        return;
    }

    printf("Train Name: ");
    scanf(" %49[^\n]", name[tcount]);

    printf("Source: ");
    scanf(" %29[^\n]", source[tcount]);

    printf("Destination: ");
    scanf(" %29[^\n]", dest[tcount]);

    printf("Departure Time (HH:MM): ");
    scanf("%9s", time[tcount]);

    printf("Total Seats Availability: ");
    scanf("%d", &total[tcount]);

    printf("Fare: ");
    scanf("%f", &fare[tcount]);

    if (total[tcount] <= 0 || fare[tcount] < 0)
    {
        printf("Invalid details!\n");
        return;
    }

    avail[tcount] = total[tcount];
    tcount++;

    printf("Train Added!\n");
}

// MODULE 2:
void bookTicket()
{
    int n, t, s;

    if (bcount == MAX || tcount == 0) return;

    displayTrains();

    printf("\nTrain Number: ");
    scanf("%d", &n);

    t = findTrain(n);

    if (t == -1)
    {
        printf("Train not found!\n");
        return;
    }

    printf("Passenger Name: ");
    scanf(" %49[^\n]", passenger[bcount]);

    pnr[bcount] = nextPNR++;
    bt[bcount] = n;

    if (avail[t] > 0)
    {
        s = findSeat(t);
        seat[bcount] = s;
        status[bcount] = 1;
        avail[t]--;

        printf("Confirmed! PNR:%d Seat:%d\n",
               pnr[bcount], s);
    }
    else
    {
        seat[bcount] = 0;
        status[bcount] = 2;

        printf("Waiting List! PNR:%d\n", pnr[bcount]);
    }

    bcount++;
}

// MODULE 3: 
void cancelTicket()
{
    int p, i, t, s;

    printf("Enter PNR: ");
    scanf("%d", &p);

    for (i = 0; i < bcount; i++)
        if (pnr[i] == p) break;

    if (i == bcount || status[i] == 3)
    {
        printf("Invalid or cancelled PNR!\n");
        return;
    }

    t = findTrain(bt[i]);

    if (status[i] == 1)
    {
        status[i] = 3;
        seat[i] = 0;
        avail[t]++;

        for (int j = 0; j < bcount; j++)
            if (bt[j] == bt[i] && status[j] == 2)
            {
                s = findSeat(t);
                if (s != -1)
                {
                    status[j] = 1;
                    seat[j] = s;
                    avail[t]--;
                    printf("Waiting passenger confirmed!\n");
                }
                break;
            }
    }
    else
        status[i] = 3;

    printf("Ticket Cancelled!\n");
}

void search()
{
    int choice, n, i, found = 0;
    char a[30], b[30];

    printf("1.Train Number 2.Route 3.PNR: ");
    scanf("%d", &choice);

    if (choice == 1)
    {
        printf("Train Number: ");
        scanf("%d", &n);

        i = findTrain(n);

        if (i != -1)
        {
            printf("%d %s %s-%s %s Seats:%d Fare:%.2f\n",
                   tname[i], name[i], source[i], dest[i],
                   time[i], avail[i], fare[i]);
            return;
        }
    }
    else if (choice == 2)
    {
        printf("Source: ");
        scanf(" %29[^\n]", a);

        printf("Destination: ");
        scanf(" %29[^\n]", b);

        for (i = 0; i < tcount; i++)
            if (!strcmp(source[i], a) && !strcmp(dest[i], b))
            {
                printf("%d %s %s Seats:%d Fare:%.2f\n",
                       tname[i], name[i], time[i],
                       avail[i], fare[i]);
                found = 1;
            }

        if (found) return;
    }
    else if (choice == 3)
    {
        printf("PNR: ");
        scanf("%d", &n);

        for (i = 0; i < bcount; i++)
            if (pnr[i] == n)
            {
                printf("%d %s Train:%d Seat:%d Status:%d\n",
                       pnr[i], passenger[i], bt[i],
                       seat[i], status[i]);
                return;
            }
    }

    printf("Record not found!\n");
}

// MODULE 4: 
void sortTrains()
{
    int choice, i, j, x, y;
    char temp[50];

    printf("1.Number 2.Time 3.Fare: ");
    scanf("%d", &choice);

    for (i = 0; i < tcount - 1; i++)
        for (j = 0; j < tcount - i - 1; j++)
        {
            x = 0;

            if (choice == 1 && tname[j] > tname[j + 1])
                x = 1;
            else if (choice == 2 &&
                     strcmp(time[j], time[j + 1]) > 0)
                x = 1;
            else if (choice == 3 && fare[j] > fare[j + 1])
                x = 1;

            if (x)
            {
                // Swap all train details
                y = tname[j]; tname[j] = tname[j+1]; tname[j+1] = y;
                y = total[j]; total[j] = total[j+1]; total[j+1] = y;
                y = avail[j]; avail[j] = avail[j+1]; avail[j+1] = y;

                float f = fare[j];
                fare[j] = fare[j+1]; fare[j+1] = f;

                strcpy(temp, name[j]);
                strcpy(name[j], name[j+1]);
                strcpy(name[j+1], temp);

                strcpy(temp, source[j]);
                strcpy(source[j], source[j+1]);
                strcpy(source[j+1], temp);

                strcpy(temp, dest[j]);
                strcpy(dest[j], dest[j+1]);
                strcpy(dest[j+1], temp);

                strcpy(temp, time[j]);
                strcpy(time[j], time[j+1]);
                strcpy(time[j+1], temp);
            }
        }

    displayTrains();
}

void reports()
{
    int choice, i, found = 0;

    printf("0.All 1.Confirmed 2.Waiting 3.Cancelled: ");
    scanf("%d", &choice);

    for (i = 0; i < bcount; i++)
        if (choice == 0 || status[i] == choice)
        {
            printf("\nPNR:%d Name:%s Train:%d Seat:%d Status:%d",
                   pnr[i], passenger[i], bt[i],
                   seat[i], status[i]);
            found = 1;
        }

    if (!found) printf("\nNo records!\n");
}

int main()
{
    int ch;

    do
    {
        printf("\n\n=== RAILWAY RESERVATION ===\n");
        printf("1.Train Details\n");
        printf("2.Ticket Booking\n");
        printf("3.Cancellation and Search\n");
        printf("4.Sorting and Reports\n");
        printf("5.Exit\nChoice: ");
        scanf("%d", &ch);

        switch (ch)
        {
            case 1:
                addTrain();
                displayTrains();
                break;

            case 2:
                bookTicket();
                break;

            case 3:
                printf("1.Cancel 2.Search: ");
                scanf("%d", &ch);
                if (ch == 1) cancelTicket();
                else search();
                break;

            case 4:
                printf("1.Sort Trains 2.Booking Reports: ");
                scanf("%d", &ch);
                if (ch == 1) sortTrains();
                else reports();
                break;

            case 5:
                printf("Thank You!\n");
                break;

            default:
                printf("Invalid choice!\n");
        }

    } 
    while (ch != 5);

    return 0;
}
