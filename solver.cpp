/*
challenger1229 :)
*/
#include<bits/stdc++.h>
using namespace std;

#define int long long

bool ONLINE_MODE = false;
bool DEBUG_MODE = false;
int NUMBER_OF_CASES = 1;

vector< tuple< vector<vector<int>>, vector<pair<int, int>> > > TEST_CASES = {
    { 
        { 
            {0, 0, 0, 0, 0, 0, 1, 1}, 
            {0, 0, 0, 2, 3, 1, 1, 1},    
            {0, 0, 2, 2, 3, 3, 1, 1}, 
            {0, 4, 4, 4, 3, 3, 3, 1}, 
            {0, 5, 5, 5, 6, 6, 6, 1}, 
            {0, 7, 5, 5, 6, 6, 7, 1},
            {7, 7, 7, 5, 6, 7, 7, 1},
            {7, 7, 7, 7, 7, 7, 1, 1}  
        }, 
        { 
            {0, 0}
        } 
    }
};

void displayMatrix(vector<vector<int>> &colorCompositionOfMatrix, vector<pair<int, int>> &placedQueensPositions, string displayMessage){
    
    int numberOfRows = colorCompositionOfMatrix.size();
    int numberOfColumns = colorCompositionOfMatrix[0].size();
    
    string rowString = " ";
    for(int columnNumber = 0; columnNumber<numberOfColumns; columnNumber++){
        rowString+="O ";
    }
    vector<string> displayOutput(numberOfRows, rowString);
    
    for(int positionIndex =0; positionIndex<placedQueensPositions.size(); positionIndex++){
        displayOutput[placedQueensPositions[positionIndex].first][(placedQueensPositions[positionIndex].second+1)*2-1] = 'X';
    }
    
    cout<<displayMessage<<endl;
    cout<<endl;
    for(int rowIndex = 0; rowIndex < displayOutput.size(); rowIndex++){
        cout<<displayOutput[rowIndex]<<endl<<endl;
    }
    cout<<endl;
    
}

void addAffectedCells( vector<vector<int>> &blockedPositionsMatrix, pair<int, int> &cell ){
    
    int numberOfRows = blockedPositionsMatrix.size();
    int numberOfColumns = blockedPositionsMatrix[0].size();
    
    // populate cells in the same column
    for(int i=0; i<numberOfRows; i++){
        if(i==cell.first) continue;
        blockedPositionsMatrix[i][cell.second]+=1;
    }
    
    // populate cells in the same row
    for(int i=0; i<numberOfColumns; i++){
        if(cell.second==i) continue;
        blockedPositionsMatrix[cell.first][i]+=1;
    }
    
    bool isInLastRow = (cell.first==(numberOfRows-1));
    bool isInFirstRow = (cell.first==0);
    bool isInLastColumn = (cell.second==(numberOfColumns-1));
    bool isInFirstColumn = (cell.second== 0);
    
    // populate diagonally adjancent cells
    if((not isInFirstRow) and (not isInFirstColumn)){
        blockedPositionsMatrix[cell.first-1][cell.second-1]+=1;
    }
    
    if((not isInFirstRow) and (not isInLastColumn)){
        blockedPositionsMatrix[cell.first-1][cell.second+1]+=1;
    }
    
    if((not isInLastRow) and (not isInFirstColumn)){
        blockedPositionsMatrix[cell.first+1][cell.second-1]+=1;
    }
    
    if((not isInLastRow) and (not isInLastColumn)){
        blockedPositionsMatrix[cell.first+1][cell.second+1]+=1;
    }
    
}

void restoreAffectedCells( vector<vector<int>> &blockedPositionsMatrix, pair<int, int> &cell ){
    
    int numberOfRows = blockedPositionsMatrix.size();
    int numberOfColumns = blockedPositionsMatrix[0].size();
    
    // populate cells in the same column
    for(int i=0; i<numberOfRows; i++){
        if(i==cell.first) continue;
        blockedPositionsMatrix[i][cell.second]-=1;
    }
    
    // populate cells in the same row
    for(int i=0; i<numberOfColumns; i++){
        if(cell.second==i) continue;
        blockedPositionsMatrix[cell.first][i]-=1;
    }
    
    bool isInLastRow = (cell.first==numberOfRows-1);
    bool isInFirstRow = (cell.first==0);
    bool isInLastColumn = (cell.second==numberOfColumns-1);
    bool isInFirstColumn = (cell.second== 0);
    
    // populate diagonally adjancent cells
    if((not isInFirstRow) and (not isInFirstColumn)){
        blockedPositionsMatrix[cell.first-1][cell.second-1]-=1;
    }
    
    if((not isInFirstRow) and (not isInLastColumn)){
        blockedPositionsMatrix[cell.first-1][cell.second+1]-=1;
    }
    
    if((not isInLastRow) and (not isInFirstColumn)){
        blockedPositionsMatrix[cell.first+1][cell.second-1]-=1;
    }
    
    if((not isInLastRow) and (not isInLastColumn)){
        blockedPositionsMatrix[cell.first+1][cell.second+1]-=1;
    }
    
}

