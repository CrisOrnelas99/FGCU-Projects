// Developers: Cristobal Ornelas and Joshua Martin
// Class: COP 2006 Spring 2025
// Date: April 20, 2025

/*Resources::   https://www.geeksforgeeks.org/rand-and-srand-in-ccpp/
                https://www.geeksforgeeks.org/static-member-function-in-cpp/
                https://www.w3schools.com/cpp/ref_ctime_clock.asp
                https://www.quora.com/How-do-you-create-a-C-vector-of-structs
                https://www.geeksforgeeks.org/cpp-program-for-char-to-int-conversion/
                ChaptGPT. (2025, March 28). How to properly organize multiple classes and functions into one .cpp file without function calling error. Generated using OpenAI. https://chatgpt.com/
                https://www.geeksforgeeks.org/sudoku-backtracking-7
                https://www.youtube.com/watch?v=eAFcj_2quWI
                https://pencilprogrammer.com/algorithms/sudoku-using-backtracking/
*/

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <limits>

// Generates a random number for the board(0-8)
int randomNum9()
    {
    return rand() % 9;
    }
//class for tracking user's movements and score
class TrackUser
{
    public:
        //user's current score, options used
        int score = 0;
        int numCorrectMoves = 0;
        int numHints = 0;
        int numUndos = 0;
        //keep track of each move from user
        struct UserMove
        {
            int row;
            int col;
            int previousNum;
            bool correctlyPlaced;   //if the number in square is correct
        };
        std::vector<UserMove> moveHistory; //vector of all user moves using struct (used for the undo option)
};
//class for validating user input
class InputValidator
{
    public:
        //check if user integer input (1-9) is valid
        static void validateInteger(int &userInt)
        {
            bool validInput = false;
            //loop until valid input is received and valid Input becomes true
            while (!validInput)
            {
                try
                {
                    std::cin >> userInt;
                    //checks if input failed or for unnecessary characters
                    if (std::cin.fail() || std::cin.peek() != '\n')
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw " Enter a valid number [1-9]: ";
                    }
                    //check if input is not (1-9)
                    if (userInt < 1 || userInt > 9)
                        throw " Enter a valid number [1-9]: ";
                    //exit loop if input is valid
                    validInput = true;
                    }
                //prints error message and prompt user to put correct input
                catch (const char *msg)
                    {
                    std::cerr << "Invalid Input!" << msg << std::endl;
                    }
                }
        }

        static void validateChar(char &userChar, bool RtoZ)
        {
            bool validInput = false;
            //loop until valid input is received and valid Input becomes true
            while (!validInput)
            {
                try
                {
                    std::cin >> userChar;
                    //check if input failed or for unnecessary characters
                    if (std::cin.fail() || std::cin.peek() != '\n')
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw " Enter only one valid character";
                    }
                    //checks if user Input for (A-K)
                    if (!RtoZ)
                    {
                        if (userChar < 'A' || userChar > 'K')
                        {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            throw " Enter a valid column (A-I) or option (J/K): ";
                        }
                    }
                    //check if user input (R-Z)
                    else if (RtoZ && (userChar < 'R' || userChar > 'Z'))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw " Enter a valid row (R-Z): ";
                    }
                    //exit loop if input is valid
                    validInput = true;
                }
                //if error thrown it is caught, prints error message and Prompt user to try again
                catch (const char *msg)
                {
                    std::cerr << "Invalid Input!" << msg << std::endl;
                }
            }
        }
};

