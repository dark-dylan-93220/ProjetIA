#include "BT.hpp"

//--------------------------SequenceNode------------------------------
void SequenceNode::AddChild(unique_ptr<BTNode> child) {
    children.push_back(move(child));
}
NodeState SequenceNode::Execute(const float& deltaTime) {
    for (auto& child : children) {
        if (child->Execute(deltaTime) == NodeState::FAILURE) {
            return NodeState::FAILURE;
        }
    }
    return NodeState::SUCCESS;
}

//----------------------------SelectorNode---------------------------------
void SelectorNode::AddChild(unique_ptr<BTNode> child) {
    children.push_back(move(child));
}
NodeState SelectorNode::Execute(const float& deltaTime) {
    for (auto& child : children) {
        if (child->Execute(deltaTime) == NodeState::SUCCESS) {
            return NodeState::SUCCESS;
        }
    }
    return NodeState::FAILURE;
}

//----------------------------Blackboard---------------------------------
void Blackboard::SetValue(const string& key, bool value) {
    data[key] = value;
}
bool Blackboard::GetValue(const string& key) {
    return data[key];
}

//----------------------------ConditionNode---------------------------------
NodeState ConditionNode::Execute(const float& deltaTime)  {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

//----------------------------CheckEnemyProximityNode---------------------------------
NodeState CheckEnemyProximityNode::Execute(const float& deltaTime) {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

//----------------------------PrintMessageNode--------------------------
NodeState PrintMessageNode::Execute(const float& deltaTime) {
    cout << message << endl;
    return NodeState::SUCCESS;
}

//----------------------------ChaseNode---------------------------------
NodeState chaseNode::Execute(const float& deltaTime) {
    if (!enemy->lowHP) {
        enemy->enemyFollowsPath(deltaTime); //suivre
        return NodeState::SUCCESS;
    }
    return NodeState::FAILURE;
}

//----------------------------AttackNode--------------------------------
NodeState attackNode::Execute(const float& deltaTime) {
    enemy->attack(deltaTime); //attaquer
    return NodeState::SUCCESS;
}

NodeState fleeNode::Execute(const float& deltaTime) {
    if (enemy->playerDetected && enemy->lowHP) {
        enemy->enemyFollowsPath(deltaTime); //fuir
        cout << enemy << " fuit." << endl;
        return NodeState::SUCCESS;
    }
    return NodeState::FAILURE;
}

NodeState patrolNode::Execute(const float& deltaTime) {
    if (!enemy->playerDetected) {
        enemy->patrol(deltaTime); //patrouiller
        return NodeState::SUCCESS;
    }
    return NodeState::FAILURE;
}

void InheritFromEveryone::makeTree(unique_ptr<SelectorNode>& root, Blackboard& bb, shared_ptr<Enemy>& _enemy,
    bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid)
{
    //shared_ptr<Enemy> enemy = _enemy;

    // Inscription des "valeurs" sur le blackboard
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("flee", lowHP + playerDetected);

    // Noeuds racines
    auto root2 = make_unique<SelectorNode>();
    
    // LowHP
    auto sequence = make_unique<SequenceNode>();
    sequence->AddChild(make_unique<ConditionNode>(bb, "flee", 2));
    unique_ptr<fleeNode> fleeN = make_unique<fleeNode>(_enemy);
    sequence->AddChild(move(fleeN));

    // PlayerDetected
    auto sequence2 = make_unique<SequenceNode>();
    sequence2->AddChild(make_unique<ConditionNode>(bb, "playerDetected", 1));
    unique_ptr<chaseNode> chaseN = make_unique<chaseNode>(_enemy);
    sequence2->AddChild(move(chaseN));

    // PlayerInsight
    auto sequence3 = make_unique<SequenceNode>();
    sequence3->AddChild(make_unique<ConditionNode>(bb, "playerInsight", 1));
    unique_ptr<attackNode> attackN = make_unique<attackNode>(_enemy);
    sequence3->AddChild(move(attackN));

    root2->AddChild(move(sequence2));
    root2->AddChild(move(sequence3));

    // Patrouille
    auto sequence4 = make_unique<SequenceNode>();
    unique_ptr<patrolNode> patrolN = make_unique<patrolNode>(_enemy);
    sequence4->AddChild(move(patrolN));

    // Ajout du noeud séquence au noeud racine
    root->AddChild(move(sequence));
    root->AddChild(move(root2));
    root->AddChild(move(sequence4));
}

void InheritFromEveryone::executeTree(unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP, const float& deltaTime)
{
    // Mise à jour des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("flee", lowHP + playerDetected);
    // Exécution du noeud racine
    root->Execute(deltaTime);
}