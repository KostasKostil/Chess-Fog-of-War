#pragma once
#include "players.h"
#include <SFML/Graphics.hpp>

void Visualize(std::string filename);
void VisPlay(std::shared_ptr<Player> p);
void PlayStockfish(int depth);
