//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../TableEntry.h"
#include "../Manager.h"

TEST(TableEntryTest, GetterTests) /* NOLINT */
{
    auto entry = std::make_unique<ClassProject::TableEntry>(1, 2, 3);
    EXPECT_EQ(entry->getHigh(), 1);
    EXPECT_EQ(entry->getLow(), 2);
    EXPECT_EQ(entry->getTopVar(), 3);
}

TEST(ManagerTest, ConstructorTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseNode = manager->getUniqueTableEntry(0);
    EXPECT_EQ(falseNode->getHigh(), 0);
    EXPECT_EQ(falseNode->getLow(), 0);
    EXPECT_EQ(falseNode->getTopVar(), 0);

    auto trueNode = manager->getUniqueTableEntry(1);
    EXPECT_EQ(trueNode->getHigh(), 1);
    EXPECT_EQ(trueNode->getLow(), 1);
    EXPECT_EQ(trueNode->getTopVar(), 1);

}

TEST(ManagerTest, UniqueTableSizeTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    EXPECT_EQ(manager->uniqueTableSize(), 2);

}

TEST(ManagerTest, TrueTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    EXPECT_EQ(manager->True(), 1);

}

TEST(ManagerTest, FalseTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    EXPECT_EQ(manager->False(), 0);

}

TEST(ManagerTest, CreateVarTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    EXPECT_EQ(aId, 2);
    EXPECT_EQ(bId, 3);

    auto aEntry = manager->getUniqueTableEntry(aId);
    EXPECT_EQ(aEntry->getHigh(), 1);
    EXPECT_EQ(aEntry->getLow(), 0);
    EXPECT_EQ(aEntry->getTopVar(), aId);
    auto bEntry = manager->getUniqueTableEntry(bId);
    EXPECT_EQ(bEntry->getHigh(), 1);
    EXPECT_EQ(bEntry->getLow(), 0);
    EXPECT_EQ(bEntry->getTopVar(), bId);

}

TEST(ManagerTest, IsConstantTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    EXPECT_TRUE(manager->isConstant(manager->False()));
    EXPECT_TRUE(manager->isConstant(manager->True()));
    EXPECT_FALSE(manager->isConstant(aId));
    EXPECT_FALSE(manager->isConstant(bId));

}

TEST(ManagerTest, IsVariableTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    EXPECT_FALSE(manager->isVariable(manager->False()));
    EXPECT_FALSE(manager->isVariable(manager->True()));
    EXPECT_TRUE(manager->isVariable(aId));
    EXPECT_TRUE(manager->isVariable(bId));

}

TEST(ManagerTest, TopVarTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    EXPECT_EQ(manager->topVar(manager->False()), manager->False());
    EXPECT_EQ(manager->topVar(manager->True()), manager->True());
    EXPECT_EQ(manager->topVar(aId), aId);
    EXPECT_EQ(manager->topVar(bId), bId);

}

TEST(ManagerTest, IteTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto cId = manager->createVar("c");
    auto dId = manager->createVar("d");

    // Terminal Cases
    EXPECT_EQ(manager->ite(falseId, falseId, falseId), falseId);
    EXPECT_EQ(manager->ite(trueId, trueId, trueId), trueId);
    EXPECT_EQ(manager->ite(falseId, aId, bId), bId);
    EXPECT_EQ(manager->ite(trueId, aId, bId), aId);
    EXPECT_EQ(manager->ite(aId, trueId, falseId), aId);
    EXPECT_EQ(manager->ite(bId, aId, aId), aId);

    // a or b
    auto a_or_b = manager->ite(aId, trueId, bId);
    EXPECT_EQ(a_or_b, 6);
    auto a_or_b_entry = manager->getUniqueTableEntry(a_or_b);
    EXPECT_EQ(a_or_b_entry->getHigh(), 1);
    EXPECT_EQ(a_or_b_entry->getLow(), 3);
    EXPECT_EQ(a_or_b_entry->getTopVar(), 2);

    // c and d
    auto c_and_d = manager->ite(cId, dId, falseId);
    EXPECT_EQ(c_and_d, 7);
    auto c_and_d_entry = manager->getUniqueTableEntry(c_and_d);
    EXPECT_EQ(c_and_d_entry->getHigh(), 5);
    EXPECT_EQ(c_and_d_entry->getLow(), 0);
    EXPECT_EQ(c_and_d_entry->getTopVar(), 4);

    auto f = manager->ite(a_or_b, c_and_d, falseId);
    auto f_entry = manager->getUniqueTableEntry(f);
    EXPECT_EQ(f_entry->getHigh(), 7);
    EXPECT_EQ(f_entry->getLow(), 8);
    EXPECT_EQ(f_entry->getTopVar(), 2);

}