class SudokuGenerator
{
    public:
        //Checks if placing number in each row/column and block is okay to do so
        static bool safeSquare(int board[9][9], int row, int col, int num)
        {
            // Check if number exists in the same row or column
            for (int index = 0; index < 9; index++)
                if (board[row][index] == num || board[index][col] == num)
                    return false;
            // Get top-left corner of the 3x3 box
            int startRow = row - row % 3;
            int startCol = col - col % 3;

            // Check the 3x3 block for the same number
            for (int blockRow = 0; blockRow < 3; blockRow++)
                for (int blockCol = 0; blockCol < 3; blockCol++)
                    if (board[startRow + blockRow][startCol + blockCol] == num)
                        return false;
            //square is good to place number
            return true;
        }
        //fill board using backtracking algorithm
        static bool backTrackSudoku(int board[9][9], int row, int col)
        {
            // If all rows are done, board is complete
            if (row == 9)
                return true;
            // Move to next row with recursion
            if (col == 9)
                return backTrackSudoku(board, row + 1, 0);
            //Skip filled squares with recursion and go column by column
            if (board[row][col] != 0)
                return backTrackSudoku(board, row, col + 1);
            //shuffle numbers 1-9 using random num generator
            int nums[9] = {1,2,3,4,5,6,7,8,9};
            for (int index = 0; index < 9; index++)
            {
                int randomNum = randomNum9();
                std::swap(nums[index], nums[randomNum]);
            }
            for (int num = 0; num < 9; num++)
            {
                int currentNum = nums[num];
                if (safeSquare(board, row, col, currentNum))
                {
                    // Try safe numbers and place
                    board[row][col] = currentNum;
                    // Keep solving
                    if (backTrackSudoku(board, row, col + 1))
                        return true;
                    board[row][col] = 0; // backtrack
                }
            }
            //no number fit in board
            return false;
        }
        //called from main and generates full board, copy board unto solvedBoard and removes numbers to play
        static void generateBoard(int board[9][9], int solvedBoard[9][9],  bool modifiableSquares[9][9])
        {
            //start board with zero's
            for (int row = 0; row < 9; row++)
                for (int col = 0; col < 9; col++)
                    board[row][col] = 0;
            //fill board using backtracking
            backTrackSudoku(board, 0, 0);
            //copy solution from generated board
            for (int row = 0; row < 9; row++)
                for (int col = 0; col < 9; col++)
                    solvedBoard[row][col] = board[row][col];
            //remove numbers from board so player can start
            removeNums(board, modifiableSquares);
        }
        //remove random squares from the full generated board
        static void removeNums(int board[9][9], bool modifiableSquares[9][9])
        {
            //number of squares to remove(difficulty)
            int removeCount = 50;
            //loop until numbers that need to be removed are removed
            while (removeCount > 0)
            {   //generate random coordinate
                int row = randomNum9();
                int col = randomNum9();
                //make sure square doesn't have zero already
                if (board[row][col] != 0)
                {
                    //remove num and place zero
                    board[row][col] = 0;
                    //set it as something user is able to modify in board
                    modifiableSquares[row][col] = true;
                    removeCount--;
                }
            }
        }
        //check if board is full (no zero's)
        static bool isBoardFull(int board[9][9])
        {
            for (int row = 0; row < 9; row++)
                for (int col = 0; col < 9; col++)
                    if (board[row][col] == 0)
                        return false;
            return true;
        }
        //displays the board for the user in terminal
        static void displayBoard(int board[9][9])
        {
            std::cout << "       A     B     C     D     E     F     G     H     I  \n";
            std::cout << "    ------+-----+-----+-----+-----+-----+-----+-----+------\n";

            for (int col = 0; col < 9; col++)
            {
                //label rows (R-Z) and create grid
                std::cout << " " << char('R' + col) << "  |";
                for (int row = 0; row < 9; row++)
                {
                    //if the number has a zero, display empty square
                    if (board[row][col] == 0)
                        std::cout << "     |";
                    //display the number in square
                    else
                        std::cout << "  " << board[row][col] << "  |";
                }
                std::cout << "\n    ------+-----+-----+-----+-----+-----+-----+-----+------\n";
            }
        }
};

