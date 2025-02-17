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
    virtual NodeState Execute() = 0;
};

class SequenceNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class SelectorNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    bool expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, bool value) : blackboard(bb), key(key), expectedValue(value) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class CheckEnemyProximityNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    bool expectedValue;
public:
    CheckEnemyProximityNode(Blackboard& bb, const std::string& key, bool value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute() override;
};

class chaseNode : public BTNode {
private:
    Enemy& enemy;
public:
    chaseNode(Enemy& enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class attackNode : public BTNode {
private:
    Enemy& enemy;
public:
    attackNode(Enemy& enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class fleeNode : public BTNode {
private:
    Enemy& enemy;
public:
    fleeNode(Enemy& enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class patrolNode : public BTNode {
private:
    Enemy& enemy;
public:
    patrolNode(Enemy& enemy) : enemy(enemy) {}
    void addGrid(Grid& _grid) override;
    NodeState Execute() override;
};

class PrintMessageNode : public BTNode {
private:
    std::string message;
public:
    PrintMessageNode(const std::string& msg) : message(msg) {}
    NodeState Execute() override;
};

// Méthode douteuse, c'est vrai... Mais bon au moins ça marche
class InheritFromEveryone : public SelectorNode, public SequenceNode, public Blackboard {
public:
    static void makeTree(std::unique_ptr<SelectorNode>& root, std::unique_ptr<SelectorNode>& root2, std::unique_ptr<SequenceNode>& sequence, std::unique_ptr<SequenceNode>& sequence2, std::unique_ptr<SequenceNode>& sequence3, std::unique_ptr<SequenceNode>& sequence4, Blackboard& bb, Enemy& enemy,
        bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid);

    static void executeTree(std::unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP);
};