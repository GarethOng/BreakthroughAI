#include "bitboard.hpp"
#include <iostream>

Bitboard::Bitboard(uint64_t p, uint64_t a) {
         player = p;
         ai = a;
}

bool Bitboard::checkPresent(pos pos, Agent agent) {
   return agent == Agent::AI ? ((1UL << pos) & ai) > 0 : ((1UL << pos) & player) > 0;
}

unsigned int Bitboard::countPieces(Agent agent) {
   unsigned int count = 0;
   for (int i = 0; i < 64; i ++) {
      if (checkPresent(i, agent)) {
         count += 1;
      }
   }
   return count;
}

unsigned int Bitboard::countLaneControl(Agent agent) {
   uint64_t mask = 0x0101010101010101UL;
   uint64_t board = agent == Agent::AI ? ai : player;
   unsigned int count = 0;
   for (int i = 0; i <= 7; i ++) {
      if ((mask & board) > 0) {
         count += 1;
      }
      mask = mask << 1;
   }
   return count;
}

bool Bitboard::isValidMove(pos initial, pos final, Agent agent) {
   bool withinRange =  (0 <= initial && initial <= 63) && (0 <= final && final <= 63);
   bool inControl = checkPresent(initial, agent);
   if (agent == Agent::AI) {
      bool withinMove = initial % 8 == 0 ?  (initial - 8 <= final) && (final <= initial - 7) : (initial + 1) % 8 == 0 ? (initial - 9 <= final) && (final <= initial - 8) : (initial - 9 <= final) && (final <= initial - 7);
      bool isBlocked = checkPresent(final, agent);
      bool isValidTakeDown = final == initial - 8 ? !checkPresent(final, Agent::player) : true;
      return withinRange && inControl && withinMove && !isBlocked && isValidTakeDown;
   } else {
      bool withinMove = initial % 8 == 0 ?  (initial + 8 <= final) && (final <= initial + 9) : (initial + 1) % 8 == 0 ? (initial + 7 <= final) && (final <= initial + 8) : (initial + 7 <= final) && (final <= initial + 9);
      bool isBlocked = checkPresent(final, agent);
      bool isValidTakeDown = final == initial + 8 ? !checkPresent(final, Agent::AI) : true;
      return withinRange && inControl && withinMove && !isBlocked && isValidTakeDown;
   }
}

Bitboard Bitboard::move(Agent agent,pos initial, pos final) {
   uint64_t newAI = ai;
   u_int64_t newPlayer = player;
   if (agent == Agent::AI) {
      newAI = newAI | (1UL << final);
      newAI = newAI & ~ (1UL << initial);
      newPlayer = newPlayer & ~ (1UL << final);
      return Bitboard(newPlayer, newAI);
   } else {
      newPlayer = newPlayer | (1UL << final);
      newPlayer = newPlayer & ~ (1UL << initial);
      newAI = newAI & ~ (1UL << final);
      return Bitboard(newPlayer, newAI);
   }
}

std::string Bitboard::toString() {
   std::string result = "   0   1   2   3   4   5   6   7\n";
   result += " ----+---+---+---+---+---+---+---|\n7|";
   int row = 56;
   int column = 0;
   while (row + column >= 0) {
      bool isAI = checkPresent(row + column, Agent::AI);
      bool isPlayer = checkPresent(row + column, Agent::player);
      if (isAI) {
         result += " B |";
      }
      if (isPlayer) {
         result += " W |";
      }
      if (!(isAI || isPlayer)) {
         result += "   |";
      }
      column += 1;
      if (column > 7) {
         column = 0;
         row -= 8;
         result += "\n ----+---+---+---+---+---+---+---|";
         if (row >= 0) {
            result += "\n"+ std::to_string(row / 8) + "|";
         }
      }
   }
   return result;
}

std::pair<Bitboard*, int> Bitboard::generateChildren(Agent agent) {
   Bitboard *children;
   children = (Bitboard*) malloc(44 * sizeof(Bitboard));
   int count = 0;
   std::pair<Bitboard*, int> result(children, count);

   if (agent == Agent::AI) {
      for (pos i = 0; i < 64; i ++) {
         if (checkPresent(i, agent)) {
            if (isValidMove(i, i - 7, agent)) {
               result.first[result.second] = move(agent, i, i - 7);
               result.second += 1;
            }
            if (isValidMove(i, i - 8, agent)) {
               result.first[result.second] = move(agent, i, i - 8);
               result.second += 1;
            }
            if (isValidMove(i, i - 9, agent)) {
               result.first[result.second] = move(agent, i, i - 9);
               result.second += 1;
            }
         }
      }
      return result;
   } else {
      for (pos i = 0; i < 64; i ++) {
         if (checkPresent(i, agent)) {
            if (isValidMove(i, i + 7, agent)) {
               result.first[result.second] = move(agent, i, i + 7);
               result.second += 1;
            }
            if (isValidMove(i, i + 8, agent)) {
               result.first[result.second] = move(agent, i, i + 8);
               result.second += 1;
            }
            if (isValidMove(i, i + 9, agent)) {
               result.first[result.second] = move(agent, i, i + 9);
               result.second += 1;
            }
         }
      }
      return result;
   }
}

bool Bitboard::isPlayerWin() {
   return (0xFF00000000000000UL & player) > 0;
}
bool Bitboard::isAIWin() {
   return (0x00000000000000FFUL & ai) > 0;
}

float Bitboard::evalute() {
   return countPieces(Agent::AI) - countPieces(Agent::player);
}

int main() {
   Bitboard test(0x000000000000FFFFUL, 0xFFFF000000000000UL);
}