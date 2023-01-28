#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <random>

class Player {
private:
  std::string name;

public:
  std::vector<std::string> hand;

  int pairs;
  Player(std::string name) : name(name), pairs(0) {}

  std::string getName() { return name; }

  void printDeck() {
    std::cout << "\n\033[0;39mYour Cards: ";
    for (int i = 0; i < this->hand.size(); i++) {
      std::cout << this->hand[i] << " ";
    }
    std::cout << "\033[0;39m" << std::endl;
  }
};

class Bot {
public:  
  int pairs;
  std::vector<std::string> hand;

  Bot() { this->pairs = 0; }
};

class Deck {
private:
  std::vector<std::string> cards;

public:
  Deck() {
    // initialize deck with cards
    for (int i = 2; i <= 14; i++) {
        cards.insert(cards.end(), {std::to_string(i), std::to_string(i), std::to_string(i), std::to_string(i)});
    }
  }

  std::vector<std::string> getCards() { return cards; }

  void removeCard(std::string card) {
    // find index of card in vector and remove it
    int index = -1;
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i] == card) {
        index = i;
        break;
      }
    }

    if (index != -1) {
      cards.erase(cards.begin() + index);
    } else {
      throw std::invalid_argument("Card not found in the Deck");
    }
  }
};
void printLeaderboard(Player* player, Bot bot) {
    std::vector<std::pair<std::string, int>> leaderboard;
    leaderboard.push_back(std::make_pair(player->getName(), player->pairs));
    leaderboard.push_back(std::make_pair("Bot", bot.pairs));

    // Sort the leaderboard by pairs in descending order
    std::sort(leaderboard.begin(), leaderboard.end(),
        [](const std::pair<std::string, int> &leaderboardA, const std::pair<std::string, int> &leaderboardB) {
            return leaderboardA.second > leaderboardB.second;
        });

    std::cout << "\033[2mLeaderboard:\033[0m" << std::endl;
    for (auto const &entry : leaderboard) {
        std::cout << entry.first << ": " << entry.second << " pairs" << std::endl;
    }
}