void backTracking(vector<vector<int>> &blockedPositionsMatrix, bool &solutionObtained, vector<pair<int,int>> &solution, map<int, vector<pair<int, int>>> &cellsForColor){
    
    if(DEBUG_MODE){
        displayMatrix(blockedPositionsMatrix, solution, " test : ");
    }
    // all colors have been handled
    if(cellsForColor.size() == 0){
        solutionObtained = true;
        return;
    }
    
    
    // obtain first key
    int firstColor = cellsForColor.begin()->first;
    vector<pair<int, int>> cellsForFirstColor = cellsForColor[firstColor];
    cellsForColor.erase(firstColor);
    

    
    for(auto cell: cellsForFirstColor){
        
        if(blockedPositionsMatrix[cell.first][cell.second]){
            if (DEBUG_MODE){
                        cout<<cell.first<<" "<<cell.second<<endl;
                    }
            continue;
        }
        for(auto cell: cellsForFirstColor){
            blockedPositionsMatrix[cell.first][cell.second] += 1;
        }
        solution.push_back(cell);
        addAffectedCells( blockedPositionsMatrix, cell);
        backTracking(blockedPositionsMatrix, solutionObtained, solution, cellsForColor);
        if(solutionObtained){
            return;
        }
        solution.pop_back();
        for(auto cell: cellsForFirstColor){
            blockedPositionsMatrix[cell.first][cell.second] -= 1;
        }
    
        restoreAffectedCells( blockedPositionsMatrix, cell);
    }
    

    cellsForColor[firstColor] = cellsForFirstColor;
    
}



vector<pair<int, int>> solveThis(vector<vector<int>> &colorCompositionOfMatrix, vector<pair<int, int>> &originalPlacedQueensPositions){
    
    int numberOfRows = colorCompositionOfMatrix.size();
    int numberOfColumns = colorCompositionOfMatrix[0].size();
    
    vector<vector<int>> blockedPositionsMatrix(numberOfRows, vector<int>(numberOfColumns, 0));
    bool solutionObtained = false;
    vector<pair<int,int>> solution; 
    map<int, vector<pair<int, int>>> cellsForColor;
    
    for(int row = 0; row<numberOfRows; row++){
        for(int column = 0; column<numberOfColumns; column++){
            cellsForColor[colorCompositionOfMatrix[row][column]].push_back({row, column});
        }
    }
    
    // cout<<cellsForColor.size()<<endl;
    for(int placedQueenPositionIndex = 0; placedQueenPositionIndex < originalPlacedQueensPositions.size(); placedQueenPositionIndex++ ){
        if(DEBUG_MODE){
                cout<<"non zero queens"<<endl;
            }
        int colorForTheQueensPosition = colorCompositionOfMatrix[originalPlacedQueensPositions[placedQueenPositionIndex].first][originalPlacedQueensPositions[placedQueenPositionIndex].second];
        vector<pair<int, int>> cellsForColorForTheQueensPosition = cellsForColor[colorForTheQueensPosition];
        cellsForColor.erase(colorForTheQueensPosition);
        
        for(auto cell: cellsForColorForTheQueensPosition){
            blockedPositionsMatrix[cell.first][cell.second] += 1;
        }
        
        addAffectedCells( blockedPositionsMatrix, originalPlacedQueensPositions[placedQueenPositionIndex]);
        solution.push_back( originalPlacedQueensPositions[placedQueenPositionIndex] );  
    }
    
    backTracking( blockedPositionsMatrix, solutionObtained, solution, cellsForColor );
    
    return solution;
    
}

vector< tuple< vector<vector<int>>, vector<pair<int, int>>> > getFromAPI( int numberOfCases ){
    
    vector< tuple< vector<vector<int>>, vector<pair<int, int>>> > testCasesObtained;
    if(ONLINE_MODE){
        cout<<"doing something in online mode"<<endl<<endl;
    }
    else{
        
        for(int testCaseIndex = 0; testCaseIndex<min(numberOfCases, (int)1); testCaseIndex++){
            testCasesObtained.push_back(TEST_CASES[testCaseIndex]);
        }
    }
    
    return testCasesObtained;
        
}
void solve()
{
    
    vector< tuple< vector<vector<int>>, vector<pair<int, int>>> >inputs = getFromAPI( NUMBER_OF_CASES );
    
    for(auto input: inputs){
        vector<vector<int>> colorCompositionOfMatrix;
        vector<pair<int, int>> originalPlacedQueensPositions;
        
        tie(colorCompositionOfMatrix, originalPlacedQueensPositions) = input;
        // cout<<originalPlacedQueensPositions.size()<<endl;
        
        displayMatrix( colorCompositionOfMatrix, originalPlacedQueensPositions, "This should be you input matrix");
        vector<pair<int, int>> finalPlacedQueensPositions = solveThis( colorCompositionOfMatrix, originalPlacedQueensPositions );
        displayMatrix( colorCompositionOfMatrix, finalPlacedQueensPositions, "This is your output matrix" );
    }  
    
}

int32_t main()
{
    ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
    int t = 1; //cin>>t;
    while(t--) solve();
    return 0;
}
