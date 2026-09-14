#pragma once

#include <random>
#include <string>

class ForestBattle
{
  public:
    enum class Menu
    {
        Main,
        Attack,
        Item,
        Pause,
        Message,
        Finished
    };

    enum class Result
    {
        None,
        Victory,
        Defeat,
        Escaped
    };

    ForestBattle();

    void Start();
    void MoveUp();
    void MoveDown();
    void Confirm();
    void Back();
    void QuickAttack();
    void QuickPotion();
    void QuickRun();

    Menu CurrentMenu() const;
    Result CurrentResult() const;
    int Selection() const;
    int PlayerHp() const;
    int EnemyHp() const;
    int Potions() const;
    int SuperPotions() const;
    const std::string &Message() const;

  private:
    void Attack();
    void UseItem();
    void RunAway();
    void FinishPlayerTurn(const std::string &message);
    void AdvanceMessage();
    int RandomPercent();

    std::mt19937 random;
    Menu menu = Menu::Main;
    Result result = Result::None;
    int selection = 0;
    int playerHp = 100;
    int enemyHp = 50;
    int potions = 3;
    int superPotions = 1;
    bool enemyTurnPending = false;
    std::string message;
};
