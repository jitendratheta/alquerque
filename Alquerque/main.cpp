//
//  main.cpp
//  Alquerque
//
//  Created by Jitendra S. Kushwaha on 16/02/17.
//  Copyright © 2017 Jitendra S. Kushwaha. All rights reserved.
//

#include<iostream>
#include<cstdlib>
#include<cstring>
#include<algorithm>
#include<cmath>
#include<cstdio>
#include<fstream>

#include "types.h"

using namespace std;

void printBoard();

const int DEPTH = 6;
const int TOTAL_MOVES = 10;

enum Color : byte{E, W, B};

typedef long long int ll;
string bestMove,lastMove;
bool checkAgain;


int mySide,oppSide;
int possible[49][8] = {
    {7, 8, 1,-1,-1,-1,-1,-1}, 	//0
    {0,8,2,-1,-1,-1,-1,-1},      //1
    {8,9,10,1,3,-1,-1,-1},       //2
    {10,2,4,-1,-1,-1,-1,-1},     //3
    {10,11,12,3,5,-1,-1,-1},     //4
    {6,12,4,-1,-1,-1,-1,-1},     //5
    {13,12,5,-1,-1,-1,-1,-1},	//6
    {0,14,8,-1,-1,-1,-1,-1},     //7
    {0,14,15,16,7,9,1,2},		//8
    {16,8,10,2,-1,-1,-1,-1},     //9
    {16,17,18,9,11,2,3,4},		//10
    {18,10,12,4,-1,-1,-1,-1},	//11
    {6,20,19,13,18,11,5,4},		//12
    {6,20,12,-1,-1,-1,-1,-1},	//13
    {21,22,15,7,8,-1,-1,-1},     //14
    {22,14,16,8,-1,-1,-1,-1},	//15
    {23,22,24,15,17,9,8,10},     //16
    {24,16,18,10,-1,-1,-1,-1},	//17
    {25,24,26,17,19,11,10,12},	//18
    {26,20,18,12,-1,-1,-1,-1},	//19
    {27,13,26,19,12,-1,-1,-1},	//20
    {28,22,14,-1,-1,-1,-1,-1},	//21
    {28,21,14,29,30,23,15,16},	//22
    {30,22,24,16,-1,-1,-1,-1},	//23
    {31,30,32,23,25,17,16,18},	//24
    {32,24,26,18,-1,-1,-1,-1},	//25
    {34,33,27,25,20,32,18,19},	//26
    {34,20,26,-1,-1,-1,-1,-1},	//27
    {35,21,29,36,22,-1,-1,-1},	//28
    {28,30,36,22,-1,-1,-1,-1},	//29
    {37,36,38,29,31,22,23,24},	//30
    {38,30,32,24,-1,-1,-1,-1},	//31
    {39,40,38,33,31,25,26,24},	//32
    {34,40,32,26,-1,-1,-1,-1},	//33
    {41,27,40,33,26,-1,-1,-1},	//34
    {42,36,28,-1,-1,-1,-1,-1},	//35
    {42,43,44,35,37,28,29,30},	//36
    {44,36,38,30,-1,-1,-1,-1},	//37
    {44,45,46,37,39,31,30,32},	//38
    {46,40,38,32,-1,-1,-1,-1},	//39
    {48,47,41,34,46,39,33,32},	//40
    {48,40,34,-1,-1,-1,-1,-1},	//41
    {43,35,36,-1,-1,-1,-1,-1},	//42
    {42,44,36,-1,-1,-1,-1,-1},	//43
    {43,45,37,36,38,-1,-1,-1},	//44
    {44,46,38,-1,-1,-1,-1,-1},	//45
    {47,45,40,39,38,-1,-1,-1},	//46
    {48,46,40,-1,-1,-1,-1,-1},	//47
    {47,41,40,-1,-1,-1,-1,-1}	//48
};

int board[7][7] = {
    {B, B, B, B, B, B, B},
    {B, B, B, B, B, B, B},
    {B, B, B, B, B, B, B},
    {B, B, B, E, W, W, W},
    {W, W, W, W, W, W, W},
    {W, W, W, W, W, W, W},
    {W, W, W, W, W, W, W}};


