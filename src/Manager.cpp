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

    nextId = 2;

}

ClassProject::BDD_ID ClassProject::Manager::createVar(const std::string &label) {

    uniqueTable[nextId] = std::make_shared<TableEntry>(trueId, falseId, nextId);
    nextId++;

    return (nextId - 1);
}

const ClassProject::BDD_ID &ClassProject::Manager::True() {
    return trueId;
}

const ClassProject::BDD_ID &ClassProject::Manager::False() {
    return falseId;
}

bool ClassProject::Manager::isConstant(const ClassProject::BDD_ID f) {
    return ((f == falseId) || (f == trueId));
}

bool ClassProject::Manager::isVariable(const ClassProject::BDD_ID f) {
    auto entry = getUniqueTableEntry(f);
    return ((entry->getHigh() == trueId) && (entry->getLow() == falseId) && (entry->getTopVar() == f));
}

size_t ClassProject::Manager::uniqueTableSize() {
    return uniqueTable.size();
}

std::shared_ptr<ClassProject::TableEntry> ClassProject::Manager::getUniqueTableEntry(ClassProject::BDD_ID id) {
    return uniqueTable[id];
}



