#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <math.h>
#include <time.h>

#define N 9

using namespace std;

//Example sudokus to solve
int problem1[N][N] = {
   {3, 0, 6, 5, 0, 8, 4, 0, 0},
   {5, 2, 0, 0, 0, 0, 0, 0, 0},
   {0, 8, 7, 0, 0, 0, 0, 3, 1},
   {0, 0, 3, 0, 1, 0, 0, 8, 0},
   {9, 0, 0, 8, 6, 3, 0, 0, 5},
   {0, 5, 0, 0, 9, 0, 6, 0, 0},
   {1, 3, 0, 0, 0, 0, 2, 5, 0},
   {0, 0, 0, 0, 0, 0, 0, 7, 4},
   {0, 0, 5, 2, 0, 6, 3, 0, 0}
};

int problem2[N][N] = {
   {0, 0, 4, 9, 6, 2, 3, 0, 0},
   {0, 6, 0, 1, 0, 0, 4, 0, 0},
   {8, 2, 0, 3, 7, 0, 0, 0, 6},
   {0, 0, 1, 4, 0, 6, 0, 2, 0},
   {0, 0, 2, 7, 5, 0, 0, 0, 0},
   {0, 9, 3, 2, 0, 0, 7, 0, 4},
   {2, 7, 0, 0, 3, 0, 9, 4, 0},
   {1, 0, 0, 0, 0, 0, 2, 7, 5},
   {9, 0, 0, 8, 2, 0, 0, 0, 1}
};

int problem3[N][N] = {
   {8, 0, 0, 0, 0, 0, 0, 0, 0},
   {0, 3, 6, 0, 0, 0, 0, 0, 0},
   {0, 7, 0, 0, 9, 0, 2, 0, 0},
   {0, 5, 0, 0, 0, 7, 0, 0, 0},
   {0, 0, 0, 0, 4, 5, 7, 0, 0},
   {0, 0, 0, 1, 0, 0, 0, 3, 0},
   {0, 0, 1, 0, 0, 0, 0, 6, 8},
   {0, 0, 8, 5, 0, 0, 0, 1, 0},
   {0, 9, 0, 0, 0, 0, 4, 0, 0}
};

int problem4[N][N] = {
   {0, 0, 0, 0, 0, 1, 2, 0, 0},
   {0, 0, 0, 0, 0, 0, 3, 4, 0},
   {0, 0, 0, 0, 0, 0, 0, 5, 6},
   {0, 0, 0, 0, 0, 0, 0, 0, 7},
   {0, 0, 0, 0, 0, 0, 0, 0, 0},
   {1, 0, 0, 0, 0, 0, 0, 0, 0},
   {7, 2, 0, 0, 0, 0, 0, 0, 0},
   {0, 4, 8, 0, 0, 0, 0, 0, 0},
   {0, 0, 6, 3, 0, 0, 0, 0, 0}
};

//Functions to print useful data structures:
void printSudoku(int sudoku[9][3][3])
//Prints sudoku board (in processed format)
{
    cout << endl;
    for(int l=0;l<3;l++)
    {
        for(int j=0;j<3;j++)
        {
            for(int i=3*l;i<3*l+3;i++)
            {
                for(int k=0;k<3;k++)
                {
                    cout << sudoku[i][j][k] << " ";
                }
                cout << "| ";
            }
            cout << endl;
        }
        cout << "_ _ _   _ _ _   _ _ _" << endl << endl;
    }
}
void printCoords(array<vector<pair<int, int>>,9> coords)
//Print list of fixed coordinates (useful for testing).
{
    for(size_t i = 0; i < 9; i++)
    {
        cout << "Array element num: " << i << endl;
        // get the iterator of the specific array element
        for (vector <pair<int,int> >::const_iterator iter = coords[i].begin();
            iter != coords[i].end();
            ++iter)
        {
            cout << "j= "    << iter->first
                << ", i= " << iter->second << endl;
        }
    }
}