inline int FindCurPos() {
    
    int black = 0,white = 0, difference;
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j) {
            if (board[i][j] == W)
                ++white;
            if(board[i][j] == B)
                ++black;
        }
    difference = white - black;
    
    if(mySide == W)
        return difference;
    else
        return -difference;
}


inline void update_board(string move,int side) {
    if(move.length() <= 0)
        return;
    
    unsigned long numOfMoves = (move.length() / 3) + 1;
    int row,column;
    if (move.at(2) == '*') {
        board['7' - (int)move.at((numOfMoves - 1) * 3 + 1)]
            [(int)move.at((numOfMoves - 1) * 3) - 'a'] = side; //placing side at the destination
        if ((move.at(1) != move.at((numOfMoves - 1) * 3 + 1)) ||
                (move.at(0) != move.at((numOfMoves - 1) * 3)))		//important
            board['7' - (int)move.at(1)][(int)move.at(0) - 'a'] = E;
        for (int i = 1; i < numOfMoves; i++) {
            row = (('7' - move.at((i * 3) + 1)) + ('7' - move.at((i - 1) * 3 + 1)) ) / 2;
            column = ((move.at(i * 3) - 'a') + (move.at((i - 1) * 3) - 'a') )/ 2;
            board[row][column] = E;
        }
    }
    else {
        board['7' - (int)move.at(4)][(int)move.at(3) - 'a'] = side;
        board['7' - (int)move.at(1)][(int)move.at(0) - 'a'] = E;
    }
    
}


inline void back_track(string move,int side) {
    
    if(move.length() <= 0)
        return;

    unsigned long numOfMoves=(move.length()/3+1);
    int row,column;
    if (move.at(2) == '*') {
        board['7' - (int)move.at(1)][(int)move.at(0) - 'a'] = side;
        if ((move.at(1) != move.at((numOfMoves-1) * 3 + 1)) ||
                (move.at(0) != move.at((numOfMoves - 1) * 3)) )		//important
            board['7' - (int)move.at((numOfMoves - 1) * 3 + 1)]
                    [(int)move.at((numOfMoves - 1) * 3) - 'a'] = E;
        for (int i = 1; i < numOfMoves; i++)
        {
            row=( ('7' - move.at((i * 3) + 1)) + ('7' - move.at((i - 1) * 3 + 1)) )/ 2;
            column=( (move.at(i * 3) - 'a') + (move.at((i - 1) * 3) - 'a') )/ 2;
            board[row][column] = (side ^ 3);
        }
    }
    else {
        board['7' - (int)move.at(1)][(int)move.at(0) - 'a'] = side;
        board['7' - (int)move.at(4)][(int)move.at(3) - 'a'] = E;
    }

}

inline bool isOnCorner(int i,int j) {
    return ((i==0 && j==0) || (i==0 && j==6) || (i==6 && j==0) || (i==6 && j==6));
}

inline bool isOnEdge(int i,int j) {
    return (i==0 || j==0 || i==6 || j==6);
}