TEST(ManagerTest, CoFactorTrueTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto cId = manager->createVar("c");
    auto dId = manager->createVar("d");

    auto a_or_b = manager->ite(aId, trueId, bId);
    auto c_and_d = manager->ite(cId, dId, falseId);

    // Terminal Cases
    EXPECT_EQ(manager->coFactorTrue(falseId, aId), falseId);
    EXPECT_EQ(manager->coFactorTrue(trueId, aId), trueId);
    EXPECT_EQ(manager->coFactorTrue(aId, falseId), aId);
    EXPECT_EQ(manager->coFactorTrue(aId, trueId), aId);
    EXPECT_EQ(manager->coFactorTrue(bId, aId), bId);

    EXPECT_EQ(manager->coFactorTrue(falseId), falseId);
    EXPECT_EQ(manager->coFactorTrue(trueId), trueId);

    // F.TopVar == x
    EXPECT_EQ(manager->coFactorTrue(a_or_b, aId), trueId);
    EXPECT_EQ(manager->coFactorTrue(c_and_d, cId), dId);
    EXPECT_EQ(manager->coFactorTrue(aId), trueId);
    EXPECT_EQ(manager->coFactorTrue(a_or_b), trueId);
    EXPECT_EQ(manager->coFactorTrue(c_and_d), dId);

    // Else
    EXPECT_EQ(manager->coFactorTrue(a_or_b, bId), trueId);
    EXPECT_EQ(manager->coFactorTrue(c_and_d, dId), cId);

}

TEST(ManagerTest, CoFactorFalseTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto cId = manager->createVar("c");
    auto dId = manager->createVar("d");

    auto a_or_b = manager->ite(aId, trueId, bId);
    auto c_and_d = manager->ite(cId, dId, falseId);

    // Terminal Cases
    EXPECT_EQ(manager->coFactorFalse(falseId, aId), falseId);
    EXPECT_EQ(manager->coFactorFalse(trueId, aId), trueId);
    EXPECT_EQ(manager->coFactorFalse(aId, falseId), aId);
    EXPECT_EQ(manager->coFactorFalse(aId, trueId), aId);
    EXPECT_EQ(manager->coFactorFalse(bId, aId), bId);

    EXPECT_EQ(manager->coFactorFalse(falseId), falseId);
    EXPECT_EQ(manager->coFactorFalse(trueId), trueId);

    // F.TopVar == x
    EXPECT_EQ(manager->coFactorFalse(a_or_b, aId), bId);
    EXPECT_EQ(manager->coFactorFalse(c_and_d, cId), falseId);
    EXPECT_EQ(manager->coFactorFalse(aId), falseId);
    EXPECT_EQ(manager->coFactorFalse(a_or_b), bId);
    EXPECT_EQ(manager->coFactorFalse(c_and_d), falseId);

    // Else
    EXPECT_EQ(manager->coFactorFalse(a_or_b, bId), aId);
    EXPECT_EQ(manager->coFactorFalse(c_and_d, dId), falseId);

}

TEST(ManagerTest, And2Test) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    auto a_and_b = manager->and2(aId, bId);

    EXPECT_EQ(manager->and2(falseId, falseId), falseId);
    EXPECT_EQ(manager->and2(falseId, trueId), falseId);
    EXPECT_EQ(manager->and2(trueId, falseId), falseId);
    EXPECT_EQ(manager->and2(trueId, trueId), trueId);

    EXPECT_EQ(manager->and2(aId, falseId), falseId);
    EXPECT_EQ(manager->and2(aId, trueId), aId);
    EXPECT_EQ(manager->and2(falseId, aId), falseId);
    EXPECT_EQ(manager->and2(trueId, aId), aId);

    EXPECT_EQ(manager->getUniqueTableEntry(a_and_b)->getHigh(), bId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_and_b)->getLow(), falseId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_and_b)->getTopVar(), aId);

}

