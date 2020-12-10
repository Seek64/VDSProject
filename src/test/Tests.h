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

#endif //VDS_PROJECT_TESTS_H

