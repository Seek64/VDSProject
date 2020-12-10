// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <set>
#include <string>
#include <map>
#include <memory>

#include "TableEntry.h"
#include "ManagerInterface.h"

namespace ClassProject {

    class Manager : public ManagerInterface {

    public:

        Manager();

        size_t uniqueTableSize() override;

        std::shared_ptr<TableEntry> getUniqueTableEntry(BDD_ID id);

    private:

        std::map<BDD_ID, std::shared_ptr<TableEntry>> uniqueTable;

    };

}
#endif

// Local Variables:
// mode: c++
// End:
