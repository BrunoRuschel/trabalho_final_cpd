#include "printer.h"

auto ID_SIZE = 10;
auto SHORT_NAME_SIZE = 20;
auto LONG_NAME_SIZE = 40;
auto POSITIONS_SIZE = 15;
auto RATINGS_SIZE = 10;
auto COUNT_SIZE = 10;

void printPlayerInfo(const vector<JOGADOR>& players) {
    std::cout << std::setw(ID_SIZE) << "sofifa_id" << std::setw(SHORT_NAME_SIZE) << "short_name"
    << std::setw(LONG_NAME_SIZE) << "long_name" << setw(POSITIONS_SIZE) << "player_positions"
    << setw(RATINGS_SIZE) << "rating" << setw(COUNT_SIZE) << "count" << endl;

    std::cout << std::setw(ID_SIZE) << std::setfill('-') << " " << std::setw(SHORT_NAME_SIZE)
    << " " << std::setw(LONG_NAME_SIZE) << " " << std::setfill('-') << setw(POSITIONS_SIZE) << " "
    << setfill('-') << " " << setw(RATINGS_SIZE) << setfill('-') << " " << setw(COUNT_SIZE) << setfill('-') << " " << endl;

    // Print table rows
    for (const auto& player : players) {
        auto rating = player.count_ratings > 0 ? player.soma_ratings/player.count_ratings : 0;
        std::cout << std::setw(ID_SIZE) << player.id << std::setw(SHORT_NAME_SIZE) << player.short_name
                  << std::setw(LONG_NAME_SIZE) <<  player.long_name << setw(POSITIONS_SIZE) <<  player.posicoes
                  << setw(RATINGS_SIZE) <<  rating << setw(COUNT_SIZE) <<  player.count_ratings << endl;
    }
}
