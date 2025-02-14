#include "BT.hpp"


void BTNode::makeTree(Enemy& enemy, int playerDetected, int playerInsight, int lowHP) {
    Blackboard bb;
    bb.SetValue("playerDetected", playerDetected);
    bb.SetValue("playerInsight", playerInsight);
    bb.SetValue("lowHP", lowHP);

    auto root = std::make_unique<SelectorNode>();

    auto sequence = std::make_unique<SequenceNode>();
    sequence->AddChild(std::make_unique<ConditionNode>(bb, "lowHP", 1));
    sequence->AddChild(std::make_unique<fleeNode>(enemy));

    root->AddChild(std::move(sequence));

    auto root2 = std::make_unique<SelectorNode>();

    auto sequence2 = std::make_unique<SequenceNode>();
    sequence2->AddChild(std::make_unique<ConditionNode>(bb, "playerDetected", 1));
    sequence2->AddChild(std::make_unique<chaseNode>(enemy));

    auto sequence3 = std::make_unique<SequenceNode>();
    sequence3->AddChild(std::make_unique<ConditionNode>(bb, "playerInsight", 1));
    sequence3->AddChild(std::make_unique<attackNode>(enemy));

    root2->AddChild(std::move(sequence2));
    root2->AddChild(std::move(sequence3));

    root->AddChild(std::move(root2));

    auto sequence4 = std::make_unique<SequenceNode>();
    sequence4->AddChild(std::make_unique<patrolNode>(enemy));

    root->AddChild(std::move(sequence4));

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
void Blackboard::SetValue(const std::string& key, int value) {
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



