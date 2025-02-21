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
    unordered_map<string, bool> data;
public:
    void SetValue(const string& key, bool value);
    bool GetValue(const string& key);
};

class BTNode {
protected:
    Grid grid;
public:
    virtual ~BTNode() = default;
    virtual NodeState Execute(const float& deltaTime) = 0;
};

class SequenceNode : public BTNode {
private:
    vector<unique_ptr<BTNode>> children;
public:
    void AddChild(unique_ptr<BTNode> child);
    NodeState Execute(const float& deltaTime) override;
};

class SelectorNode : public BTNode {
private:
    vector<unique_ptr<BTNode>> children;
public:
    void AddChild(unique_ptr<BTNode> child);
    NodeState Execute(const float& deltaTime) override;
};

class ConditionNode : public BTNode {
private:
    Blackboard& blackboard;
    string key;
    bool expectedValue;
public:
    ConditionNode(Blackboard& bb, const string& key, int value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute(const float& deltaTime) override;
};

class CheckEnemyProximityNode : public BTNode {
private:
    Blackboard& blackboard;
    string key;
    bool expectedValue;
public:
    CheckEnemyProximityNode(Blackboard& bb, const string& key, bool value) : blackboard(bb), key(key), expectedValue(value) {}
    NodeState Execute(const float& deltaTime) override;
};

class chaseNode : public BTNode {
private:
    shared_ptr<Enemy> enemy;
public:
    chaseNode(shared_ptr<Enemy> enemy) : enemy(enemy) {}
    NodeState Execute(const float& deltaTime) override;
};

class attackNode : public BTNode {
private:
    shared_ptr<Enemy> enemy;
public:
    attackNode(shared_ptr<Enemy> enemy) : enemy(enemy) {}
    NodeState Execute(const float& deltaTime) override;
};

class fleeNode : public BTNode {
private:
    shared_ptr<Enemy> enemy;
public:
    fleeNode(shared_ptr<Enemy> enemy) : enemy(enemy) {}
    NodeState Execute(const float& deltaTime) override;
};

class patrolNode : public BTNode {
private:
    shared_ptr<Enemy> enemy;
public:
    patrolNode(shared_ptr<Enemy> enemy) : enemy(enemy) {}
    NodeState Execute(const float& deltaTime) override;
};

class PrintMessageNode : public BTNode {
private:
    string message;
public:
    PrintMessageNode(const string& msg) : message(msg) {}
    NodeState Execute(const float& deltaTime) override;
};

// Méthode douteuse, c'est vrai... Mais bon au moins ça marche
class InheritFromEveryone : public virtual SelectorNode, public virtual SequenceNode, public Blackboard {
public:
    void makeTree(unique_ptr<SelectorNode>& root1, Blackboard& bb, shared_ptr<Enemy>& _enemy, bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid);
    void executeTree(unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP, const float& deltaTime);
};