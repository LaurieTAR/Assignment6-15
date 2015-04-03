//
//
//
//
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
#include <iomanip>

using namespace std;


struct COORDINATES
{
    int x;
    int y;
a

void createArray(int arr[][100],int r, int c);

void fillArray(int arr[][100], int r, int c, istream &in);

void seed(int sn, ostream &out);

void moveMouse(int &x, int &y);

void printStats(int numdrowned, int numescaped, int numstarved, ostream &out);

void printArray(int arr[][100], int r, int c, ostream &out);

void readArray(int arr[][100], int r, int c, COORDINATES &m, COORDINATES &b1, COORDINATES &b2, ostream &out);


int main(int argc, char**argv)
{
    int numdrowned=0, numescaped=0, numstarved=0, r, c, sn;

    COORDINATES mstart, bridge1, bridge2;
    string ifile, ofile;

    ifstream in;
    ofstream out;

    if(argc != 4)
    {
        cout << "Must specify input file, output file, and seed option." << flush;
        return -1;
    }

    sn = atoi(argv[3]);
    ifile = argv[1];
    ofile = argv[2];

    in.open(ifile.c_str());
    out.open(ofile.c_str());

    if(!in || !out)
    {
        cout << "Cannot open file." << flush;
        return -1;
    }

    out << "Input file: " << ifile << endl; // prints input file
    out << "Output file: " << ofile << endl; // prints output file
    in >> r >> c; // gets row and columns
    seed(sn, out); // finds seed

    int classmap[100][100];
    createArray(classmap, r, c);
    fillArray(classmap, r, c, in); // fill map from file
    printArray(classmap, r, c, out); // print map from file
    int classloc[100][100];
    createArray(classloc, r, c);

    readArray(classmap, r, c, mstart, bridge1, bridge2, out);

    for(int l = 1; l <= 100; l++)
    {
        int x = mstart.x;
        int y = mstart.y;
        int moves = 0;
        bool escape = false, drown = false;
        while(escape == false && drown == false && moves < 100)
        {
            moveMouse(x, y);
            //out << "(" << x << ", " << y << ")" << endl;
            classloc[x][y]++;
            //out << classmap[x][y] << endl;
            if(classmap[x][y] == -1) // drowned
            {
                drown = true;
                numdrowned++;
            }
            else if((x == bridge1.x && y == bridge1.y) || (x == bridge2.x && y == bridge2.y))
            {
                escape = true;
                numescaped++;
            }
            moves++;
        }
        out << "Turn " << l << ": " << flush;
        if(drown == true)
        {
            out << "The mouse has drowned." << flush;
        }
        else if(escape == true)
        {
            out << "It's alive! (crossed the bridge and escaped)" << flush;
        }
        else
        {
            out << "The mouse has wandered too long and has starved to death." << flush;
            numstarved++;
        }
       // out << "(" << x << ", " << y << ")" << endl;
       out << endl;
    }

    printStats(numdrowned, numescaped, numstarved, out);
    printArray(classloc, r, c, out);
    in.close();
    out.close();

    return 0;

}

void createArray(int arr[][100], int r, int c)
{
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            arr[i][j] = 0;
        }
    }
    return;
}

void fillArray(int arr[][100], int r, int c, istream &in)
{
    int pos;
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            in >> pos;
            arr[i][j] = pos;
        }
    }
    return;
}

void readArray(int arr[][100], int r, int c, COORDINATES &m, COORDINATES &b1, COORDINATES &b2, ostream &out)
{
    bool bridge1found = false;
    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            if(arr[i][j] == 1)
            {
                m.x = i;
                m.y = j;
                out << "Start at (" << m.x << ", " << m.y << ")" << endl;
                arr[i][j] = 0;
            }
            else if(arr[i][j] == 0 && (i == 0 || i == (r-1) || j == 0 || j == (c-1)))
            {
                if(bridge1found == false)
                {
                    b1.x = i;
                    b1.y = j;
                    bridge1found = true;
                    out << "Bridge 1 at (" << b1.x << ", " << b1.y << ")" << endl;
                }
                else
                {
                    b2.x = i;
                    b2.y = j;
                    out << "Bridge 2 at (" << b2.x << ", " << b2.y << ")" << endl;
                }
            }
        }
    }
    out << endl;
    return;
}

void seed(int sn, ostream &out)
{
    switch (sn)
    {
    case 1:
        srand(42);
        out << "The RNG was seeded with a fixed number." << endl;
        break;
    case 2:
        srand(time(NULL));
        out << "The RNG was seeded with the time." << endl;
        break;
    default:
        out << "The RNG was not seeded." << endl;
        break;
    }
    out << endl;
    return;
}

void moveMouse(int &x, int &y)
{
    int posarray[2][4] = {{-1, 0, 1, 0}, {0, 1, 0, -1}};
    int d = rand() % 4;
    x += (posarray[0][d]);
    y += (posarray[1][d]);
    return;
}

void printStats(int numdrowned, int numescaped, int numstarved, ostream &out)
{
    out << endl;
    out << "The mouse escaped " << numescaped << " times." << endl;
    out << "The mouse drowned " << numdrowned << " times." << endl;
    out << "The mouse starved " << numstarved << " times." << endl;
    out << endl;
}

void printArray(int arr[][100], int r, int c, ostream &out)
{
    int templarg = 0;
    bool neg = false;
    for(int i = 0; i < r; i++) // find largest number
    {
        for(int j = 0; j < c; j++)
        {
            if(templarg < arr[i][j])
            {
                templarg = arr[i][j];
            }
            if(arr[i][j] < 0)
            {
                neg = true;
            }
        }
    }
    int digits = 1;
    while(templarg /= 10) // find digits
    {
        digits ++;
    }
    if(neg == true && digits == 1) //add space for negative numbers
    {
        digits ++;
    }

    for(int i = 0; i < r; i++)
    {
        for(int j = 0; j < c; j++)
        {
            out << setw(digits) << arr[i][j] << ' ';
        }
        out << endl;
    }

    out << endl;
    return;
}
/*
parse command line parameters, open files, read size of maps
    check for correct number of files
    check input
    check output
    getline
    in >> r >> c
if needed, seed the random number generator
    no seed - do nothing
    seed w/ number - srand
    seed w time, include ctime, srand time
create the maps (will 0-fill the arrays)
    Classmap(r, c)
    classloc(r, c)
read the map && save initial position of the mouse and bridges
mx = ?
my = ?
br1x = ?
br1y = ?
br2x = ?
br2y = ?


for 100 lives (int l = 0; l <100; l++)
    put mouse at starting point on map
        int x = mx
        int y = my
    loop for 100 moves or until escaped or drowned
    (while incr variable < 100 OR drowned = true OR survived = true)
        move mouse FUNCTION
        (rand gen N, S, W, or E)
        (access direction array)
        X + (array[D][0])
        Y + (array[D][1])
        count move to target square
        (add number to class at location)
        if( drowned ) (if array[x][y] = -1)
            handle drowning
            (drowned = true)
            (drownum ++)
        else if( escaped ) if x, y = bridge1 or bridge2
            handle escaping
            (escaped = true)
            (escapnum ++)
        end if
    end loop for 100 moves, etc
    if( starved )
        handle starving
        (starvnum ++)
    end if
end for 100 lives
print statistics CHECK
(out starved, drowned, escaped)
print count map CHECK
(print array, look at spiral)
*/