int GameTree(int depth,int alpha,int beta,int level,int side)
{
    //cout<<"Calling Game tree "<<alpha<<"  "<<beta<<endl;
    if(depth==0)
    {
        //depending on level assign min or max in previuos level
        int currPos=FindCurPos();
        return currPos;
    }
    
    /*for all possible move
     for each move get string
     update board
     ++level;
     currMin=GameTree(--depth,alpha,beta,level,side)   //depending on condition
     currMax=GameTree(--depth,alpha,beta,level,side)
     --level
     back track board
     */
    
    
    int belowMax,belowMin;
    //int changedSide;
    int outerRow,outerCol;
    int movableRow,movableCol;
    int InnerRow,InnerCol;
    int nMovableRow,nMovableCol;
    for(int i=0;i<49;i++)
    {
        outerRow=i/7;outerCol=i%7;
        
        if(board[outerRow][outerCol]==side)
            for(int j=0;possible[i][j]!=-1 && j<8;j++)
            {
                string nMove;
                nMove.append(1,(char)('a'+outerCol));
                nMove.append(1,(char)('7'-outerRow));
                //cout<<nMove<<endl;
                movableRow=possible[i][j]/7;
                movableCol=possible[i][j]%7;
                //Checking for border case where we cant jump out
                if(alpha>=beta)							//This is whats called pruning
                    return beta;
                /*neighbour element is empty*/
                if(board[movableRow][movableCol]==0)
                {
                    nMove.append("-");
                    nMove.append(1,(char)('a'+movableCol));
                    nMove.append(1,(char)('7'-movableRow));
                    //printf("Normal move from %d ->%d ",i,possible[i][j]);
                    //cout<<"("<<nMove<<")"<<flush<<endl;
                    
                    
                    //***********//this is recursion part
                    update_board(nMove,side);
                    //printBoard();
                    //changedSide=((side==1)?2:1);
                    /***********************/	//This part is important
                    if((level&1)==1)
                    { //opponent is playing
                        //pruning will be here
                        //			    cout<<"opponent seeing for :"<<nMove<<" At level :"<<level<<endl;
                        //			    printBoard();
                        belowMin=GameTree(depth-1,alpha,beta,level+1,((side==1)?2:1));
                        if(beta>belowMin)
                            beta=belowMin;
                        /*	if(alpha>=beta)				//This is whats called pruning
                         return beta;	*/
                        
                    }
                    else
                    { //me is playing
                        //				cout<<"i am seeing for :"<<nMove<<" At level :"<<level<<endl;
                        //				printBoard();
                        belowMax=GameTree(depth-1,alpha,beta,level+1,((side==1)?2:1));
                        //				cout<<"got below max "<<belowMax<<" at "<<level<<endl;
                        if(level==0)
                        {
                            //cout<<"checking "<<bestMove<< " "<<lastMove<<endl;
                            if ((bestMove=="") && ( (lastMove[0]!=nMove[3])  || (lastMove[1]!=nMove[4]) ) )
                            {
                                //						cout<<"Choosing best move :"<<nMove<<" At level :"<<level<<endl;
                                //						printBoard();
                                bestMove=nMove;
                                //						cout<<" Alpha :"<<((belowMax>alpha)?belowMax:alpha)<<" Beta :"<<beta<<endl;
                            }
                            else if(belowMax>=alpha)
                            {
                                if ( (lastMove[0]!=nMove[3])  || (lastMove[1]!=nMove[4]) )    //not to repeat last move
                                {
                                    //						cout<<"Choosing best move :"<<nMove<<" because belowMax>alpha"<<" At level :"<<level<<endl;
                                    //						printBoard();
                                    if((nMove.length()>=bestMove.length()) && (nMove[2]=='*'))
                                        bestMove=nMove;
                                    //						cout<<" Alpha :"<<belowMax<<"Beta :"<<beta<<endl;
                                }
                            }
                        }
                        if(belowMax>alpha)
                            alpha=belowMax;
                        
                    }
                    /*******************************/
                    back_track(nMove,side);
                    //***********//this is recursion part
                    //  call the function recursively
                    
                    
                    
                    
                }
                /*neighbour element is same as ours*/
                else if(board[movableRow][movableCol]==side)
                    ;//printf("No move possible from %d ->%d\n",i,possible[i][j]);
                
                /*neighbour element is opponents player*/
                else
                {
                    if((!isOnEdge(outerRow,outerCol) && isOnEdge(movableRow,movableCol)) || (isOnEdge(outerRow,outerCol) && isOnCorner(movableRow,movableCol)))
                        ;//printf("Going outside board from %d-> %d \n",i,(2*possible[i][j] - i));
                    else
                    {
                        if(board[(2*possible[i][j] - i)/7][(2*possible[i][j] - i)%7]==0)
                        {
                            ;//printf("cut move possible from %d-> %d ",i,(2*possible[i][j] - i));
                            nMove.append("*");
                            nMove.append(1,(char)('a'+(2*possible[i][j] - i)%7));
                            nMove.append(1,(char)('7'-(2*possible[i][j] - i)/7));
                            //cout<<"("<<nMove<<")"<<flush<<endl;
                            
                            
                            checkAgain=true;
                            
                            int p=i,q=j;
                            string cMove=nMove;
                            while(checkAgain)
                            {
                                checkAgain=false;
                                update_board(nMove,side);
                                //printBoard();
                                //sleep(2);
                                // Update the board accordingly
                                p=2*possible[p][q]-p;
                                InnerRow=p/7;
                                InnerCol=p%7;
                                for(q=0;possible[p][q]!=-1&&q<8;q++)   //Start of for loop
                                {
                                    
                                    nMovableRow=possible[p][q]/7;
                                    nMovableCol=possible[p][q]%7;
                                    if((board[nMovableRow][nMovableCol]!=side)&&(board[nMovableRow][nMovableCol]!=0))
                                    {
                                        //the neighbour is an opponent
                                        if((!isOnEdge(InnerRow,InnerCol) && isOnEdge(nMovableRow,nMovableCol)) || (isOnEdge(InnerRow,InnerCol) && isOnCorner(nMovableRow,nMovableCol)))
                                            ;//printf("Going outside board from %d-> %d \n",p,(2*possible[p][q] - q));
                                        else
                                        {
                                            //cout<<"point checking"<<(2*possible[p][q] - p)<<endl;
                                            if(board[(2*possible[p][q] - p)/7][(2*possible[p][q] - p)%7]==0)
                                            {
                                                //printf("cut move possible from %d-> %d \n",p,(2*possible[p][q] - p));
                                                //sleep(2);
                                                cMove.append("*");
                                                cMove.append(1,(char)('a'+(2*possible[p][q] - p)%7));
                                                cMove.append(1,(char)('7'-(2*possible[p][q] - p)/7));
                                                nMove="";
                                                nMove.assign(cMove, cMove.length()-5, 5 );
                                                //cout<<cMove<<endl;
                                                //cout<<nMove<<flush<<endl;
                                                checkAgain=true;
                                                break;   // call gametree
                                            }
                                        }
                                        
                                    }	//end of if when multiple cross is possible
                                }   //end of for loop
                                
                            }   //  end of while
                            //cout<<"cut move possible from "<<cMove<<flush<<endl;
                            //***********//this is recursion part
                            // This already updated in parts -->update_board(cMove);
                            //printBoard();
                            
                            
                            /***********************/	//This part is important
                            if((level&1)==1)
                            { //opponent is playing
                                //pruning will be here
                                //			    cout<<"opponent seeing for :"<<cMove<<" At level :"<<level<<endl;
                                //			    printBoard();
                                belowMin=GameTree(depth-1,alpha,beta,level+1,((side==1)?2:1));
                                if(beta>belowMin)
                                    beta=belowMin;
                                
                            }
                            else
                            { //me is playing
                                //				cout<<"i am seeing for :"<<cMove<<" At level :"<<level<<endl;
                                //				printBoard();
                                belowMax=GameTree(depth-1,alpha,beta,level+1,((side==1)?2:1));
                                //				cout<<"got below max "<<belowMax<<" at "<<level<<endl;
                                if(level==0)
                                {
                                    if(bestMove=="" )
                                    {
                                        //						cout<<"Choosing best move :"<<cMove<<" At level :"<<level<<endl;
                                        //						printBoard();
                                        //						cout<<" Alpha :"<<((belowMax>alpha)?belowMax:alpha)<<"Beta :"<<beta<<endl;
                                        bestMove=cMove;
                                    }
                                    else if(belowMax>=alpha)
                                    {
                                        //if ( (lastMove[0]!=nMove[3])  || (lastMove[1]!=nMove[4]) )  //not to repeat last move
                                        //{
                                        //						cout<<"Choosing best move :"<<cMove<<" because belowMax>alpha"<<" At level :"<<level<<endl;
                                        //						printBoard();
                                        bestMove=cMove;			
                                        //						cout<<" Alpha :"<<belowMax<<"Beta :"<<beta<<endl;
                                        //}
                                    }
                                }
                                if(belowMax>alpha)
                                    alpha=belowMax;	
                                
                            }
                            /*******************************/
                            
                            back_track(cMove,side);						
                            
                            //***********//this is recursion part
                            
                        }   //end of if(board[(2*possible[i][j] - i)/7][(2*possible[i][j] - i)%7]==0)
                    }
                }
            }	//end of checking each move in hash of 8 from given position
    }		// end of   -->for(int i=0;i<49;i++)
    
    if((level&1)==1) //opponent is playing
        return beta;
    else
        return alpha;
    
}

