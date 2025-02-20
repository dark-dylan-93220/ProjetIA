#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <string>

#include "Enemy.hpp"

enum class NodeState { SUCCESS, FAILURE, RUNNING };

class Blackboard {
private:
    std::unordered_map<std::string, bool> data;
public:
    void SetValue(const std::string& key, bool value);
    bool GetValue(const std::string& key);
};

class BTNode {
protected:
    Grid grid;
public:
    virtual ~BTNode() = default;
    virtual void addGrid(Grid& _grid) = 0;
    virtual NodeState Execute(const float& deltaTime) = 0;
};

class SequenceNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class SelectorNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    bool expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class CheckEnemyProximityNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    bool expectedValue;
public:
    CheckEnemyProximityNode(Blackboard& bb, const std::string& key, bool value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute(const float& deltaTime) override;
};

class chaseNode : public BTNode {
private:
    std::shared_ptr<Enemy> enemy;
public:
    chaseNode(std::shared_ptr<Enemy> enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class attackNode : public BTNode {
private:
    std::shared_ptr<Enemy> enemy;
public:
    attackNode(std::shared_ptr<Enemy> enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class fleeNode : public BTNode {
private:
    std::shared_ptr<Enemy> enemy;
public:
    fleeNode(std::shared_ptr<Enemy> enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class patrolNode : public BTNode {
private:
    std::shared_ptr<Enemy> enemy;
public:
    patrolNode(std::shared_ptr<Enemy> enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute(const float& deltaTime) override;
};

class PrintMessageNode : public BTNode {
private:
    std::string message;
public:
    PrintMessageNode(const std::string& msg) : message(msg) {}
    NodeState Execute(const float& deltaTime) override;
};

// Méthode douteuse, c'est vrai... Mais bon au moins ça marche
class InheritFromEveryone : public virtual SelectorNode, public virtual SequenceNode, public Blackboard {
public:
    void makeTree(std::unique_ptr<SelectorNode>& root1, Blackboard& bb, std::shared_ptr<Enemy>& _enemy, bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid);
    void executeTree(std::unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP, const float& deltaTime, Enemy& enemy);
};