void printInstructions(){
  std::string inputInstructions;
  std::cout << "Would you like to see the game instructions for Go Fish? (Y/N): " << std::endl;
  std::cin >> inputInstructions;
  if (inputInstructions == "Y" || inputInstructions == "y"){
    std::cout << "\nThe following is how the game works You will be the player and face a bot. You will go first and ask the bot for a card, if the bot has the card, you will increase your number of pairs. If the bot does not have the requested card, you will draw a card. This pattern will continue with the player and bot alternating turns until there are no more cards remaining in the deck. The player with the most pairs at the end will win!" << std::endl;
  }
}
int main() {
  std::cout << "\033[4;1;91mWelcome to Go Fish!\033[0;39m" << std::endl;

  std::cout << std::endl;
  std::string name;
  std::cout << "\033[31mWhat is your name? Enter here: ";
  std::cin >> name;
  std::cout << " " << std::endl;
  system("clear");
  printInstructions();
  // initialize player
  Player *player = new Player(name);
  // initialize bot
  Bot bot;

  // initialize deck
  Deck deck;

  // std::mt19937 engine(time(NULL));
  // std::uniform_int_distribution<int> dist(0, deck.getCards().size() - 1);
  

  // give the player and bot some cards
	srand(time(NULL));
  for (int i = 0; i < 5; i++) {
    
    // int randomIndex = dist(engine);
		srand(rand());
		int randomIndex = rand() % deck.getCards().size();
    player->hand.push_back(deck.getCards()[randomIndex]);
    deck.removeCard(deck.getCards()[randomIndex]);

		srand(rand());
    randomIndex = rand() % deck.getCards().size();
    bot.hand.push_back(deck.getCards()[randomIndex]);
    deck.removeCard(deck.getCards()[randomIndex]);
  }

  // game loop
  std::string card;
  while (deck.getCards().size() > 0) {
    // player's turn
    player->printDeck();
    std::cout << "\033[0;92mPlease select a card to ask for: \033[0;39m";
    std::cin >> card;

    std::cout << "";

    // check if card is in bot's hand
    if (std::find(bot.hand.begin(), bot.hand.end(), card) != bot.hand.end()) {
      std::cout << "\n\033[1;32mYou got a match!\033[0;39m" << std::endl;
      player->pairs++;
      bot.hand.erase(std::remove(bot.hand.begin(), bot.hand.end(), card),
                     bot.hand.end());
      player->hand.push_back(card);
    } else {
      std::cout << "\n\033[1;91mGo Fish!\033[0;39m" << std::endl;
      // draw a card from the deck
      if (deck.getCards().size() > 0) {
        int randomIndex = rand() % deck.getCards().size();
        std::cout << std::endl;
        std::cout << "\033[0;39mYou drew a \033[1m" << deck.getCards()[randomIndex] << "\033[0;39m" << std::endl;
        std::cout << std::endl;
        player->hand.push_back(deck.getCards()[randomIndex]);
        deck.removeCard(deck.getCards()[randomIndex]);
      } else {
        std::cout << "\033[1;31mDeck is empty!\033[0;39m" << std::endl;
      }

      // check for pairs in player's hand
      for (int i = 0; i < player->hand.size(); i++) {
        for (int j = i + 1; j < player->hand.size(); j++) {
          if (player->hand[i] == player->hand[j]) {
            std::cout << "\033[0;39mYou have a \033[1;92mpair of " << player->hand[i] << "s\033[0;39m!"
                      << std::endl;
            player->pairs++;
            player->hand.erase(player->hand.begin() + j);
            player->hand.erase(player->hand.begin() + i);
            i--;
            break;
          }
        }
      }

          // bot's turn
		srand(rand());
    int randomIndex = rand() % bot.hand.size();
    std::string randomCard = bot.hand[randomIndex];
    std::cout << "\n\033[0;31mThe bot asks if you have a " << randomCard << "\033[0;39m" << std::endl;

    if (std::find(player->hand.begin(), player->hand.end(), randomCard) != player->hand.end()) {
        std::cout << "\033[0;31mThe bot got a match\033[0;39m" << std::endl;
        bot.pairs++;
        player->hand.erase(std::remove(player->hand.begin(), player->hand.end(), randomCard), player->hand.end());
        bot.hand.push_back(deck.getCards()[randomIndex]);
    } else {
        std::cout << "\033[0;31mThere is not a match. The Bot picks up a card.\033[0;39m" << std::endl;
      	srand(rand());  
				int randomCardIndex = rand() % deck.getCards().size();
        bot.hand.push_back(deck.getCards()[randomCardIndex]);
        deck.removeCard(deck.getCards()[randomCardIndex]);
    }




      // check for pairs in bot's hand
      for (int i = 0; i < bot.hand.size(); i++) {
        for (int j = i + 1; j < bot.hand.size(); j++) {
          if (bot.hand[i] == bot.hand[j]) {
            std::cout << "\033[0;31mBot has a pair of " << bot.hand[i] << "s!\033[0;39m"
                      << std::endl;
            bot.pairs++;
            bot.hand.erase(bot.hand.begin() + j);
            bot.hand.erase(bot.hand.begin() + i);
            i--;
            break;
          }
        }
      }
      std::cout << std::endl;
      //print leaderboard
      printLeaderboard(player, bot);
      //print amounts
      std::cout << std::endl;
      std::cout << "\033[0;31Bot has " << bot.hand.size() << " Cards.\033[0;39m"<< std::endl;
      std::cout << deck.getCards().size() << " Cards remain in the deck" << std::endl;
    }
  }
  // determine winner
  if (player->pairs > bot.pairs) {
    std::cout << "\033[1;92mCongratulations, " << player->getName() << "! You won with "
              << player->pairs << " pairs." << std::endl;
  } else if (player->pairs < bot.pairs) {
    std::cout << "\033[1;31mSorry, " << player->getName() << ". The bot won with "
              << bot.pairs << " pairs." << std::endl;
  } else {
    std::cout << "\033[1;92mIt's a tie! Both " << player->getName() << " and the bot had "
              << player->pairs << " pairs." << std::endl;
  }

  return 0;
}