void getBestMove(int side) {
    bestMove = "";
    GameTree( DEPTH , -24, 24, 0, side);
}

int main() {
    
    int checkPosition;
    mySide = 1;
    oppSide = 2;
    
    // Maintain the number of moves played. Max 100 moves can be done
   	int numberOfMoves = 0;
    
    // the first Move would decide whether you would play as white player or
    // black player if the firstMove is "Start" then play as White else play as Black
   	printBoard();
    string opponentMove;
    string firstMove;
    cin >> firstMove;
    
    
    if (firstMove == "Start" || firstMove == "start")   // Play as white
        mySide = 1;
    else {                                             // Play as black
        mySide=2;
        oppSide=1;
        update_board(firstMove, oppSide);
        printBoard();
    }
    
    while (++numberOfMoves < TOTAL_MOVES) {
        
        getBestMove(mySide);
        lastMove = bestMove;
        cout << bestMove << endl;
        
        update_board(bestMove, mySide);
        printBoard();
        
        // Now Take opponents move
        // cin >> opponentMove;
        getBestMove(oppSide);
        opponentMove = bestMove;
        cout << opponentMove << endl;
        
        if (opponentMove == "Quit")
            break;
        
        // Update the board with that move
        update_board(opponentMove, oppSide);
        
        printBoard();
    }
    return 0;
}




