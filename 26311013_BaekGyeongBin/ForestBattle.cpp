#include "ForestBattle.h"

#include <algorithm>

ForestBattle::ForestBattle() : random(std::random_device{}()) {}

void ForestBattle::Start()
{
    result = Result::None;
    selection = 0;
    playerHp = playerMaxHp = 100;
    playerAttack = 20;
    potions = 3;
    superPotions = 1;
    score = 0;
    regularCompleted = 0;
    bossTurn = 0;
    ruinsVisited = caveVisited = enemyTurnPending = false;
    SetupEncounter(Biome::Forest);
}

int ForestBattle::MenuItemCount() const
{
    if (menu == Menu::Main || menu == Menu::Attack || menu == Menu::Reward)
        return 3;
    if (menu == Menu::Item || menu == Menu::BiomeSelect || menu == Menu::Pause ||
        menu == Menu::Finished)
        return 2;
    return 0;
}

void ForestBattle::MoveUp()
{
    const int count = MenuItemCount();
    if (count > 0)
        selection = (selection + count - 1) % count;
}

void ForestBattle::MoveDown()
{
    const int count = MenuItemCount();
    if (count > 0)
        selection = (selection + 1) % count;
}

void ForestBattle::MoveLeft() { MoveUp(); }
void ForestBattle::MoveRight() { MoveDown(); }

void ForestBattle::Confirm()
{
    if (menu == Menu::Message)
        return AdvanceMessage();
    if (menu == Menu::BiomeSelect)
        return SelectBiome();
    if (menu == Menu::Reward)
        return SelectReward();
    if (menu == Menu::Main)
    {
        if (selection == 0)
            menu = Menu::Attack;
        else if (selection == 1)
            menu = Menu::Item;
        else
            return RunAway();
        selection = 0;
    }
    else if (menu == Menu::Attack)
        Attack();
    else if (menu == Menu::Item)
        UseItem();
}

void ForestBattle::Back()
{
    if (menu == Menu::Attack || menu == Menu::Item)
    {
        menu = Menu::Main;
        selection = 0;
    }
    else if (menu == Menu::Main)
    {
        menu = Menu::Pause;
        selection = 0;
    }
    else if (menu == Menu::Pause)
    {
        menu = Menu::Main;
        selection = 0;
    }
}

void ForestBattle::QuickAttack()
{
    if (menu == Menu::Main)
    {
        selection = 0;
        Attack();
    }
}

void ForestBattle::QuickPotion()
{
    if (menu == Menu::Main)
    {
        selection = 0;
        UseItem();
    }
}

void ForestBattle::QuickRun()
{
    if (menu == Menu::Main)
        RunAway();
}

void ForestBattle::SelectBiome()
{
    const Biome next = selection == 0 ? Biome::Ruins : Biome::Cave;
    ruinsVisited = next == Biome::Ruins;
    caveVisited = next == Biome::Cave;
    SetupEncounter(next);
}

void ForestBattle::SelectReward()
{
    if (selection == 0)
    {
        playerAttack += 5;
        message = "공격 강화 획득! 공격력이 5 증가했습니다.";
    }
    else if (selection == 1)
    {
        playerMaxHp += 20;
        playerHp += 20;
        message = "체력 강화 획득! 최대 체력과 현재 체력이 20 증가했습니다.";
    }
    else
    {
        potions += 2;
        message = "도구 보급 획득! 회복약 2개를 받았습니다.";
    }
    ContinueAfterEncounter();
}

void ForestBattle::SetupEncounter(Biome nextBiome)
{
    biome = nextBiome;
    result = Result::None;
    selection = 0;
    enemyTurnPending = false;
    if (biome == Biome::Forest)
    {
        biomeName = "숲";
        enemyName = "숲의 야수";
        enemyMaxHp = 50;
        enemyAttack = 10;
    }
    else if (biome == Biome::Ruins)
    {
        biomeName = "폐허";
        enemyName = "폐허의 수호자";
        enemyMaxHp = 70 + regularCompleted * 10;
        enemyAttack = 12 + regularCompleted * 2;
    }
    else if (biome == Biome::Cave)
    {
        biomeName = "동굴";
        enemyName = "동굴의 박쥐";
        enemyMaxHp = 40 + regularCompleted * 10;
        enemyAttack = 14 + regularCompleted * 2;
    }
    else
    {
        biomeName = "성채";
        enemyName = "성채 보스";
        enemyMaxHp = 200;
        enemyAttack = 18;
        bossTurn = 0;
    }
    enemyHp = enemyMaxHp;
    menu = Menu::Main;
    message = biomeName + "에서 " + enemyName + "이(가) 나타났습니다!";
}

void ForestBattle::ContinueAfterEncounter()
{
    if (regularCompleted == 1)
    {
        menu = Menu::BiomeSelect;
        selection = 0;
        message = "다음에 탐험할 지역을 선택하세요.";
    }
    else if (regularCompleted == 2)
    {
        const Biome remaining = ruinsVisited ? Biome::Cave : Biome::Ruins;
        ruinsVisited = ruinsVisited || remaining == Biome::Ruins;
        caveVisited = caveVisited || remaining == Biome::Cave;
        SetupEncounter(remaining);
    }
    else if (regularCompleted == 3)
        SetupEncounter(Biome::Citadel);
}