TEST(ManagerTest, Or2Test) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    auto a_or_b = manager->or2(aId, bId);

    EXPECT_EQ(manager->or2(falseId, falseId), falseId);
    EXPECT_EQ(manager->or2(falseId, trueId), trueId);
    EXPECT_EQ(manager->or2(trueId, falseId), trueId);
    EXPECT_EQ(manager->or2(trueId, trueId), trueId);

    EXPECT_EQ(manager->or2(aId, falseId), aId);
    EXPECT_EQ(manager->or2(aId, trueId), trueId);
    EXPECT_EQ(manager->or2(falseId, aId), aId);
    EXPECT_EQ(manager->or2(trueId, aId), trueId);

    EXPECT_EQ(manager->getUniqueTableEntry(a_or_b)->getHigh(), trueId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_or_b)->getLow(), bId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_or_b)->getTopVar(), aId);

}

TEST(ManagerTest, Xor2Test) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    auto a_xor_b = manager->xor2(aId, bId);

    EXPECT_EQ(manager->xor2(falseId, falseId), falseId);
    EXPECT_EQ(manager->xor2(falseId, trueId), trueId);
    EXPECT_EQ(manager->xor2(trueId, falseId), trueId);
    EXPECT_EQ(manager->xor2(trueId, trueId), falseId);

    EXPECT_EQ(manager->xor2(aId, falseId), aId);
    EXPECT_EQ(manager->xor2(aId, trueId), manager->neg(aId));
    EXPECT_EQ(manager->xor2(falseId, aId), aId);
    EXPECT_EQ(manager->xor2(trueId, aId), manager->neg(aId));

    EXPECT_EQ(manager->getUniqueTableEntry(a_xor_b)->getHigh(), manager->neg(bId));
    EXPECT_EQ(manager->getUniqueTableEntry(a_xor_b)->getLow(), bId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_xor_b)->getTopVar(), aId);

}

TEST(ManagerTest, NegTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");

    auto neg_a = manager->neg(aId);

    EXPECT_EQ(manager->neg(falseId), trueId);
    EXPECT_EQ(manager->neg(trueId), falseId);

    EXPECT_EQ(manager->getUniqueTableEntry(neg_a)->getHigh(), falseId);
    EXPECT_EQ(manager->getUniqueTableEntry(neg_a)->getLow(), trueId);
    EXPECT_EQ(manager->getUniqueTableEntry(neg_a)->getTopVar(), aId);

}

TEST(ManagerTest, Nand2Test) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    auto a_nand_b = manager->nand2(aId, bId);

    EXPECT_EQ(manager->nand2(falseId, falseId), trueId);
    EXPECT_EQ(manager->nand2(falseId, trueId), trueId);
    EXPECT_EQ(manager->nand2(trueId, falseId), trueId);
    EXPECT_EQ(manager->nand2(trueId, trueId), falseId);

    EXPECT_EQ(manager->nand2(aId, falseId), trueId);
    EXPECT_EQ(manager->nand2(aId, trueId), manager->neg(aId));
    EXPECT_EQ(manager->nand2(falseId, aId), trueId);
    EXPECT_EQ(manager->nand2(trueId, aId), manager->neg(aId));

    EXPECT_EQ(manager->getUniqueTableEntry(a_nand_b)->getHigh(), manager->neg(bId));
    EXPECT_EQ(manager->getUniqueTableEntry(a_nand_b)->getLow(), trueId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_nand_b)->getTopVar(), aId);

}

TEST(ManagerTest, Nor2Test) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");

    auto a_nor_b = manager->nor2(aId, bId);

    EXPECT_EQ(manager->nor2(falseId, falseId), trueId);
    EXPECT_EQ(manager->nor2(falseId, trueId), falseId);
    EXPECT_EQ(manager->nor2(trueId, falseId), falseId);
    EXPECT_EQ(manager->nor2(trueId, trueId), falseId);

    EXPECT_EQ(manager->nor2(aId, falseId), manager->neg(aId));
    EXPECT_EQ(manager->nor2(aId, trueId), falseId);
    EXPECT_EQ(manager->nor2(falseId, aId), manager->neg(aId));
    EXPECT_EQ(manager->nor2(trueId, aId), falseId);

    EXPECT_EQ(manager->getUniqueTableEntry(a_nor_b)->getHigh(), falseId);
    EXPECT_EQ(manager->getUniqueTableEntry(a_nor_b)->getLow(), manager->neg(bId));
    EXPECT_EQ(manager->getUniqueTableEntry(a_nor_b)->getTopVar(), aId);

}

