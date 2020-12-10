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

#endif //VDS_PROJECT_TESTS_H

