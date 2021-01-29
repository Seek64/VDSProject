//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "../gtest/gtest.h"
#include "../TableEntry.h"
#include "../Manager.h"

struct ManagerTest : testing::Test {
    std::unique_ptr<ClassProject::Manager> manager = std::make_unique<ClassProject::Manager>();

    ClassProject::BDD_ID false_id = manager->False();
    ClassProject::BDD_ID true_id = manager->True();

    ClassProject::BDD_ID a_id = manager->createVar("a");
    ClassProject::BDD_ID b_id = manager->createVar("b");
    ClassProject::BDD_ID c_id = manager->createVar("c");
    ClassProject::BDD_ID d_id = manager->createVar("d");

    ClassProject::BDD_ID neg_a_id = manager->neg(a_id);
    ClassProject::BDD_ID neg_b_id = manager->neg(b_id);

    ClassProject::BDD_ID a_and_b_id = manager->and2(a_id, b_id);
    ClassProject::BDD_ID a_or_b_id = manager->or2(a_id, b_id);
    ClassProject::BDD_ID a_xor_b_id = manager->xor2(a_id, b_id);
    ClassProject::BDD_ID a_nand_b_id = manager->nand2(a_id, b_id);
    ClassProject::BDD_ID a_nor_b_id = manager->nor2(a_id, b_id);

    ClassProject::BDD_ID c_or_d_id = manager->or2(c_id, d_id);

    // f1 = a*b+c+d
    ClassProject::BDD_ID f1_id = manager->or2(a_and_b_id, c_or_d_id);
};

TEST(TableEntryTest, GetterTests) /* NOLINT */
{
    auto entry = std::make_unique<ClassProject::TableEntry>(1, 2, 3);
    EXPECT_EQ(entry->getHigh(), 1);
    EXPECT_EQ(entry->getLow(), 2);
    EXPECT_EQ(entry->getTopVar(), 3);
    EXPECT_EQ(entry->getName(), "");

    auto entry2 = std::make_unique<ClassProject::TableEntry>(4, 5, 6, "testName");
    EXPECT_EQ(entry2->getHigh(), 4);
    EXPECT_EQ(entry2->getLow(), 5);
    EXPECT_EQ(entry2->getTopVar(), 6);
    EXPECT_EQ(entry2->getName(), "testName");
}

TEST(UniqueTableSizeTest, UniqueTableSizeTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();
    EXPECT_EQ(manager->uniqueTableSize(), 2);
    auto a_id = manager->createVar("a");
    EXPECT_EQ(manager->uniqueTableSize(), 3);
    auto b_id = manager->createVar("b");
    EXPECT_EQ(manager->uniqueTableSize(), 4);
    manager->and2(a_id, b_id); // Generates neg(b) at standard triples check
    EXPECT_EQ(manager->uniqueTableSize(), 6);
    manager->or2(a_id, b_id);
    EXPECT_EQ(manager->uniqueTableSize(), 7);
    manager->and2(b_id, a_id); // Generates neg(a) at standard triples check
    EXPECT_EQ(manager->uniqueTableSize(), 8);
    manager->or2(b_id, a_id);
    EXPECT_EQ(manager->uniqueTableSize(), 8);
}

TEST_F(ManagerTest, TrueTest) /* NOLINT */
{
    EXPECT_EQ(manager->True(), true_id);
}

TEST_F(ManagerTest, FalseTest) /* NOLINT */
{
    EXPECT_EQ(manager->False(), false_id);
}