TEST(ManagerTest, GetTopVarNameTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto a_and_b = manager->and2(aId, bId);

    EXPECT_EQ(manager->getTopVarName(falseId), "False");
    EXPECT_EQ(manager->getTopVarName(trueId), "True");
    EXPECT_EQ(manager->getTopVarName(aId), "a");
    EXPECT_EQ(manager->getTopVarName(bId), "b");
    EXPECT_EQ(manager->getTopVarName(a_and_b), "a");

}

TEST(ManagerTest, FindNodesTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto falseId = manager->False();
    auto trueId = manager->True();
    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto cId = manager->createVar("c");
    auto dId = manager->createVar("d");

    auto a_or_b = manager->or2(aId, bId);
    auto c_and_d = manager->and2(cId, dId);
    auto f = manager->and2(a_or_b, c_and_d);

    std::set<ClassProject::BDD_ID> a_or_b_nodes;
    std::set<ClassProject::BDD_ID> c_and_d_nodes;
    std::set<ClassProject::BDD_ID> f_nodes;

    manager->findNodes(a_or_b, a_or_b_nodes);
    manager->findNodes(c_and_d, c_and_d_nodes);
    manager->findNodes(f, f_nodes);

    EXPECT_EQ(a_or_b_nodes.size(), 4);
    EXPECT_TRUE(a_or_b_nodes.find(falseId) != a_or_b_nodes.end());
    EXPECT_TRUE(a_or_b_nodes.find(trueId) != a_or_b_nodes.end());
    EXPECT_TRUE(a_or_b_nodes.find(bId) != a_or_b_nodes.end());
    EXPECT_TRUE(a_or_b_nodes.find(a_or_b) != a_or_b_nodes.end());

    EXPECT_EQ(c_and_d_nodes.size(), 4);
    EXPECT_TRUE(c_and_d_nodes.find(falseId) != c_and_d_nodes.end());
    EXPECT_TRUE(c_and_d_nodes.find(trueId) != c_and_d_nodes.end());
    EXPECT_TRUE(c_and_d_nodes.find(dId) != c_and_d_nodes.end());
    EXPECT_TRUE(c_and_d_nodes.find(c_and_d) != c_and_d_nodes.end());

    EXPECT_EQ(f_nodes.size(), 6);
    EXPECT_TRUE(f_nodes.find(falseId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(trueId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(dId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(c_and_d) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(manager->and2(bId, c_and_d)) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(f) != f_nodes.end());

}

TEST(ManagerTest, FindVarsTest) /* NOLINT */
{
    auto manager = std::make_unique<ClassProject::Manager>();

    auto aId = manager->createVar("a");
    auto bId = manager->createVar("b");
    auto cId = manager->createVar("c");
    auto dId = manager->createVar("d");

    auto a_or_b = manager->or2(aId, bId);
    auto c_and_d = manager->and2(cId, dId);
    auto f = manager->and2(a_or_b, c_and_d);

    std::set<ClassProject::BDD_ID> a_or_b_nodes;
    std::set<ClassProject::BDD_ID> c_and_d_nodes;
    std::set<ClassProject::BDD_ID> f_nodes;

    manager->findVars(a_or_b, a_or_b_nodes);
    manager->findVars(c_and_d, c_and_d_nodes);
    manager->findVars(f, f_nodes);

    EXPECT_EQ(a_or_b_nodes.size(), 2);
    EXPECT_TRUE(a_or_b_nodes.find(aId) != a_or_b_nodes.end());
    EXPECT_TRUE(a_or_b_nodes.find(bId) != a_or_b_nodes.end());

    EXPECT_EQ(c_and_d_nodes.size(), 2);
    EXPECT_TRUE(c_and_d_nodes.find(cId) != c_and_d_nodes.end());
    EXPECT_TRUE(c_and_d_nodes.find(dId) != c_and_d_nodes.end());

    EXPECT_EQ(f_nodes.size(), 4);
    EXPECT_TRUE(f_nodes.find(aId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(bId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(cId) != f_nodes.end());
    EXPECT_TRUE(f_nodes.find(dId) != f_nodes.end());

}

#endif //VDS_PROJECT_TESTS_H