//Functions to preprocess the sudoku problem into array filled with random 
//values, where each block has unique 'unfixed' values 1-9,
//and corresponding array of vector coordinates of the unfixed values:
void findFixedValues(int problem[N][N],int fixedValues[N][N])
//fill corresponding array fixedvalues to mark values fixed (1) or unfixed (0)
{
    for(int i =0; i < N; i++)
    {
        for(int j=0;j<N;j++)
        {
            if(problem[i][j] == 0)
            {
                fixedValues[i][j] = 0;
            } else {
                fixedValues[i][j] = 1;
            }
        }
    }
}
void blockSelect(int problem[N][N],int sudoku[9][3][3],int fixedValues[9][3][3])
//Form sudoku data struct. Array representing 9 'blocks' [9], with 3x3 coords [3][3].
//Also store a seperate array marking if values are fixed(1) or unfixed(0).
{   
    int fixedValues0[N][N]; 
    findFixedValues(problem,fixedValues0);
    for(int i=0;i<9;i++)
    {
        for(int j=0;j<3;j++)
        {
            for(int k=0;k<3;k++)
            {   
                if(i<3){
                    sudoku[i][j][k] = problem[j][k+3*i];
                    fixedValues[i][j][k] = fixedValues0[j][k+3*i];
                } if(i>= 3 && i<6){
                    sudoku[i][j][k] = problem[j+3][k+3*(i-3)];
                    fixedValues[i][j][k] = fixedValues0[j+3][k+3*(i-3)];
                } if(i>=6){
                    sudoku[i][j][k] = problem[j+6][k+3*(i-6)];
                    fixedValues[i][j][k] = fixedValues0[j+6][k+3*(i-6)];
                }else{}
            }
        }
    }
}
void findMissing(int arr[], int n, int low, int high,vector <int>& missing)
//Finds the missing numbers from a list:
{ 
    // Sort array
    sort(arr, arr + n); 
    // Binary search for the lowest
    int* ptr = lower_bound(arr, arr + n, low); 
    int index = ptr - arr; 
    int i = index, x = low; 
    int y = 0;
    //Search up to highest value in list
    while (i < n && x <= high) { 
        if (arr[i] != x) {
            missing.push_back(x);
            y++;
        } else{
            i++; }
        x++; 
    } 
    // search past highest value in list.
    while (x <= high) 
    {
        missing.push_back(x);
        x++;
        y++;
    }
} 
int removeAt(vector<int>& vec, int n)
//Given vector index, remove from the vector and return value.
{
    int value = vec.at(n);
    swap(vec.at(n), vec.back());
    vec.pop_back();
    return value;
}
void zeroCheck(int blockFixedValues[3][3],vector<int>& vec)
//Fill vector with the coordinates of the fixed entries (ones)
{
    for(int k=0;k<3;k++)
    {
        for(int l=0;l<3;l++)
        {   
            if(blockFixedValues[k][l]==0){
                vec.push_back(k);
                vec.push_back(l);
            }else{}
        }
    }
}
void blockFiller(int block[3][3],vector <int>& missing,vector<int>& indexes)
//recursively fill a block with random numbers supplied- so that each block 
//contains set [1,9].
{
    if((missing.size()==1)){
        int v = removeAt(missing,0);
        int j= indexes.back();
        indexes.pop_back();
        int i = indexes.back();
        block[i][j] = v;
        return ;
    } else {
        //srand((unsigned int)time(NULL));
        int index = rand() % (missing.size()-1);
        int v = removeAt(missing,index);

        int j= indexes.back();
        indexes.pop_back();
        int i = indexes.back();
        indexes.pop_back();
        block[i][j] = v;
        blockFiller(block,missing,indexes);
        return;
    }
}
void fillBlocks(int sudoku[N][3][3],int fixedValues[N][3][3])
//finds the missing values in a block and locations, passes missing value 
//to blockFiller to perform filling operation
{   
    for(int b=0;b<N;b++)
    {
        int values[9];
        int fixedmarkers[9];
        for(int i=0;i<3;i++)
        {
            for(int j=0;j<3;j++)
            {   
                values[3*i+j] = sudoku[b][i][j];
                fixedmarkers[3*i+j] = fixedValues[b][i][j];
            }
        }
        int m = count(fixedmarkers,fixedmarkers+9,0);
        int n = sizeof(values) / sizeof(values[0]);

        vector<int> missing(0);
        findMissing(values,n,1,9,missing);

        vector<int> indexes(0);
        zeroCheck(fixedValues[b],indexes);

        blockFiller(sudoku[b],missing,indexes);
    }
}
void unfixedCoords(int fixedValues[N][3][3],array<vector<pair<int, int>>,9> &coords)
//Fill an array of vectors of pairs with the coordinates of the unfixed entries (zeros)
{
    for(int n=0;n<9;n++)
    {
        for(int j=0;j<3;j++)
        {
            for(int i=0;i<3;i++)
            {
                if(fixedValues[n][j][i]==0)
                {
                    coords[n].push_back({j,i});
                }
            }
        }
    }
}


