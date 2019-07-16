//------------------------------------------------------------------------------
// Game.h
//
// Declarations for Game class.
// 
//------------------------------------------------------------------------------
//

#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <utility>
#include "Field.h"
#include "Worm.h"
#include "Chest.h"
#include "Random.h"

namespace Sep
{

  //----------------------------------------------------------------------------
  // Game Class
  // class to store the map/board as a vector of vectors of Field, as well as
  //  the width and size of the board/map, thw worms of players and the chests
  // that are spawned into the map. This class also handles all the game logic
  // for  shooting weapons/moving/gravity/etc
  //
  class Game
  {
    private:
  
      //------------------------------------------------------------------------
      // ID of the worm on move
      //
      int worm_on_move_;
      
      //------------------------------------------------------------------------
      // ID of the worm on move if player one is on move, ID of player one's
      // next worm if player two is on move
      //
      int worm_of_player_one_;
      
      //------------------------------------------------------------------------
      // ID of the worm on move if player wto is on move, ID of player two's
      // next worm if player one is on move
      //
      int worm_of_player_two_;
      
      //------------------------------------------------------------------------
      // Width of the board/map
      //
      int board_width_;

      //------------------------------------------------------------------------
      // Height of the board/map
      //
      int board_height_;
  
      //------------------------------------------------------------------------
      // Height of the board/map
      //
      Random random_number_generator_;
      
      //------------------------------------------------------------------------
      // Vector of vectors to hold the information about the board/map
      //
      std::vector<std::vector<Field>> map_;
  
      //------------------------------------------------------------------------
      // Vector of objects of Worm class to hold information about the worms
      // of the players
      //
      std::vector<Worm> worms_;
  
      //------------------------------------------------------------------------
      // Vector of objects of Chest class to hold information about the chests
      // that spawn into the map
      //
      std::vector<Chest*> chests_;
      
      //------------------------------------------------------------------------
      // boolean variable to store if move has already been done in this turn
      //
      bool moved_;
      
      //------------------------------------------------------------------------
      // Private Method used to simplify loadConfig
      // Used to parse input from file until it meets the desired character,
      //  ignores empty lines, lines starting with a '#' and lines of spaces
      // @param std::ifstream* input Reference to ifstream object used to read
      // @param std::string* read_line Reference to string that the lines are
      //                                stored into
      // @param wanted_char The character we want to find
      // @return Returns 0 if successful, -1 if unsuccessful
      //
      int read_input_until_char(std::ifstream* input, std::string &read_line,
              char wanted_char);

      //------------------------------------------------------------------------
      // Private Method used to simplify loadConfig
      // Used to create instances of Field and set them in the map_ in 1 line
      // called in a for loop
      // @param std::ifstream &input Reference to ifstream object used to read
      // @param std::string &line Reference to string that the lines are
      //                                stored into
      // @param int row is the row we are storing the fields in
      // @return Returns 0 if successful, -1 if unsuccessful
      //
      int readLineOfMap(std::string &line, std::ifstream* input, int row);

      //------------------------------------------------------------------------
      // Private gravity Method
      // Used to make sure that a Worm falls (if it can) until it
      //  finds a non-AIR and non-CHEST field underneath itself.
      // Also handles potential death of the worm
      // @param worm - the index of the worm we are apply gravity to (in list_)
      // @return nothing
      //
      void gravity(int worm);
  
  
      //------------------------------------------------------------------------
      // Private findWormOnMove Method
      // it is  a getter  type method
      // Used to return the index of the worm that is currently on move
      // @param nothing
      // @return int index of worm that is on move now
      //
      int findWormOnMove();

