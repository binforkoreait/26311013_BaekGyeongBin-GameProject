#include "ForestBattle.h"

#include <algorithm>

ForestBattle::ForestBattle() : random(std::random_device{}())
{
}

void ForestBattle::Start()
{
    menu = Menu::Main;
    result = Result::None;
    selection = 0;
    playerHp = 100;
    enemyHp = 50;
    potions = 3;
    superPotions = 1;
    enemyTurnPending = false;
    message = "숲의 야수가 나타났다! 행동을 선택하세요.";
}

void ForestBattle::MoveUp()
{
    int count = 0;
    if (menu == Menu::Main || menu == Menu::Attack)
    {
        count = 3;
    }
    else if (menu == Menu::Item || menu == Menu::Pause || menu == Menu::Finished)
    {
        count = 2;
    }
    if (count > 0)
    {
        if (selection == 0)
        {
            selection = count - 1;
        }
        else
        {
            selection--;
        }
    }
}

void ForestBattle::MoveDown()
{
    int count = 0;
    if (menu == Menu::Main || menu == Menu::Attack)
    {
        count = 3;
    }
    else if (menu == Menu::Item || menu == Menu::Pause || menu == Menu::Finished)
    {
        count = 2;
    }
    if (count > 0)
    {
        if (selection == count - 1)
        {
            selection = 0;
        }
        else
        {
            selection++;
        }
    }
}

void ForestBattle::Confirm()
{
    if (menu == Menu::Message)
    {
        AdvanceMessage();
        return;
    }
    if (menu == Menu::Main)
    {
        if (selection == 0)
        {
            menu = Menu::Attack;
        }
        else if (selection == 1)
        {
            menu = Menu::Item;
        }
        else
        {
            RunAway();
            return;
        }
        selection = 0;
    }
    else if (menu == Menu::Attack)
    {
        Attack();
    }
    else if (menu == Menu::Item)
    {
        UseItem();
    }
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
    if (menu != Menu::Main)
    {
        return;
    }
    selection = 0;
    Attack();
}

void ForestBattle::QuickPotion()
{
    if (menu != Menu::Main)
    {
        return;
    }
    selection = 0;
    UseItem();
}

void ForestBattle::QuickRun()
{
    if (menu != Menu::Main)
    {
        return;
    }
    RunAway();
}

void ForestBattle::Attack()
{
    int damage = 20;
    std::string text = "기본 공격! 20의 피해를 입혔습니다.";
    if (selection == 1)
    {
        if (RandomPercent() >= 75)
        {
            FinishPlayerTurn("강한 공격이 빗나갔습니다!");
            return;
        }
        damage = 30;
        text = "강한 공격! 30의 피해를 입혔습니다.";
    }
    else if (selection == 2)
    {
        damage = 10;
        text = "흡수 공격! 10의 피해를 입혔습니다.";
    }

    int actualDamage = (std::min)(enemyHp, damage);
    enemyHp -= actualDamage;
    if (selection == 2)
    {
        int healing = actualDamage / 2;
        playerHp = (std::min)(100, playerHp + healing);
        text += " 체력 " + std::to_string(healing) + " 회복.";
    }
    if (enemyHp == 0)
    {
        result = Result::Victory;
        menu = Menu::Finished;
        selection = 0;
        message = text + " 숲의 야수를 물리쳤습니다!";
        return;
    }
    FinishPlayerTurn(text);
}

void ForestBattle::UseItem()
{
    if (playerHp == 100)
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
        potions--;
        playerHp = (std::min)(100, playerHp + 30);
        FinishPlayerTurn("회복약을 사용해 체력을 회복했습니다.");
    }
    else
    {
        if (superPotions == 0)
        {
            message = "고급 회복약이 없습니다.";
            return;
        }
        superPotions--;
        playerHp = (std::min)(100, playerHp + 60);
        FinishPlayerTurn("고급 회복약을 사용해 체력을 회복했습니다.");
    }
}

void ForestBattle::RunAway()
{
    if (RandomPercent() < 60)
    {
        result = Result::Escaped;
        menu = Menu::Finished;
        selection = 0;
        message = "도망에 성공했습니다. 전투를 마칩니다.";
    }
    else
    {
        FinishPlayerTurn("도망에 실패했습니다!");
    }
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
        playerHp = (std::max)(0, playerHp - 10);
        message = "숲의 야수가 공격해 10의 피해를 받았습니다.";
        if (playerHp == 0)
        {
            result = Result::Defeat;
            menu = Menu::Finished;
            selection = 0;
            message += " 게임 오버.";
        }
    }
    else
    {
        menu = Menu::Main;
        selection = 0;
        message = "다음 행동을 선택하세요.";
    }
}

int ForestBattle::RandomPercent()
{
    std::uniform_int_distribution<int> distribution(0, 99);
    return distribution(random);
}

ForestBattle::Menu ForestBattle::CurrentMenu() const
{
    return menu;
}
ForestBattle::Result ForestBattle::CurrentResult() const
{
    return result;
}
int ForestBattle::Selection() const
{
    return selection;
}
int ForestBattle::PlayerHp() const
{
    return playerHp;
}
int ForestBattle::EnemyHp() const
{
    return enemyHp;
}
int ForestBattle::Potions() const
{
    return potions;
}
int ForestBattle::SuperPotions() const
{
    return superPotions;
}
const std::string &ForestBattle::Message() const
{
    return message;
}
