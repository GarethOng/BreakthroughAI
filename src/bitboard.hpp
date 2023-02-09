#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <stdint.h>
#include <cmath>
#include <string>

#include "types.hpp"

using pos = unsigned short int;

class Bitboard {
   private:
      uint64_t player; //default start uint64_t player = 65535;
      uint64_t ai; //default start uint64_t ai =18446462598732840960;
      bool checkPresent(pos pos, Agent agent);
      unsigned int countPieces(Agent agent);
      unsigned int countLaneControl(Agent agent);

   public:
      Bitboard(uint64_t player, uint64_t ai);
      Bitboard move(Agent agent, pos initial, pos final);
      bool isValidMove(pos initial, pos final, Agent agent);
      std::pair<Bitboard*, int> generateChildren(Agent agent);
      bool isPlayerWin();
      bool isAIWin();
      float evalute();
      std::string toString();
};

#endif