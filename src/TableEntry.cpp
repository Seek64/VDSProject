//
// Created by deutschmann on 10.12.20.
//

#include "TableEntry.h"

ClassProject::TableEntry::TableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar) : high{high}, low{low}, topVar{topVar} {}

ClassProject::BDD_ID ClassProject::TableEntry::getHigh() {
    return high;
}

ClassProject::BDD_ID ClassProject::TableEntry::getLow() {
    return low;
}

ClassProject::BDD_ID ClassProject::TableEntry::getTopVar() {
    return topVar;
}

