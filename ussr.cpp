#include<iostream>
#include<vector>
#include<fstream>
#include<algorithm>
#include<windows.h>

using namespace std;

//!Some shit to make colors work
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
WORD saved_attributes;
//!End of shit

struct field
{
    vector<vector<int> > F;
    field()
    {
        for (int j = 0; j < 8; j++)
        {
            vector<int> A(8);
            for (int i = 0; i < 8; i++)
            {
                A[i] = 0;
            }
            F.push_back(A);
        }
    }
};

struct impactzone
{
    vector<vector<int> > I;
    impactzone()
    {
        for (int j = 0; j < 8; j++)
        {
            vector<int> A(8);
            for (int i = 0; i < 8; i++)
            {
                A[i] = 0;
            }
            I.push_back(A);
        }
    }
};

impactzone operator + (impactzone &A, impactzone &B)
{
    impactzone C;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            C.I[i][j] = A.I[i][j] + B.I[i][j];
        }
    }
    return C;
}

/*struct figs
{
    int f;
    static const int king = 10;
    static const int queen = 11;
    static const int rook = 12;  //ladya
    static const int knight = 13;//horse
    static const int bishop = 14;//oficer

};
*/
struct turn
{
    int x, y;
    turn()
    {
        x = 0;
        y = 0;
    }
};

struct game
{
    field F;
    turn C[5];
    vector<turn> T;
    vector<int> R;
    vector<impactzone> G;
    //vector<impactzone> P;
    int A[5];
    game(field FF)
    {
        F = FF;
        int k = 0;
        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                if (F.F[i][j] != 0)
                {
                    C[k].x = i;
                    C[k].y = j;
                    k++;
                }
            }
        }
        //A[5];
        for (int i = 0; i < 5; i++)
        {
            A[i] = i + 10;
        }
    }
};

istream & operator >> (istream & din, field &F)
{
    for (int i = 0; i < 5; i ++)
    {
        int x, y;
        cin >> x >> y;
        F.F[x][y] = -1;
    }
    return din;
}

ostream & operator << (ostream & dout, field &F)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            cout << F.F[i][j] << " ";
        }
        cout << endl;
    }
    return dout;
}

void color(string S)
{
        /* Save current attributes */
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    saved_attributes = consoleInfo.wAttributes;
    //SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY);
    SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
    cout << S;

    /* Restore original attributes */
    SetConsoleTextAttribute(hConsole, saved_attributes);
}

ostream & operator << (ostream & dout, impactzone A)
{
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (A.I[i][j] == 10)
                color("K ");
            else if (A.I[i][j] == 11)
                color("Q ");
            else if (A.I[i][j] == 12)
                color("R ");
            else if (A.I[i][j] == 13)
                color("H ");
            else if (A.I[i][j] == 14)
                color("B ");
            else
                cout << A.I[i][j] << " ";
        }
        cout << endl;
    }
    return dout;
}

bool isgood (int x, int y, field &F)
{
    if ((x > 7)||(x < 0)||(y > 7)||(y < 0)||(F.F[x][y] != 0))
    {
        return 0;
    }
    return 1;
}

impactzone imzking (int x, int y, field &F)
{
    impactzone A;
    for (int dx = -1; dx < 2; dx++)
    {
        for (int dy = -1; dy < 2; dy++)
        {
            int g = isgood(x + dx, y + dy, F);
            if (!g)
            {
                continue;
            }
            A.I[x + dx][y + dy] = g;
        }
    }
    return A;
}

impactzone imzrook (int x, int y, field &F)
{
    impactzone A;
    for (int i = x + 1; i < 8; i++)
    {
        int g = isgood(i, y, F);
        if (!g)
        {
            break;
        }
        A.I[i][y] = g;
    }
    for (int i = x - 1; i >= 0; i--)
    {
        int g = isgood(i, y, F);
        if (!g)
        {
            break;
        }
        A.I[i][y] = g;
    }
    for (int i = y - 1; i >= 0; i--)
    {
        int g = isgood(x, i, F);
        if (!g)
        {
            break;
        }
        A.I[x][i] = g;
    }
    for (int i = y + 1; i < 8; i++)
    {
        int g = isgood(i, y, F);
        if (!g)
        {
            break;
        }
        A.I[x][i] = g;
    }
    return A;
}