//Functions to compute the score of the sudoku value
//Score is defined as number of repeats in each collumn/row.
int countMissing(unordered_set<int> s, int low, int high) 
//Counts missing numbers in set between low-high
{ 
    int m = 0;
    for (int x = low; x <= high; x++) 
        if (s.find(x) == s.end()) 
            m++;
    return m;
} 
int rowScore(int sudoku[N][3][3],int n,int j)
//Computes score along row given by elements [n][j][*]
{
    int score = 0;
    unordered_set<int> set;
    int index = n/3;
    for(int i=3*index;i<3*index+3;i++)
        for(int k=0;k<3;k++)
            set.insert(sudoku[i][j][k]);
    score = countMissing(set,1,9);
    return score;
}
int collumnScore(int sudoku[N][3][3],int n,int i)
//computes score along collumn given by elements [n][*][i]
{
    int score = 0;
    unordered_set<int> set;
    int start_index = n%3;
    for(int j=0;j<3;j++)
            for(int k=0;k<3;k++)
                set.insert(sudoku[start_index+3*j][k][i]);
    score = countMissing(set,1,9);
    return score;
}
int scoreBoard(int sudoku[N][3][3])
//Compute score for the entire board
{
    int score = 0;
    for(int n=0;n<3;n++)
        {
            for(int i=0;i<3;i++)
            {
                score += rowScore(sudoku,4*n,i);
                score += collumnScore(sudoku,4*n,i);
            }
        }
    return score; 
}