TEST_F(ManagerTest, CreateVarTest) /* NOLINT */
{
    EXPECT_EQ(manager->topVar(a_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(a_id), false_id);

    EXPECT_EQ(manager->topVar(b_id), b_id);
    EXPECT_EQ(manager->coFactorTrue(b_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(b_id), false_id);
}

TEST_F(ManagerTest, IsConstantTest) /* NOLINT */
{
    EXPECT_TRUE(manager->isConstant(false_id));
    EXPECT_TRUE(manager->isConstant(true_id));
    EXPECT_FALSE(manager->isConstant(a_id));
    EXPECT_FALSE(manager->isConstant(b_id));
    EXPECT_FALSE(manager->isConstant(a_and_b_id));
}

TEST_F(ManagerTest, IsVariableTest) /* NOLINT */
{
    EXPECT_FALSE(manager->isVariable(false_id));
    EXPECT_FALSE(manager->isVariable(true_id));
    EXPECT_TRUE(manager->isVariable(a_id));
    EXPECT_TRUE(manager->isVariable(b_id));
    EXPECT_FALSE(manager->isVariable(a_and_b_id));
}

TEST_F(ManagerTest, TopVarTest) /* NOLINT */
{
    EXPECT_EQ(manager->topVar(false_id), false_id);
    EXPECT_EQ(manager->topVar(true_id), true_id);
    EXPECT_EQ(manager->topVar(a_id), a_id);
    EXPECT_EQ(manager->topVar(b_id), b_id);
    EXPECT_EQ(manager->topVar(a_and_b_id), a_id);
    EXPECT_EQ(manager->topVar(c_or_d_id), c_id);
    EXPECT_EQ(manager->topVar(f1_id), a_id);
}

TEST_F(ManagerTest, IteTest) /* NOLINT */
{
    // Terminal Cases
    EXPECT_EQ(manager->ite(false_id, false_id, false_id), false_id);
    EXPECT_EQ(manager->ite(true_id, true_id, true_id), true_id);
    EXPECT_EQ(manager->ite(false_id, a_id, b_id), b_id);
    EXPECT_EQ(manager->ite(true_id, a_id, b_id), a_id);
    EXPECT_EQ(manager->ite(a_id, true_id, false_id), a_id);
    EXPECT_EQ(manager->ite(b_id, a_id, a_id), a_id);

    //TODO: Check for Standard Triples

    EXPECT_EQ(manager->ite(a_id, b_id, false_id), a_and_b_id);
    EXPECT_EQ(manager->ite(a_id, true_id, b_id), a_or_b_id);
    EXPECT_EQ(manager->ite(a_and_b_id, true_id, c_or_d_id), f1_id);
}

TEST_F(ManagerTest, CoFactorTrueTest) /* NOLINT */
{
    EXPECT_EQ(manager->coFactorTrue(false_id), false_id);
    EXPECT_EQ(manager->coFactorTrue(true_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(a_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(b_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id), b_id);
    EXPECT_EQ(manager->coFactorTrue(c_or_d_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(f1_id), manager->or2(b_id, c_or_d_id));

    EXPECT_EQ(manager->coFactorTrue(false_id, false_id), false_id);
    EXPECT_EQ(manager->coFactorTrue(false_id, true_id), false_id);
    EXPECT_EQ(manager->coFactorTrue(false_id, a_id), false_id);
    EXPECT_EQ(manager->coFactorTrue(false_id, b_id), false_id);

    EXPECT_EQ(manager->coFactorTrue(true_id, false_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(true_id, true_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(true_id, a_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(true_id, b_id), true_id);

    EXPECT_EQ(manager->coFactorTrue(a_id, false_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_id, true_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_id, a_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(a_id, b_id), a_id);

    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, false_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, true_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, a_id), b_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, c_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id, a_and_b_id), a_and_b_id);

    EXPECT_EQ(manager->coFactorTrue(c_or_d_id, c_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(c_or_d_id, d_id), true_id);

    EXPECT_EQ(manager->coFactorTrue(f1_id, a_id), manager->or2(b_id, c_or_d_id));
    EXPECT_EQ(manager->coFactorTrue(f1_id, b_id), manager->or2(a_id, c_or_d_id));
    EXPECT_EQ(manager->coFactorTrue(f1_id, c_id), true_id);
    EXPECT_EQ(manager->coFactorTrue(f1_id, d_id), true_id);
}

TEST_F(ManagerTest, CoFactorFalseTest) /* NOLINT */
{

    EXPECT_EQ(manager->coFactorFalse(false_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(true_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(a_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(b_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(c_or_d_id), d_id);
    EXPECT_EQ(manager->coFactorFalse(f1_id), c_or_d_id);

    EXPECT_EQ(manager->coFactorFalse(false_id, false_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(false_id, true_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(false_id, a_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(false_id, b_id), false_id);

    EXPECT_EQ(manager->coFactorFalse(true_id, false_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(true_id, true_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(true_id, a_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(true_id, b_id), true_id);

    EXPECT_EQ(manager->coFactorFalse(a_id, false_id), a_id);
    EXPECT_EQ(manager->coFactorFalse(a_id, true_id), a_id);
    EXPECT_EQ(manager->coFactorFalse(a_id, a_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(a_id, b_id), a_id);

    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, false_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, true_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, a_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, b_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, c_id), a_and_b_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id, a_and_b_id), a_and_b_id);

    EXPECT_EQ(manager->coFactorFalse(c_or_d_id, c_id), d_id);
    EXPECT_EQ(manager->coFactorFalse(c_or_d_id, d_id), c_id);

    EXPECT_EQ(manager->coFactorFalse(f1_id, a_id), c_or_d_id);
    EXPECT_EQ(manager->coFactorFalse(f1_id, b_id), c_or_d_id);
    EXPECT_EQ(manager->coFactorFalse(f1_id, c_id), manager->or2(a_and_b_id, d_id));
    EXPECT_EQ(manager->coFactorFalse(f1_id, d_id), manager->or2(a_and_b_id, c_id));
}

TEST_F(ManagerTest, And2Test) /* NOLINT */
{
    EXPECT_EQ(manager->and2(false_id, false_id), false_id);
    EXPECT_EQ(manager->and2(false_id, true_id), false_id);
    EXPECT_EQ(manager->and2(true_id, false_id), false_id);
    EXPECT_EQ(manager->and2(true_id, true_id), true_id);

    EXPECT_EQ(manager->and2(a_id, false_id), false_id);
    EXPECT_EQ(manager->and2(a_id, true_id), a_id);
    EXPECT_EQ(manager->and2(false_id, a_id), false_id);
    EXPECT_EQ(manager->and2(true_id, a_id), a_id);

    EXPECT_EQ(manager->topVar(a_and_b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_and_b_id), b_id);
    EXPECT_EQ(manager->coFactorFalse(a_and_b_id), false_id);
}

TEST_F(ManagerTest, Or2Test) /* NOLINT */
{
    EXPECT_EQ(manager->or2(false_id, false_id), false_id);
    EXPECT_EQ(manager->or2(false_id, true_id), true_id);
    EXPECT_EQ(manager->or2(true_id, false_id), true_id);
    EXPECT_EQ(manager->or2(true_id, true_id), true_id);

    EXPECT_EQ(manager->or2(a_id, false_id), a_id);
    EXPECT_EQ(manager->or2(a_id, true_id), true_id);
    EXPECT_EQ(manager->or2(false_id, a_id), a_id);
    EXPECT_EQ(manager->or2(true_id, a_id), true_id);

    EXPECT_EQ(manager->topVar(a_or_b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_or_b_id), true_id);
    EXPECT_EQ(manager->coFactorFalse(a_or_b_id), b_id);
}

TEST_F(ManagerTest, Xor2Test) /* NOLINT */
{
    EXPECT_EQ(manager->xor2(false_id, false_id), false_id);
    EXPECT_EQ(manager->xor2(false_id, true_id), true_id);
    EXPECT_EQ(manager->xor2(true_id, false_id), true_id);
    EXPECT_EQ(manager->xor2(true_id, true_id), false_id);

    EXPECT_EQ(manager->xor2(a_id, false_id), a_id);
    EXPECT_EQ(manager->xor2(a_id, true_id), neg_a_id);
    EXPECT_EQ(manager->xor2(false_id, a_id), a_id);
    EXPECT_EQ(manager->xor2(true_id, a_id), neg_a_id);

    EXPECT_EQ(manager->topVar(a_xor_b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_xor_b_id), neg_b_id);
    EXPECT_EQ(manager->coFactorFalse(a_xor_b_id), b_id);
}

TEST_F(ManagerTest, NegTest) /* NOLINT */
{
    EXPECT_EQ(manager->neg(false_id), true_id);
    EXPECT_EQ(manager->neg(true_id), false_id);

    EXPECT_EQ(manager->topVar(neg_a_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(neg_a_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(neg_a_id), true_id);

    // Check De Morgan's laws
    EXPECT_EQ(manager->neg(a_and_b_id), manager->or2(neg_a_id, neg_b_id));
    EXPECT_EQ(manager->neg(a_or_b_id), manager->and2(neg_a_id, neg_b_id));
}

TEST_F(ManagerTest, Nand2Test) /* NOLINT */
{
    EXPECT_EQ(manager->nand2(false_id, false_id), true_id);
    EXPECT_EQ(manager->nand2(false_id, true_id), true_id);
    EXPECT_EQ(manager->nand2(true_id, false_id), true_id);
    EXPECT_EQ(manager->nand2(true_id, true_id), false_id);

    EXPECT_EQ(manager->nand2(a_id, false_id), true_id);
    EXPECT_EQ(manager->nand2(a_id, true_id), neg_a_id);
    EXPECT_EQ(manager->nand2(false_id, a_id), true_id);
    EXPECT_EQ(manager->nand2(true_id, a_id), neg_a_id);

    EXPECT_EQ(manager->topVar(a_nand_b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_nand_b_id), neg_b_id);
    EXPECT_EQ(manager->coFactorFalse(a_nand_b_id), true_id);
}

TEST_F(ManagerTest, Nor2Test) /* NOLINT */
{
    EXPECT_EQ(manager->nor2(false_id, false_id), true_id);
    EXPECT_EQ(manager->nor2(false_id, true_id), false_id);
    EXPECT_EQ(manager->nor2(true_id, false_id), false_id);
    EXPECT_EQ(manager->nor2(true_id, true_id), false_id);

    EXPECT_EQ(manager->nor2(a_id, false_id), neg_a_id);
    EXPECT_EQ(manager->nor2(a_id, true_id), false_id);
    EXPECT_EQ(manager->nor2(false_id, a_id), neg_a_id);
    EXPECT_EQ(manager->nor2(true_id, a_id), false_id);

    EXPECT_EQ(manager->topVar(a_nor_b_id), a_id);
    EXPECT_EQ(manager->coFactorTrue(a_nor_b_id), false_id);
    EXPECT_EQ(manager->coFactorFalse(a_nor_b_id), neg_b_id);
}

TEST_F(ManagerTest, GetTopVarNameTest) /* NOLINT */
{
    EXPECT_EQ(manager->getTopVarName(false_id), "False");
    EXPECT_EQ(manager->getTopVarName(true_id), "True");
    EXPECT_EQ(manager->getTopVarName(a_id), "a");
    EXPECT_EQ(manager->getTopVarName(b_id), "b");
    EXPECT_EQ(manager->getTopVarName(a_and_b_id), "a");
    EXPECT_EQ(manager->getTopVarName(c_or_d_id), "c");
    EXPECT_EQ(manager->getTopVarName(f1_id), "a");
}

TEST_F(ManagerTest, FindNodesTest) /* NOLINT */
{
    std::set<ClassProject::BDD_ID> a_and_b_nodes;
    std::set<ClassProject::BDD_ID> c_or_d_nodes;
    std::set<ClassProject::BDD_ID> f1_nodes;

    manager->findNodes(a_and_b_id, a_and_b_nodes);
    manager->findNodes(c_or_d_id, c_or_d_nodes);
    manager->findNodes(f1_id, f1_nodes);

    EXPECT_EQ(a_and_b_nodes.size(), 4);
    EXPECT_TRUE(a_and_b_nodes.find(false_id) != a_and_b_nodes.end());
    EXPECT_TRUE(a_and_b_nodes.find(true_id) != a_and_b_nodes.end());
    EXPECT_TRUE(a_and_b_nodes.find(b_id) != a_and_b_nodes.end());
    EXPECT_TRUE(a_and_b_nodes.find(a_and_b_id) != a_and_b_nodes.end());

    EXPECT_EQ(c_or_d_nodes.size(), 4);
    EXPECT_TRUE(c_or_d_nodes.find(false_id) != c_or_d_nodes.end());
    EXPECT_TRUE(c_or_d_nodes.find(true_id) != c_or_d_nodes.end());
    EXPECT_TRUE(c_or_d_nodes.find(d_id) != c_or_d_nodes.end());
    EXPECT_TRUE(c_or_d_nodes.find(c_or_d_id) != c_or_d_nodes.end());

    EXPECT_EQ(f1_nodes.size(), 6);
    EXPECT_TRUE(f1_nodes.find(false_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(true_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(d_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(c_or_d_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(manager->or2(b_id, c_or_d_id)) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(f1_id) != f1_nodes.end());
}

TEST_F(ManagerTest, FindVarsTest) /* NOLINT */
{
    std::set<ClassProject::BDD_ID> a_and_b_nodes;
    std::set<ClassProject::BDD_ID> c_or_d_nodes;
    std::set<ClassProject::BDD_ID> f1_nodes;

    manager->findVars(a_and_b_id, a_and_b_nodes);
    manager->findVars(c_or_d_id, c_or_d_nodes);
    manager->findVars(f1_id, f1_nodes);

    EXPECT_EQ(a_and_b_nodes.size(), 2);
    EXPECT_TRUE(a_and_b_nodes.find(a_id) != a_and_b_nodes.end());
    EXPECT_TRUE(a_and_b_nodes.find(b_id) != a_and_b_nodes.end());

    EXPECT_EQ(c_or_d_nodes.size(), 2);
    EXPECT_TRUE(c_or_d_nodes.find(c_id) != c_or_d_nodes.end());
    EXPECT_TRUE(c_or_d_nodes.find(d_id) != c_or_d_nodes.end());

    EXPECT_EQ(f1_nodes.size(), 4);
    EXPECT_TRUE(f1_nodes.find(a_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(b_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(c_id) != f1_nodes.end());
    EXPECT_TRUE(f1_nodes.find(d_id) != f1_nodes.end());
}

#endif //VDS_PROJECT_TESTS_H

