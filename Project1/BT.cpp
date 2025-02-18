#include "BT.hpp"

//--------------------------SequenceNode------------------------------
void SequenceNode::AddChild(std::unique_ptr<BTNode> child) {
    children.push_back(std::move(child));
}
NodeState SequenceNode::Execute() {
    for (auto& child : children) {
        if (child->Execute() == NodeState::FAILURE) {
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
NodeState SelectorNode::Execute() {
    for (auto& child : children) {
        if (child->Execute() == NodeState::SUCCESS) {
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
NodeState ConditionNode::Execute()  {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}
void ConditionNode::addGrid(Grid& _grid) {
    grid = _grid;
}
//----------------------------CheckEnemyProximityNode---------------------------------
NodeState CheckEnemyProximityNode::Execute() {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
}

//----------------------------PrintMessageNode---------------------------
NodeState PrintMessageNode::Execute() {
    std::cout << message << std::endl;
    return NodeState::SUCCESS;
}

//----------------------------ActionNode---------------------------------
void chaseNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState chaseNode::Execute() {
    enemy->chase(1, grid); //suivre
    return NodeState::SUCCESS;
}

void attackNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState attackNode::Execute() {
    enemy->attack(1, grid); //attaquer
    return NodeState::SUCCESS;
}

void fleeNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState fleeNode::Execute() {
    enemy->flee(1, grid); //fuir
    return NodeState::SUCCESS;
}

void patrolNode::addGrid(Grid& _grid) {
    grid = _grid;
}
NodeState patrolNode::Execute() {
    enemy->patrol(1, grid); //fuir
    return NodeState::SUCCESS;
}

void InheritFromEveryone::makeTree(std::unique_ptr<SelectorNode>& root1, std::unique_ptr<SelectorNode>& root2, std::unique_ptr<SequenceNode>& sequence, std::unique_ptr<SequenceNode>& sequence2, std::unique_ptr<SequenceNode>& sequence3, std::unique_ptr<SequenceNode>& sequence4, Blackboard& bb, std::shared_ptr<Entity> _enemy,
    bool& playerDetected, bool& playerInsight, bool& lowHP, Grid& grid)
{
    std::shared_ptr<Enemy> enemy = std::dynamic_pointer_cast<Enemy>(_enemy);

    // Inscription des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("lowHP", lowHP);

    // Ajout des ordres au noeud séquence
        // LowHP
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "lowHP", 1));
    std::unique_ptr<fleeNode> fleeN = std::make_unique<fleeNode>(enemy);
    fleeN->addGrid(grid);
    sequence->AddChild(std::move(fleeN));


    // PlayerDetected
    sequence2->AddChild(std::make_unique<ConditionNode>(bb, "playerDetected", 1));
    std::unique_ptr<chaseNode> chaseN = std::make_unique<chaseNode>(enemy);
    chaseN->addGrid(grid);
    sequence2->AddChild(std::move(chaseN));
    // PlayerInsight
    sequence3->AddChild(std::make_unique<ConditionNode>(bb, "playerInsight", 1));
    std::unique_ptr<attackNode> attackN = std::make_unique<attackNode>(enemy);
    attackN->addGrid(grid);
    sequence3->AddChild(std::move(attackN));

    root2->AddChild(std::move(sequence2));
    root2->AddChild(std::move(sequence3));


    std::unique_ptr<patrolNode> patrolN = std::make_unique<patrolNode>(enemy);
    patrolN->addGrid(grid);
    sequence4->AddChild(std::move(patrolN));

    // Ajout du noeud séquence au noeud racine
    root1->AddChild(std::move(sequence));
    root1->AddChild(std::move(root2));
    root1->AddChild(std::move(sequence4));
}

void InheritFromEveryone::executeTree(std::unique_ptr<SelectorNode>& root, Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP)
{
    
    // Mise à jour des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("lowHP", lowHP);
    // Exécution du noeud racine
    root->Execute();
}