impactzone imzbishop (int x, int y, field &F)
{
    impactzone A;
    for (int d = 1; d < 8; d++)
    {
        bool g = isgood(x + d, y + d, F);
        if (!g)
        {
            break;
        }
        A.I[x + d][y + d]++;
    }
    for (int d = 1; d < 8; d++)
    {
        bool g = isgood(x + d, y - d, F);
        if (!g)
        {
            break;
        }
        A.I[x + d][y - d]++;
    }
    for (int d = 1; d < 8; d++)
    {
        bool g = isgood(x - d, y + d, F);
        if (!g)
        {
            break;
        }
        A.I[x - d][y + d]++;
    }
    for (int d = 1; d < 8; d++)
    {
        bool g = isgood(x - d, y - d, F);
        if (!g)
        {
            break;
        }
        A.I[x - d][y - d]++;
    }
    return A;
}

impactzone imzqueen(int x, int y, field &F)
{
    impactzone A = imzrook(x, y, F);
    impactzone B = imzbishop(x, y, F);
    A = A + B;
    return A;
}

impactzone imzknight(int x, int y, field &F)
{
    impactzone A;
    int g = isgood(x + 2, y + 1, F);
    if (g) {A.I[x + 2][y + 1]++;}

    g = isgood(x + 2, y - 1, F);
    if (g) {A.I[x + 2][y - 1]++;}

    g = isgood(x - 2, y - 1, F);
    if (g) {A.I[x - 2][y - 1]++;}

    g = isgood(x - 2, y + 1, F);
    if (g) {A.I[x - 2][y + 1]++;}

    g = isgood(x + 1, y - 2, F);
    if (g) {A.I[x + 1][y - 2]++;}

    g = isgood(x + 1, y + 2, F);
    if (g) {A.I[x + 1][y + 2]++;}

    g = isgood(x - 1, y - 2, F);
    if (g) {A.I[x - 1][y - 2]++;}

    g = isgood(x - 1, y + 2, F);
    if (g) {A.I[x - 1][y + 2]++;}
    return A;
}

impactzone figs(int f, int x, int y, field &F)
{
    static const int king = 10;
    static const int queen = 11;
    static const int rook = 12;  //ladya
    static const int knight = 13;//horse
    static const int bishop = 14;//oficer
    impactzone R;
    if (f == king) {R = imzking(x, y, F);}
    if (f == queen) {R = imzqueen(x, y, F);}
    if (f == rook) {R = imzrook(x, y, F);}
    if (f == knight) {R = imzknight(x, y, F);}
    if (f == bishop) {R = imzbishop(x, y, F);}
    return R;
}

void think (game &G)//Буду пока считать, что все работаетж
{
    impactzone T;
    impactzone A;
    int x, y;
    for (int i = 0; i < 120; i++)
    {
        G.G.push_back(A);
        for (int j = 0; j < 5; j++)
        {
            x = G.C[j].x;
            y = G.C[j].y;
            T = figs(G.A[j], x, y, G.F);
            G.G[i] = G.G[i] + T;
            G.G[i].I[x][y] = G.A[j];//в импактзоне показывают где какие фигуры
            //cout << G.A[j] << " "; //вроде работает перестановки выводятся
        }
        //cout << endl;//зоны -- нет
        next_permutation(G.A, G.A + 5);
    }
    //G.P = G.G;
}

//impactzone give_answer(game &G)

int eval_turn (turn t, game &G)
{
    int A[6];
    int a = isgood(t.x, t.y, G.F);
    if (!a)
    {
        return 1000;
    }
    for (int i = 0; i < 6; i++)
    {
        A[i] = 0;
    }
    for (int i = 0; i < G.G.size(); i++)
    {
        a = G.G[i].I[t.x][t.y];
        A[a]++;
    }
    a = 0;
    for (int i = 0; i < 6; i++)
    {
        if (A[i] > a)
        {
            a = A[i];
        }
    }
    return a;
}

void update (int a, turn t, game &G)
{
    vector<impactzone> N;
    for (int i = 0; i < G.G.size(); i++)
    {
        if (G.G[i].I[t.x][t.y] == a)
        {
            N.push_back(G.G[i]);
        }
    }
    G.G = N;
    cout << "*" << N.size() << endl;
    if (N.size() == 1)
    {
        cout << endl << N[0] << endl;
    }
}

turn greed(game &G)
{
    turn t;
    turn r;
    int m = 100;
    int a = 0;
    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            t.x = i;
            t.y = j;
            //cout << i << " " << j << endl;
            a = eval_turn(t, G);
            if (m > a)
            {
                m = a;
                r = t;
            }
        }
    }
    return r;
}

int main()
{
    //color("SOnya");


    while (1)
    {
        field F;
        turn t;
        int a;
        cin >> F;
        cout << endl << F;
        game G(F);
        cout << "lexa" << endl;
        think(G);
        cout << "lexa" << endl;
        while(1)
        {
            t = greed(G);
            cout << t.x << " " << t.y << endl;
            cin >> a;
            if (a == -1)
            {
                break;
            }
            update(a, t, G);
        }
    }
}
