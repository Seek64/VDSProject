#include <cassert>

#include "Manager.h"


// Local Variables:
// mode: c++
// End:

ClassProject::Manager::Manager() {

    // Add Leaf Node 0
    uniqueTable[falseId] = std::make_shared<TableEntry>(falseId, falseId, falseId);

    // Add Leaf Node 1
    uniqueTable[trueId] = std::make_shared<TableEntry>(trueId, trueId, trueId);

}

std::shared_ptr<ClassProject::TableEntry> ClassProject::Manager::getUniqueTableEntry(ClassProject::BDD_ID id) {
    return uniqueTable[id];
}

const ClassProject::BDD_ID &ClassProject::Manager::True() {
    return trueId;
}

const ClassProject::BDD_ID &ClassProject::Manager::False() {
    return falseId;
}

size_t ClassProject::Manager::uniqueTableSize() {
    return uniqueTable.size();
}

