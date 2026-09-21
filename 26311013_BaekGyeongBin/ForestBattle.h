#pragma once

#include <random>
#include <string>

// 한 번의 도전 전체(일반 전투 3회 + 보스전)를 관리한다.
class ForestBattle
{
  public:
    enum class Menu
    {
        Main,
        Attack,
        Item,
        BiomeSelect,
        Reward,
        Pause,
        Message,
        Finished
    };

    enum class Result
    {
        None,
        Victory,
        Defeat,
        Escaped,
        Clear
    };

    enum class Biome
    {
        Forest,
        Ruins,
        Cave,
        Citadel
    };

    ForestBattle();
    void Start();
    void MoveUp();
    void MoveDown();
    void MoveLeft();
    void MoveRight();
    void Confirm();
    void Back();
    void QuickAttack();
    void QuickPotion();
    void QuickRun();

    Menu CurrentMenu() const;
    Result CurrentResult() const;
    Biome CurrentBiome() const;
    int Selection() const;
    int PlayerHp() const;
    int PlayerMaxHp() const;
    int PlayerAttack() const;
    int EnemyHp() const;
    int EnemyMaxHp() const;
    int Potions() const;
    int SuperPotions() const;
    int Score() const;
    int EncounterNumber() const;
    bool IsBoss() const;
    const std::string &EnemyName() const;
    const std::string &BiomeName() const;
    const std::string &Message() const;

  private:
    void SelectBiome();
    void SelectReward();
    void SetupEncounter(Biome nextBiome);
    void ContinueAfterEncounter();
    void CompleteEncounter(bool defeatedEnemy);
    void Attack();
    void UseItem();
    void RunAway();
    void FinishPlayerTurn(const std::string &text);
    void AdvanceMessage();
    int RandomPercent();
    int MenuItemCount() const;

    std::mt19937 random;
    Menu menu = Menu::Main;
    Result result = Result::None;
    Biome biome = Biome::Forest;
    int selection = 0;
    int playerHp = 100;
    int playerMaxHp = 100;
    int playerAttack = 20;
    int enemyHp = 50;
    int enemyMaxHp = 50;
    int enemyAttack = 10;
    int potions = 3;
    int superPotions = 1;
    int score = 0;
    int regularCompleted = 0;
    int bossTurn = 0;
    bool ruinsVisited = false;
    bool caveVisited = false;
    bool enemyTurnPending = false;
    std::string enemyName;
    std::string biomeName;
    std::string message;
};
