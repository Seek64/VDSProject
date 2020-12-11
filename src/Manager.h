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

        BDD_ID createVar(const std::string &label) override;

        const BDD_ID &True() override;

        const BDD_ID &False() override;

        bool isConstant(BDD_ID f) override;

        bool isVariable(BDD_ID f) override;

        BDD_ID topVar(BDD_ID f) override;

        BDD_ID ite(BDD_ID i, BDD_ID t, BDD_ID e) override;

        BDD_ID coFactorTrue(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorFalse(BDD_ID f, BDD_ID x) override;

        BDD_ID coFactorTrue(BDD_ID f) override;

        BDD_ID coFactorFalse(BDD_ID f) override;

        BDD_ID and2(BDD_ID a, BDD_ID b) override;

        BDD_ID or2(BDD_ID a, BDD_ID b) override;

        BDD_ID xor2(BDD_ID a, BDD_ID b) override;

        BDD_ID neg(BDD_ID a) override;

        BDD_ID nand2(BDD_ID a, BDD_ID b) override;

        BDD_ID nor2(BDD_ID a, BDD_ID b) override;

        size_t uniqueTableSize() override;

        std::shared_ptr<TableEntry> getUniqueTableEntry(BDD_ID id);

    private:

        const BDD_ID falseId = 0;
        const BDD_ID trueId = 1;

        BDD_ID nextId;

        std::map<BDD_ID, std::shared_ptr<TableEntry>> uniqueTable;

        BDD_ID findUniqueTableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar);

    };

}
#endif

// Local Variables:
// mode: c++
// End:
