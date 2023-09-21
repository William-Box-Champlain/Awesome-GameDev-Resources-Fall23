
#include <list>
#include <iostream>
#include <string>

//Useful enums
enum Direction
{
  up,
  upLeft,
  upRight,
  down,
  downLeft,
  downRight,
  left,
  right,
  defDir
};

enum State
{
  live,
  dead,
  defSta
};

//Useful structs
struct Grid  //simple struct holding an x and y int. Use to hold board-dimensions and board-positions
{
  int x;
  int y;
};

struct Board
{
  int mColumns;
  int mLines;
  State** mBoard;
  Board(int columns,int lines)
  {
    mColumns = columns;
    mLines = lines;
    mBoard = new State*[mLines];
    for(int i = 0; i < mLines; i++)
    {
      mBoard[i] = new State[mColumns];
    }
    for(int i = 0; i < mLines; i++)
    {
      for(int j= 0; j< mColumns; j++)
      {
        mBoard[i][j] = dead;
      }
    }
  }
  void setState(Grid positionToSet,State newState) const
  {
    mBoard[positionToSet.x][positionToSet.y] = newState;
  }
  ~Board()
  {
    delete mBoard;
  }
};

//function declarations
Grid GetPositionToLook(Direction whereToLook, Grid myPosition);
Grid checkPositionToLook(Grid positionToLook, Grid boardDimensions);
State GetStateInDirection(const Board& myBoard, Direction whereToLook, Grid myPosition, Grid boardDimensions);
int CountLivingByMe(const Board& myBoard, Grid myPosition, Grid boardDimensions);
State AmIAliveOrDead(const Board& myBoard, State myState, Grid myPosition, Grid boardDimensions);

//coding the actual game structure
struct game
{
  int mColumns;
  int mLines;
  Board *current;
  Board *next;
  game(int columns, int lines)
  {
    mColumns = columns;
    mLines = lines;
    current = new Board(columns,lines);
    next = new Board(columns,lines);
  }
  void update()
  {
    for(int i = 0; i< mLines; i++)
    {
      for(int j = 0; j < mColumns; j++)
      {
        Grid myPosition{};
        myPosition.x = j;
        myPosition.y = i;
        Grid boardDimensions{};
        boardDimensions.x = mColumns;
        boardDimensions.y = mLines;
        next->mBoard[i][j] = AmIAliveOrDead(*current,current->mBoard[i][j],myPosition,boardDimensions);
      }
    }
    delete current;
    current = next;
    next = new Board(mColumns,mLines);
  }
  void setState(Grid positionToSet,State newState) const
  {
    current->setState(positionToSet,newState);
  }
  void coutBoardState() const
  {
    for(int i = 0; i< mLines; i++)
    {
      for(int j = 0; j < mColumns; j++)
      {
        if(current->mBoard[i][j] == live){std::cout << "#";}
        if(current->mBoard[i][j] == dead){std::cout << ".";}
      }
    }
  }
  ~game()
  {
    delete current;
    delete next;
  }
};

int main(){
  //setup variables
  int columns;
  int lines;
  int turns;

  //get lines
  std::cin >> columns >> lines >> turns;

  //setup board
  game gameBoard(columns,lines);

  // parse . and # to get initial state
  std::string line;
  for(int i = 0; i < lines; i++)
  {
    std::getline(std::cin,line);
    int j = 0;
    for(char& c : line)
    {
      if(c == '.')
      {
        j++;
      }
      if(c == '#')
      {
        Grid temp{};
        temp.x = j;
        temp.y = i;
        gameBoard.setState(temp,live);
      }
    }
  }
  //simulate steps
  for(int i = 0; i < turns; i++)
  {
    gameBoard.update();
  }

  //output the final state
  gameBoard.coutBoardState();
  return 0;
}

//determine what Grid is being looked at from myPosition
Grid GetPositionToLook(Direction whereToLook, Grid myPosition)
{
  Grid positionToLook = myPosition;
  switch (whereToLook) {
    case up: // up one
      positionToLook.y++;
      break;
    case upLeft: // up one, left one
      positionToLook.y++;
      positionToLook.x--;
      break;
    case upRight: //up one, right one
      positionToLook.y++;
      positionToLook.x++;
      break;
    case down: //down one
      positionToLook.y--;
      break;
    case downLeft: //down one, left one
      positionToLook.y++;
      positionToLook.x--;
      break;
    case downRight: //down one, right one
      positionToLook.y++;
      positionToLook.x++;
      break;
    case left: //left one
      positionToLook.x--;
      break;
    case right: //right one
      positionToLook.x++;
      break; //nothing
    case defDir:
      break;
  }
  return positionToLook;
}
//if positionToLook isn't a valid position, then translate it to a valid position
Grid checkPositionToLook(Grid positionToLook, Grid boardDimensions)
{
  Grid temp = positionToLook;
  if(temp.x > boardDimensions.x)
  {
    temp.x = temp.x % boardDimensions.x;
  }
  if(temp.x < 0)
  {
    temp.x = boardDimensions.x + temp.x;
  }
  if(temp.y > boardDimensions.y)
  {
    temp.y = temp.y % boardDimensions.y;
  }
  if(temp.y < 0)
  {
    temp.y = boardDimensions.y + temp.y;
  }
  return temp;
}
//Look at grid in the direction whereToLook from myPosition to find out if it's live or dead
State GetStateInDirection(const Board& myBoard, Direction whereToLook, Grid myPosition, Grid boardDimensions)
{
  //find position to look and check it's a valid position
  Grid positionToLook = GetPositionToLook(whereToLook,myPosition);
  positionToLook = checkPositionToLook(positionToLook,boardDimensions);
  return myBoard.mBoard[positionToLook.x][positionToLook.y];
}

int CountLivingByMe(const Board& myBoard, Grid myPosition, Grid boardDimensions)
{
  int livingCount = 0;
  for(int i = up; i != defDir; i++)
  {
    if(GetStateInDirection(myBoard, static_cast<Direction>(i),myPosition,boardDimensions) == State::live)
      livingCount++;
  }
  return  livingCount;
}

State AmIAliveOrDead(const Board& myBoard, State myState, Grid myPosition, Grid boardDimensions)
{
  int livingByMe = CountLivingByMe(myBoard,myPosition,boardDimensions);
  if(myState == live)
  {
    if(livingByMe < 2 || livingByMe > 3)
    {
      return dead;
    }
    if(livingByMe == 2 || livingByMe == 3)
    {
      return live;
    }
  }
  if(myState == dead && livingByMe == 3)
  {
    return live;
  }
  if(myState == defSta)
  {
    return dead;
  }
  return dead;
}