class userPlay
{
    public:
        //keep score and check user moves
        TrackUser track;
        //function to let user undo move, except hints
        void userUndo(int board[9][9])
        {
            //if hint used
            if (track.moveHistory.empty())
            {
                std::cout << "****Can't undo Hints****\n";
                return;
            }
            // store last move and remove it from history(vector) when undo
            TrackUser::UserMove lastMove = track.moveHistory.back();
            track.moveHistory.pop_back();
            // Put number back to what it was before the user placed num
            board[lastMove.row][lastMove.col] = lastMove.previousNum;
            //remove points for undoing
            track.score -= 5;
            track.numUndos++;
            // If the undone move was correct, remove correct num points to fix score
            if (lastMove.correctlyPlaced)
            {
                track.score -= 10;
                track.numCorrectMoves--;
            }
            // display to user what square was undone
            std::cout << "Undo: (" << char('A' + lastMove.row) << ", "
                      << char('R' + lastMove.col) << ") to " << lastMove.previousNum
                      << "\n";
        }
        //fill random empty square with correct num
        void userHint(int board[9][9], int solvedBoard[9][9])
        {
            //initialize random coordinate
            int row, col;
            do {
                row = randomNum9();
                col = randomNum9();
            }
            //loop until empty square found
            while (board[row][col] != 0);
            //place correct square from solved board to empty square in user's board
            board[row][col] = solvedBoard[row][col];
            //remove points for using hint
            track.score -= 15;
            track.numHints++;
            //display where hint was placed
            std::cout << "Hint placed at (" << char('A' + row) << ", " << char('R' + col) << ")\n";
        }
        //compare user board to solved board
        bool checkSolution(int board[9][9], int solvedBoard[9][9])
        {
            for (int row = 0; row < 9; row++)
                for (int col = 0; col < 9; col++)
                    if (board[row][col] != solvedBoard[row][col])
                        //boards don't match
                        return false;
            //board matches
            return true;
        }
        //main game loop function called from main
        void playGame(int board[9][9], int solvedBoard[9][9], bool modifiableSquares[9][9])
        {
            //declare input variables and initialize bool for if game is complete
            char userRow, userColumn;
            int userNum;
            bool gameFinished = false;
            //start timer for the end score
            clock_t startTime = clock();
            //play sudoku until user finished
            while (!gameFinished)
            {
                //display board in terminal
                SudokuGenerator::displayBoard(board);
                //if board is completely full, break out of loop
                if (SudokuGenerator::isBoardFull(board))
                    break;
                //prompt user for input for options or go ahead and place coordinates
                std::cout << "Enter 'J' to undo, 'K' to get a random hint or\n";
                std::cout << "Enter column (A-I): ";
                //call function using the user's input as argument for error handling, don't use (R-K)
                InputValidator::validateChar(userColumn, false);
                //call undo Function if 'J'
                if (userColumn == 'J')
                {
                    userUndo(board);
                    continue;
                }
                //call userHint function if 'K'
                if (userColumn == 'K')
                {
                    userHint(board, solvedBoard);
                    continue;
                }
                //prompt user for inputs
                std::cout << "Enter row (R-Z): ";
                //call function using the user's input as argument for error handling, use (R-K)
                InputValidator::validateChar(userRow, true);
                //call function using the user's input as argument for error handling
                std::cout << "Enter number (1-9): ";
                InputValidator::validateInteger(userNum);
                //convert user char to integers for row/column index
                int xAxis = userColumn - 'A';
                int yAxis = userRow - 'R';
                //only allow to change squares that are able to modify
                if (modifiableSquares[xAxis][yAxis])
                {
                    //check if user's num input is correct
                    bool correctlyPlaced = (userNum == solvedBoard[xAxis][yAxis]);
                    //save the move into moveHistory vector using the userMove struct
                    TrackUser::UserMove userMove = { xAxis, yAxis, board[xAxis][yAxis], correctlyPlaced };
                    track.moveHistory.push_back(userMove);
                    //update board
                    board[xAxis][yAxis] = userNum;
                    //display where user placed number
                    std::cout << "Placed " << userNum << " at " << userColumn << userRow << "\n";
                    //add points if correctly placed)
                    if (correctlyPlaced)
                    {
                        track.score += 10;
                        track.numCorrectMoves++;
                    }
                }
                //any square not able to be modified will display this
                else
                    std::cout << "*****The square can't be modified*****\n";
            }
            //stop the timer and give points depending on time completed board
            clock_t endTime = clock();
            double timeTaken = double(endTime - startTime) / CLOCKS_PER_SEC;
            if (timeTaken < 120)
                track.score += 50;
            else if (timeTaken < 360)
                track.score += 30;
            else if (timeTaken < 600)
                track.score += 10;
            //check if board solved correctly by comparing user board and solved board
            bool solved = checkSolution(board, solvedBoard);
            //display if solved or not solved
            if (!solved)
            {
                track.score = 0;
                std::cout << "Incorrect solution!\n";
            }
            else
            {
                if (track.score < 0)
                    track.score = 0;
                std::cout << "Board Solved!\n";
            }
            //display stats and end score
            std::cout << "\n--- Your Score ---\n";
            std::cout << "Correct User Moves: " << track.numCorrectMoves << " \n";
            std::cout << "Hints Used: " << track.numHints << " \n";
            std::cout << "Undos Used: " << track.numUndos << " \n";
            std::cout << "Time Taken: " << timeTaken << " seconds\n";
            std::cout << "--------------------------\n";
            std::cout << "Total Score: " << track.score << " points\n";
        }
};

int main()
{
    //random number generator
    srand(time(0));
    userPlay game;
    //declare user board and solved board
    int board[9][9], solvedBoard[9][9];
    //track which square in board is modifiable, initiate all squares to false
    bool modifiableSquares[9][9] = {false};
    //generate the board
    SudokuGenerator::generateBoard(board, solvedBoard, modifiableSquares);
    //play the game
    game.playGame(board, solvedBoard, modifiableSquares);
    return 0;
}
