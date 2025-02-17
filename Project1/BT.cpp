#include "BT.hpp"

static void makeTree(std::unique_ptr<SelectorNode>& root, std::unique_ptr<SequenceNode>& sequence, Blackboard& bb, Enemy& enemy, 
    bool& playerDetected, bool& playerInsight, bool& lowHP) 
{
    // Inscription des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("lowHP", lowHP);

    // Ajout des ordres au noeud séquence
        // LowHP
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "lowHP", 1));
    sequence->AddChild(std::make_unique<fleeNode>(enemy));
        // PlayerDetected
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "playerDetected", 1));
    sequence->AddChild(std::make_unique<chaseNode>(enemy));
        // PlayerInsight
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "playerInsight", 1));
    sequence->AddChild(std::make_unique<attackNode>(enemy));
    sequence->AddChild(std::make_unique<patrolNode>(enemy));
    
    // Ajout du noeud séquence au noeud racine
    root->AddChild(std::move(sequence));
}

static void updateBlackboard(Blackboard& bb, bool& playerDetected, bool& playerInsight, bool& lowHP) 
{
    // Mise à jour des "valeurs" sur le blackboard
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("lowHP", lowHP);
}

static void executeTree(std::unique_ptr<SelectorNode>& root) 
{
    // Exécution du noeud racine
    root->Execute();
}

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

//----------------------------Blackboard---------------------------------
void Blackboard::SetValue(const std::string& key, bool value) {
    data[key] = value;
}
int Blackboard::GetValue(const std::string& key) {
    return data[key];
}

//----------------------------ConditionNode---------------------------------
NodeState ConditionNode::Execute()  {
    return (blackboard.GetValue(key) == expectedValue) ? NodeState::SUCCESS : NodeState::FAILURE;
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
NodeState chaseNode::Execute() {
    enemy.chase(1, grid); //suivre
    return NodeState::SUCCESS;
}

NodeState attackNode::Execute() {
    enemy.attack(1, grid); //attaquer
    return NodeState::SUCCESS;
}

NodeState fleeNode::Execute() {
    enemy.flee(1, grid); //fuir
    return NodeState::SUCCESS;
}

NodeState patrolNode::Execute() {
    enemy.patrol(1, grid); //fuir
    return NodeState::SUCCESS;
}