      //------------------------------------------------------------------------
      // Private Move Method
      // Moves the worm on mnove left or right the required number of steps
      // returns 0 if OK, -1 if worm on move died
      //
      // @param direction dictates if the worm will move left or right
      // @param steps Number of steps the worm should move left or right
      //
      int move(int steps, int direction);
  
  
      //------------------------------------------------------------------------
      // Private findWormAt Method
      // Used to return the index of the worm that is currently on coordinates
      // passed to function
      // @param int row - the row we want
      // @param int col - the column that we want
      // @return int index of worm that is on those coordinates now, -1 if that
      // field is not a WORM
      //
      int findWormAt(int row, int col);
  
     
      //------------------------------------------------------------------------
      // Private gravity Method
      // Used to make sure that a Worm falls (if it can) until it
      // finds a non-AIR and field underneath itself.
      // Worm does not take damage here, and it is only used when spawning the
      // worms onto the map, but it can still die from falling into water
      // @param int row - the row we want
      // @param int col - the column we want
      // @return int new row of the worm we want
      //
      int beginGravity(int row, int col);

      //------------------------------------------------------------------------
      // private Chest gravity method
      // Used to create make sure a chest falls down, if it hits a worm, he
      //  picks it up
      // @param int index is the index of the wanted chest object in vector
      // chests_
      //
      void chestGravity(int index);

      //------------------------------------------------------------------------
      // private Pick up chest method
      // Used to make sure that a chest is deleted, the worm on location row, 
      // col gets its content, a message is dispalyed describing the content
      // of the chest
      //
      void pickUpChest(int row, int col, int chest);


      //------------------------------------------------------------------------
      // find chest at method
      // returns index of chest at row col, if it found a chest there,a nd -1
      // if no chest was found there
      //
      int findChestAt(int row, int col);  

      
    public:

      //------------------------------------------------------------------------
      // Constructor
      //
      Game();
  
  
      //------------------------------------------------------------------------
      // Destructor
      //
      ~Game();

      //------------------------------------------------------------------------
      // Load Configuration File Method
      // Reads the configuration file, checks its validity, and stores the map
      //  into a vector of vectors
      // @param cfg_file The name of the file we want to read
      // @return Returns 0 if sucessful, -1 if unsuccessful
      //
      int loadConfig(std::string cfg_file);

      //------------------------------------------------------------------------
      // Method for adding worm
      // Checks if a worm can be added at provided coordinates, then checks if
      //  worm can fall down
      //
      // @param row The row we want to spawn the worm in
      // @param col The column we want to spawn the worm in
      // @return Returns 0 if successful, -2 if unsuccessful
      //
      void addWorms();

      //------------------------------------------------------------------------
      // Print Method
      // Prints the current map, surrounded by coordiante numbers
      //
      void printMap();


      //------------------------------------------------------------------------
      // Move Logic Method
      // used to see if move command is currently allowed via moved_ attribute
      // @param steps -3 to 3, the number of steps the move should make, 
      //     negative if left, posititve if right
      // @return 1 if command not allowed, -1 if worm died, 0 if move OK
      //
      int moveLogic(int steps);
      

      //------------------------------------------------------------------------
      // Add chest method
      // Used to create a new instance of a chest on the map, then apply
      //  chest gravity to it
      //
      int addChest();
      

      //------------------------------------------------------------------------
      // Gained weapon switch method
      // returns a string describing the name of the passed weapon
      //
      std::string gainedWeaponSwitch(Worm::WeaponType weapon);
      
      

      //------------------------------------------------------------------------
      // Victory check method
      // Checks if the game is over due to a draw, or a victory of one of the
      // players
      // @return 0 if game not over, 1 if game over
      //
      int victoryCheck();
      
      //------------------------------------------------------------------------
      // State method
      // Prints the state of the map- the worm on move's ammo, the worms 
      // that are alive and their HP etc
      //
      void state();
      
      //------------------------------------------------------------------------
      // Print on move method
      // The name and the location of the worm currently on move
      //
      void printOnMove();
      
      
      //------------------------------------------------------------------------
      // Gun method
      // shoots gun from location of worm currently on move using shotHit()
      // in wanted direction
      // @param int direction the number on a numpad corresponding to the 
      // wanted direction
      //
      int gun(std::pair<int, int> coordinates_change);

