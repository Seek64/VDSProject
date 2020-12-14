//
// Created by deutschmann on 10.12.20.
//

#ifndef VDSPROJECT_TABLEENTRY_H
#define VDSPROJECT_TABLEENTRY_H

#include "ManagerInterface.h"

namespace ClassProject {

    class TableEntry {

    public:

        TableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar);

        TableEntry(BDD_ID high, BDD_ID low, BDD_ID topVar, std::string name);

        BDD_ID getHigh() const;

        BDD_ID getLow() const;

        BDD_ID getTopVar() const;

        std::string getName() const;

    private:

        const BDD_ID high;
        const BDD_ID low;
        const BDD_ID topVar;
        const std::string name;

    };

}

#endif //VDSPROJECT_TABLEENTRY_H
