#include "../26311013_BaekGyeongBin/ForestBattle.h"

#include <cassert>

int main()
{
    ForestBattle battle;
    battle.Start();
    assert(battle.PlayerHp() == 100);
    assert(battle.EnemyHp() == 50);
    assert(battle.Potions() == 3);

    battle.MoveDown();
    battle.Confirm();
    assert(battle.CurrentMenu() == ForestBattle::Menu::Item);
    battle.Confirm();
    assert(battle.Potions() == 3);
    assert(battle.PlayerHp() == 100);
    battle.Back();

    battle.Confirm();
    assert(battle.CurrentMenu() == ForestBattle::Menu::Attack);
    battle.Confirm();
    assert(battle.EnemyHp() == 30);
    battle.Confirm();
    assert(battle.PlayerHp() == 90);
    battle.Confirm();

    battle.Confirm();
    battle.Confirm();
    assert(battle.EnemyHp() == 10);
    battle.Confirm();
    assert(battle.PlayerHp() == 80);
    battle.Confirm();

    battle.Confirm();
    battle.Confirm();
    assert(battle.EnemyHp() == 0);
    assert(battle.PlayerHp() == 80);
    assert(battle.CurrentResult() == ForestBattle::Result::Victory);

    battle.Start();
    assert(battle.PlayerHp() == 100);
    assert(battle.EnemyHp() == 50);

    battle.QuickPotion();
    assert(battle.Potions() == 3);
    battle.QuickAttack();
    assert(battle.EnemyHp() == 30);
    assert(battle.CurrentMenu() == ForestBattle::Menu::Message);
    battle.QuickAttack();
    battle.QuickPotion();
    battle.QuickRun();
    assert(battle.EnemyHp() == 30);
    assert(battle.Potions() == 3);
    assert(battle.CurrentResult() == ForestBattle::Result::None);

    battle.Confirm();
    assert(battle.PlayerHp() == 90);
    battle.Confirm();
    battle.QuickPotion();
    assert(battle.PlayerHp() == 100);
    assert(battle.Potions() == 2);
    assert(battle.CurrentMenu() == ForestBattle::Menu::Message);
}