//Functions to swap two random elements in a block
template<class bidiiter>
//fisher-yates shuffle- used to pick two random elements from coords list.
bidiiter random_unique(bidiiter begin, bidiiter end, size_t n) {
    size_t left = std::distance(begin, end);
    while (n--) 
    {
        bidiiter r = begin;
        advance(r, rand()%left);
        swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}
void swapper(int sudoku[N][3][3],vector <pair<int,int>> indexes,int n)
//swap two given indexes in block n
{
    swap(sudoku[n][indexes.begin()->first][indexes.begin()->second],
            sudoku[n][next(indexes.begin())->first][next(indexes.begin())->second]);
}

int scorer(int sudoku[N][3][3],vector <pair<int,int>> &indexes, int n)
//Compute the score across 2 given elements (index pairs), ignore if 
//same index as swapping gives net 0 score change
{   int score = 0;
    if (indexes.begin()->first != next(indexes.begin())->first)
    {
        score += rowScore(sudoku,n,indexes.begin()->first);
        score += rowScore(sudoku,n,next(indexes.begin())->first);
    }
    if (indexes.begin()->second != next(indexes.begin())->second)
    {
        score += collumnScore(sudoku,n,indexes.begin()->second);
        score += collumnScore(sudoku,n,next(indexes.begin())->second);
    }
    return score;
}
float probFunction(int deltaScore,float T)
//Change this to define a new cost function
{
    float prob = 1000/(1+exp(deltaScore/T));
    return prob;
}
int permutations(int sudoku[N][3][3],array<vector<pair<int, int>>,9> coords,
        int initialScore,float T,bool &improved)
//Randomly flips two elements in a block and computes new score.
//Accepts if score improves, uses probFunction to choose accept/reject if not
{
    int n = rand() % 9; //choose random block
    random_unique(coords[n].begin(),coords[n].end(),2); //choose two random coordiantes of elements in block
    vector <pair<int,int>> indexes = {coords[n].begin(),coords[n].end()};
    int deltaScore = -scorer(sudoku,indexes,n); //calc. score before flip
    swapper(sudoku,indexes,n); //flip elements
    deltaScore += scorer(sudoku,indexes,n); //calc. score change after flip
    if(deltaScore<0)
    //if score improves, accept change
    {
        improved = true;
        return deltaScore+initialScore; 
    }
    else
    {
    //if score worsens/same, use prob dist function to choose accept or reject.
        improved = false;
        float prob = probFunction(deltaScore,T);
        float r = rand() %1000;
        if(prob>r)
            {
                return deltaScore+initialScore;
            }
        else
            {   
                swapper(sudoku,indexes,n); //swap back if reject
                return initialScore;
            }
    }
}
float coolingFunc(float T, float a)
//Function describes cooling schedule of T after each iteration.
{
    T = T-T*a;
    return T;
}
void Annealer(int sudoku[N][3][3],array<vector<pair<int, int>>,9> coords,int score,
                    float T0,float endT,float a)
//Main loop for Simulated Annealing
{
    int count = 0;
    float T = T0;
    bool improved; //did previous iteration improve score  
    int reheatCount = 0;    //count successive times score doesn't increase
    while(T>endT && count<1e6)
    {
        if(reheatCount==1e4)
        //If score doesn't improve after 10000 iterations, return temp to T0
        //(helps avoid being stuck at local minima)
        {
            T = T0;
            reheatCount = 0;
            cout << "Reheated! at iteration "<< count <<endl;
        }

        //call permutation operation and update score
        score = permutations(sudoku,coords,score,T,improved);

        //reset/increase reheat score
        if(improved==false)
                reheatCount++;
        else
                reheatCount = 0;

        //Advance Temp using coolingFunc
        T =coolingFunc(T,a);

        if(score == 0)
        //if score=0, solution has been solved -break loop
            {
                printSudoku(sudoku);
                cout << "Solution found!"<<endl;
                cout << "Ending temp T = " << T << endl;
                cout << "Total iterations = " << count << endl;
                return ;
            }
        count++;

        if(count%10000==0)
        {
            cout << "at count "<< count <<" score is "<<score <<endl;
        }
    }
    //If loop ends without reaching score=0, print final iteration sudoku/score
    printSudoku(sudoku);
    cout << "no solution found, count was"<< count<<endl;
    cout << "final score = "<< score << endl;
    return ;
}

//Functions to check problem is valid for solving
int countRepeats(vector<int> vec)
//Count number of repeats in a vector
{
    sort(vec.begin(), vec.end());
    int l1 = vec.size();
    vec.erase( unique( vec.begin(), vec.end() ), vec.end() );
    int score =  l1-vec.size();
    return score;
}
bool checkValidity(int sudoku[N][3][3])
    //Check the problem has no fixed repeats in a row, collumn or a block 
    //and contains only numbers in range 1-9
{
    int n=0;
    int score =0;
    vector<int> vec;
    for(int n=0;n<N;n++)
    {
        int index = n/3;
        for(int j=0;j<3;j++)
        {
            for(int i=3*index;i<3*index+3;i++)
                for(int k=0;k<3;k++)
                    if(sudoku[i][j][k]>=1 && sudoku[i][j][k]<=9)
                        vec.push_back(sudoku[i][j][k]);
                    else if(sudoku[i][j][k]!=0)
                    {
                        cout << "Not Valid! Not allowed value of "<<sudoku[i][j][k]
                                    << " in block number "<< i<<endl;
                        return false; 
                    }
            score += countRepeats(vec);
            if(score>0)
            {
                cout<<"Not Valid! Repeated value in row num "<< j
                            <<" along block num "<< n<<endl;;
                return false;
            }
            vec.clear();
        }
        int start_index = n%3;
        for(int o=0;o<3;o++)
        {
            for(int p=0;p<3;p++)
                for(int q=0;q<3;q++)
                    if(sudoku[start_index+3*p][q][o] !=0)
                        vec.push_back(sudoku[start_index+3*p][q][o]);
            score += countRepeats(vec);
            if(score>0)
            {
                cout<<"Not Valid! Repeated value in collumn num "<< o
                            <<" along block num "<< n <<endl;;
                return false;
            }
            vec.clear();
        }
        for(int r=0;r<3;r++)
            for(int s=0;s<3;s++)
                if(sudoku[n][r][s] !=0)
                    vec.push_back(sudoku[n][r][s]);
        score += countRepeats(vec);
        if(score>0)
        {
            cout<<"Not Valid! Repeated value in block num "<< n << endl;
            return false;
        }
        vec.clear();
    }
    return true;
}
bool preProcess(int problem[N][N],int sudoku[N][3][3],array<vector<pair<int, int>>,9> &coords)
// Preprocess the problem and check validity
{
    int fixedValues[N][3][3];
    blockSelect(problem4,sudoku,fixedValues);

    bool test;
    test = checkValidity(sudoku);

    if(test==true)
    {
        cout << "Problem to be solved:" << endl;
        printSudoku(sudoku);
        //fill each sudoku block with numbers 1-9
        fillBlocks(sudoku,fixedValues);
        //store coordinates of unfixed coordinates as array (for each block 1-9) 
        //of vectors of each coordiante pair
        unfixedCoords(fixedValues,coords);  
        return true;
    }
    return false;
}

int main()
{
    const clock_t begin_time = clock();
    //preprocess array and check it is a valid problem
    int sudoku[N][3][3];
    array<vector<pair<int, int>>,9> coords;
    bool valid;

    //
    //Input problem to be solved here:
    valid = preProcess(problem1,sudoku,coords);
    //
    if(valid==false)
        return 0;

    //Cooling Parameters
    float T = 2;
    float endT = 0.001;
    float a = 1e-3;

    int score = scoreBoard(sudoku); //board starting score
    //Run annealing algorithm and print time taken
    Annealer(sudoku,coords,score,T,endT,a);   
    cout << "Time taken = "<< float( clock () - begin_time ) /  CLOCKS_PER_SEC <<" seconds";
}
