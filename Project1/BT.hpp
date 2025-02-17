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
    int GetValue(const std::string& key);
};

class BTNode {
public:
    Grid grid;
    virtual ~BTNode() = default;
    virtual NodeState Execute() = 0;

    static void makeTree(Enemy& enemy, int playerDetected, int playerInsight, int lowHP);
};

class SequenceNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState Execute() override;
};

class SelectorNode : public BTNode {
private:
    std::vector<std::unique_ptr<BTNode>> children;
public:
    void AddChild(std::unique_ptr<BTNode> child);
    NodeState Execute() override;
};

class Blackboard {
private:
    std::unordered_map<std::string, int> data;
public:
    void SetValue(const std::string& key, int value);
    int GetValue(const std::string& key);
};

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    int expectedValue;
public:
    ConditionNode(Blackboard& bb, const std::string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute() override;
};

class CheckEnemyProximityNode : public BTNode {
private:
    Blackboard& blackboard;
    std::string key;
    int expectedValue;
public:
    CheckEnemyProximityNode(Blackboard& bb, const std::string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute() override;
};

class chaseNode : public BTNode {
private:    
    Enemy& enemy;
public:
    chaseNode(Enemy& enemy) : enemy(enemy) {}
    NodeState Execute() override;
};

class attackNode : public BTNode {
private:
    Enemy& enemy;
public:
    attackNode(Enemy& enemy) : enemy(enemy) {}
    NodeState Execute() override;
};

class fleeNode : public BTNode {
private:
    Enemy& enemy;
public:
    fleeNode(Enemy& enemy) : enemy(enemy) {}
    NodeState Execute() override;
};

class patrolNode : public BTNode {
private:
    Enemy& enemy;
public:
    patrolNode(Enemy& enemy) : enemy(enemy) {}
    NodeState Execute() override;
};

class PrintMessageNode : public BTNode {
private:
    std::string message;
public:
    PrintMessageNode(const std::string& msg) : message(msg) {}
    NodeState Execute() override;
};

static void makeTree(std::unique_ptr<SelectorNode>& root, std::unique_ptr<SequenceNode>& sequence, Blackboard& bb, Enemy& enemy, 
    bool& playerDetected, bool& playerInsight, bool& lowHP);

static void updateBlackboard(Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP);

static void executeTree(std::unique_ptr<SelectorNode>& root);