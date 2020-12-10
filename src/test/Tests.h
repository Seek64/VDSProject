//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include <iostream>
#include <iomanip>
#include <memory>

#include "gtest/gtest.h"
#include "../TableEntry.h"
#include "../Manager.h"

TEST(TableEntryTest, GetterTests) /* NOLINT */
{
    std::unique_ptr<ClassProject::TableEntry> entry = std::make_unique<ClassProject::TableEntry>(1, 2, 3);
    EXPECT_EQ(entry->getHigh(), 1);
    EXPECT_EQ(entry->getLow(), 2);
    EXPECT_EQ(entry->getTopVar(), 3);
}

#endif //VDS_PROJECT_TESTS_H