void printBoard() {
 
//   Sample board view
    
//    7  █ --- █ --- █ --- █ --- █ --- █ --- █
//       | \   |   / | \   |   / | \   |   / |
//       |   \ | /   |   \ | /   |   \ | /   |
//    6  █ --- █ --- █ --- █ --- █ --- █ --- █
//       |   / | \   |   / | \   |   / | \   |
//       | /   |   \ | /   |   \ | /   |   \ |
//    5  █ --- █ --- █ --- █ --- █ --- █ --- █
//       | \   |   / | \   |   / | \   |   / |
//       |   \ | /   |   \ | /   |   \ | /   |
//    4  █ --- █ --- █ ---   --- ● --- ● --- ●
//       |   / | \   |   / | \   |   / | \   |
//       | /   |   \ | /   |   \ | /   |   \ |
//    3  ● --- ● --- ● --- ● --- ● --- ● --- ●
//       | \   |   / | \   |   / | \   |   / |
//       |   \ | /   |   \ | /   |   \ | /   |
//    2  ● --- ● --- ● --- ● --- ● --- ● --- ●
//       |   / | \   |   / | \   |   / | \   |
//       | /   |   \ | /   |   \ | /   |   \ |
//    1  ● --- ● --- ● --- ● --- ● --- ● --- ●
//       a     b     c     d     e     f     g
    
    std::wcout.imbue(std::locale("en_US.UTF-8"));
    
    string intermediatLine[4] = { "   | \\   |   / | \\   |   / | \\   |   / |\n",
        "   |   \\ | /   |   \\ | /   |   \\ | /   |\n",
        "   |   / | \\   |   / | \\   |   / | \\   |\n",
        "   | /   |   \\ | /   |   \\ | /   |   \\ |\n" };
    
    string lastLine = "   a     b     c     d     e     f     g\n";
    
    wchar_t object[] = {L' ', L'☻', L'█'};
    char order = 0;
    
    for (int i = 7; i > 0; i--)
    {
        cout    << i << "  ";
        wcout   << object[board[7 - i][0]] << L" --- "
        << object[board[7 - i][1]] << L" --- "
        << object[board[7 - i][2]] << L" --- "
        << object[board[7 - i][3]] << L" --- "
        << object[board[7 - i][4]] << L" --- "
        << object[board[7 - i][5]] << L" --- "
        << object[board[7 - i][6]] << endl;
        
        if(i > 1) {
            cout << intermediatLine[++order];
            cout << intermediatLine[++order];
        }
        order %= 4;
    }
    cout<< lastLine << endl;

}
