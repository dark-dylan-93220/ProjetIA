#include "BT.hpp"

//--------------------------SequenceNode------------------------------
void SequenceNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}
NodeState SequenceNode::Execute(const float& deltaTime) {
    for (auto& child : children) {
        if (child->Execute(deltaTime) == NodeState::FAILURE) {
            return NodeState::FAILURE;
        }
    }
    return NodeState::SUCCESS;
}
void SequenceNode::addGrid(Grid& _grid) {
    grid = _grid;
}

//----------------------------SelectorNode---------------------------------
void SelectorNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}
NodeState SelectorNode::Execute(const float& deltaTime) {
    for (auto& child : children) {
        if (child->Execute(deltaTime) == NodeState::SUCCESS) {
            return NodeState::SUCCESS;
        }
    }
    return NodeState::FAILURE;
}

void SelectorNode::addGrid(Grid& _grid) {
    grid = _grid;
}

//----------------------------Blackboard---------------------------------
void Blackboard::SetValue(const std::string& key, bool value) {
    data[key] = value;
}
bool Blackboard::GetValue(const std::string& key) {
    return data[key];
}

//----------------------------ConditionNode---------------------------------
NodeState ConditionNode::Execute(const float& deltaTime)  {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}
void ConditionNode::addGrid(Grid& _grid) {
    grid = _grid;
}
//----------------------------CheckEnemyProximityNode---------------------------------
NodeState CheckEnemyProximityNode::Execute(const float& deltaTime) {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

//----------------------------PrintMessageNode---------------------------
NodeState PrintMessageNode::Execute(const float& deltaTime) {
    std::cout << message << std::endl;
    return NodeState::SUCCESS;
}

//----------------------------ActionNode---------------------------------
void chaseNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState chaseNode::Execute(const float& deltaTime) {
    enemy->chase(deltaTime, grid); //suivre
    return NodeState::SUCCESS;
}

void attackNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState attackNode::Execute(const float& deltaTime) {
    enemy->attack(deltaTime, grid); //attaquer
    return NodeState::SUCCESS;
}

void fleeNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState fleeNode::Execute(const float& deltaTime) {
    if (enemy->playerDetected && enemy->lowHP) {
        enemy->flee(deltaTime, grid); //fuir
        std::cout << enemy << " fuit." << std::endl;
        return NodeState::SUCCESS;
    }
    return NodeState::FAILURE;
}

void patrolNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState patrolNode::Execute(const float& deltaTime) {
    if (!enemy->playerDetected) {
        enemy->patrol(deltaTime, grid); //patrouiller
        return NodeState::SUCCESS;
    }
    return NodeState::FAILURE;
}

void InheritFromEveryone::makeTree(std::unique_ptr<SelectorNode>& root, Blackboard& bb, std::shared_ptr<Enemy>& _enemy,
    bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid)
{
    //std::shared_ptr<Enemy> enemy = _enemy;

    // Inscription des "valeurs" sur le blackboard
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("flee", lowHP + playerDetected);

    // Noeuds racines
    auto root2 = std::make_unique<SelectorNode>();

    // Ajout des ordres au noeud séquence
        // LowHP
    auto sequence = std::make_unique<SequenceNode>();
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "flee", 2));
    std::unique_ptr<fleeNode> fleeN = std::make_unique<fleeNode>(_enemy);
    fleeN->addGrid(grid);
    sequence->AddChild(std::move(fleeN));

    // PlayerDetected
    auto sequence2 = std::make_unique<SequenceNode>();
    sequence2->AddChild(std::make_unique<ConditionNode>(bb, "playerDetected", 1));
    std::unique_ptr<chaseNode> chaseN = std::make_unique<chaseNode>(_enemy);
    chaseN->addGrid(grid);
    sequence2->AddChild(std::move(chaseN));

    // PlayerInsight
    auto sequence3 = std::make_unique<SequenceNode>();
    sequence3->AddChild(std::make_unique<ConditionNode>(bb, "playerInsight", 1));
    std::unique_ptr<attackNode> attackN = std::make_unique<attackNode>(_enemy);
    attackN->addGrid(grid);
    sequence3->AddChild(std::move(attackN));

    root2->AddChild(std::move(sequence2));
    root2->AddChild(std::move(sequence3));

    auto sequence4 = std::make_unique<SequenceNode>();
    std::unique_ptr<patrolNode> patrolN = std::make_unique<patrolNode>(_enemy);
    patrolN->addGrid(grid);
    sequence4->AddChild(std::move(patrolN));

    // Ajout du noeud séquence au noeud racine
    root->AddChild(std::move(sequence));
    root->AddChild(std::move(root2));
    root->AddChild(std::move(sequence4));
}

void InheritFromEveryone::executeTree(std::unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP, const float& deltaTime, Enemy& _enemy)
{
    // Mise à jour des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("flee", lowHP + playerDetected);
    // Exécution du noeud racine
    root->Execute(deltaTime);
}