      //------------------------------------------------------------------------
      // Bazooka method
      // shoots bazoka from location of worm currently on move using shotHit()
      // @param int direction the number on a numpad corresponding to the 
      // wanted direction
      //
      int bazooka(std::pair<int, int> coordinates_change);

      //------------------------------------------------------------------------
      // Blowtorch method
      // shoots blowtorch from location of worm currently on move using 
      // blowtorchHit and after that, blowtorchConsequence
      // @param int direction the number on a numpad corresponding to the 
      // wanted direction
      //
      int blowtorch(std::pair<int, int> coordinates_change);

      
      //------------------------------------------------------------------------
      // SHot hit method
      // Destory earth/chest
      // Damages and potentially kills worm 
      // @param int row the row we want to hit
      // @param col the column we want to hit
      // @param damage the damage we want to deal to a worm if found  
      // 
      void shotHit(int row, int col, int damage);


      //------------------------------------------------------------------------
      // Blowtorch consequece method
      // applies gravity to fields above if current field is empty
      //
      void shotConsequence(int row, int col);
    
      
      //------------------------------------------------------------------------
      // teleport method
      // if possible, teleports worm to wanted location, applies gravity to it
      // and possibly to a worm that was on top of our worm
      // @param int direction the number on a numpad corresponding to the 
      // wanted direction
      //
      int teleport(int row, int col);
      
      //------------------------------------------------------------------------
      // melee method
      // hits around the worm on move's location using hitwith bat
      //
      int melee();

      //------------------------------------------------------------------------
      // hit with bat method
      // Damages and potentially kills worm (then possibly appllies gravity to
      // field above)
      // @param int new_row the row we want to hit
      // @ param int new_col the column we want to hit
      // 
      void hitWithBat(int new_row, int new_column);
  

      //------------------------------------------------------------------------
      // Airstrike method
      // shoots airstrike into wanted column
      // @param int column the column we want to hit
      //@ return -1 if col outside map, 0 if successfull
      //
      int airstrike(int column);
      
      //------------------------------------------------------------------------
      // Getter for selected ammo of worm on move
      //
      int selectedAmmoOfWormOnMove();
      
      //------------------------------------------------------------------------
      // Setter for selected weapon of worm on move
      //
      int selectWeaponForWormOnMove(Worm::WeaponType weapon);
      

      //------------------------------------------------------------------------
      // Getter for selected weapon of worm on move
      //
      Worm::WeaponType getWeaponOfWormOnMove();


      //------------------------------------------------------------------------
      // Set first turn method
      // sets up worms of player one and two for first turn
      // sets moved_ to false
      //
      void setFirstTurn();

      //------------------------------------------------------------------------
      // Increase turn method
      // will set up worm_on_move to be next wormof other player, current 
      // player's next worm will increase
      //
      void increaseTurn();

      //------------------------------------------------------------------------
      // Increase player one method
      // will set up worm of player one to be next worm of p one that should be
      // on turn
      //
      void increasePlayerOne(void);

      //------------------------------------------------------------------------
      // Increase player two method
      // will set up worm of player two to be next worm of p two that should be
      // on turn
      //      
      void increasePlayerTwo(void);
      
      //------------------------------------------------------------------------
      // Increase turn method
      // sets the worm on move's selected weapon back to gun
      //
      void resetWeaponChoice();
      

      //------------------------------------------------------------------------
      // find worm with ID method
      // Essentially a getter method
      // returns index of worm with wanted ID, if it found a such a worm, and
      // -1 if no such worm is alive
      // @param id the id we want
      // @return the index of the worm with wanted ID, -1 if no such worm
      //
      int findWormWithId(int id);
      
     
  };
}

#endif //GAME_H
