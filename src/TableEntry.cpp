//
// Created by deutschmann on 10.12.20.
//

#include "TableEntry.h"

#include <utility>

ClassProject::TableEntry::TableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar) : high{high}, low{low}, topVar{topVar} {}

ClassProject::TableEntry::TableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar, std::string name)
        : high{high}, low{low}, topVar{topVar}, name{std::move(name)} {}

ClassProject::BDD_ID ClassProject::TableEntry::getHigh() const {
    return high;
}

ClassProject::BDD_ID ClassProject::TableEntry::getLow() const {
    return low;
}

ClassProject::BDD_ID ClassProject::TableEntry::getTopVar() const {
    return topVar;
}

std::string ClassProject::TableEntry::getName() const {
    return name;
}

