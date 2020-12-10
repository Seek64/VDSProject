#include <cassert>

#include "Manager.h"


// Local Variables:
// mode: c++
// End:

ClassProject::Manager::Manager() {

    // Add Leaf Node 0
    uniqueTable[0] = std::make_shared<TableEntry>(0, 0, 0);

    // Add Leaf Node 1
    uniqueTable[1] = std::make_shared<TableEntry>(1, 1, 1);

}

std::shared_ptr<ClassProject::TableEntry> ClassProject::Manager::getUniqueTableEntry(ClassProject::BDD_ID id) {
    return uniqueTable[id];
}

size_t ClassProject::Manager::uniqueTableSize() {
    return uniqueTable.size();
}