void ForestBattle::CompleteEncounter(bool defeatedEnemy)
{
    enemyTurnPending = false;
    if (IsBoss())
    {
        if (defeatedEnemy)
        {
            score += 1000;
            result = Result::Clear;
            message = "성채 보스를 물리쳤습니다! 던전 탈출 성공!";
        }
        menu = Menu::Finished;
        selection = 0;
        return;
    }

    ++regularCompleted;
    if (defeatedEnemy)
    {
        score += 100;
        result = Result::Victory;
        menu = Menu::Reward;
        selection = 0;
        message = enemyName + " 격파! 승리 보상을 하나 선택하세요.";
    }
    else
    {
        result = Result::Escaped;
        ContinueAfterEncounter();
    }
}

void ForestBattle::Attack()
{
    int damage = playerAttack;
    std::string text = "기본 공격! " + std::to_string(damage) + "의 피해.";
    if (selection == 1)
    {
        if (RandomPercent() >= 75)
            return FinishPlayerTurn("강한 공격이 빗나갔습니다!");
        damage = playerAttack * 3 / 2;
        text = "강한 공격! " + std::to_string(damage) + "의 피해.";
    }
    else if (selection == 2)
    {
        damage = playerAttack / 2;
        text = "흡수 공격! " + std::to_string(damage) + "의 피해.";
    }

    const int actualDamage = (std::min)(enemyHp, damage);
    enemyHp -= actualDamage;
    if (selection == 2)
    {
        const int healing = actualDamage / 2;
        playerHp = (std::min)(playerMaxHp, playerHp + healing);
        text += " 체력 " + std::to_string(healing) + " 회복.";
    }
    if (enemyHp == 0)
    {
        message = text;
        return CompleteEncounter(true);
    }
    FinishPlayerTurn(text);
}

void ForestBattle::UseItem()
{
    if (playerHp == playerMaxHp)
    {
        message = "체력이 가득 차 있어 물약을 사용하지 않았습니다.";
        return;
    }
    if (selection == 0)
    {
        if (potions == 0)
        {
            message = "회복약이 없습니다.";
            return;
        }
        --potions;
        playerHp = (std::min)(playerMaxHp, playerHp + 30);
        FinishPlayerTurn("회복약을 사용해 체력을 30 회복했습니다.");
    }
    else
    {
        if (superPotions == 0)
        {
            message = "고급 회복약이 없습니다.";
            return;
        }
        --superPotions;
        playerHp = (std::min)(playerMaxHp, playerHp + 60);
        FinishPlayerTurn("고급 회복약을 사용해 체력을 60 회복했습니다.");
    }
}

void ForestBattle::RunAway()
{
    if (IsBoss())
    {
        message = "보스전에서는 도망칠 수 없습니다.";
        return;
    }
    if (RandomPercent() < 60)
    {
        message = "도망에 성공했습니다. 보상 없이 다음 지역으로 이동합니다.";
        CompleteEncounter(false);
    }
    else
        FinishPlayerTurn("도망에 실패했습니다!");
}

void ForestBattle::FinishPlayerTurn(const std::string &text)
{
    message = text;
    enemyTurnPending = true;
    menu = Menu::Message;
}

void ForestBattle::AdvanceMessage()
{
    if (enemyTurnPending)
    {
        enemyTurnPending = false;
        int damage = enemyAttack;
        ++bossTurn;
        if (IsBoss() && bossTurn % 3 == 0)
            damage = 27;
        playerHp = (std::max)(0, playerHp - damage);
        message = enemyName + "의 공격! " + std::to_string(damage) + "의 피해를 받았습니다.";
        if (playerHp == 0)
        {
            result = Result::Defeat;
            menu = Menu::Finished;
            selection = 0;
            message += " 도전이 끝났습니다.";
        }
        else if (IsBoss() && bossTurn % 3 == 2)
            message += " 보스가 다음 턴 강한 공격을 준비합니다!";
    }
    else
    {
        menu = Menu::Main;
        selection = 0;
        message = IsBoss() && bossTurn % 3 == 2
                      ? "경고: 보스의 다음 공격은 강한 공격입니다!"
                      : "다음 행동을 선택하세요.";
    }
}

int ForestBattle::RandomPercent()
{
    std::uniform_int_distribution<int> distribution(0, 99);
    return distribution(random);
}

ForestBattle::Menu ForestBattle::CurrentMenu() const { return menu; }
ForestBattle::Result ForestBattle::CurrentResult() const { return result; }
ForestBattle::Biome ForestBattle::CurrentBiome() const { return biome; }
int ForestBattle::Selection() const { return selection; }
int ForestBattle::PlayerHp() const { return playerHp; }
int ForestBattle::PlayerMaxHp() const { return playerMaxHp; }
int ForestBattle::PlayerAttack() const { return playerAttack; }
int ForestBattle::EnemyHp() const { return enemyHp; }
int ForestBattle::EnemyMaxHp() const { return enemyMaxHp; }
int ForestBattle::Potions() const { return potions; }
int ForestBattle::SuperPotions() const { return superPotions; }
int ForestBattle::Score() const { return score; }
int ForestBattle::EncounterNumber() const { return IsBoss() ? 4 : regularCompleted + 1; }
bool ForestBattle::IsBoss() const { return biome == Biome::Citadel; }
const std::string &ForestBattle::EnemyName() const { return enemyName; }
const std::string &ForestBattle::BiomeName() const { return biomeName; }
const std::string &ForestBattle::Message() const { return message; }
