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

        BDD_ID getHigh();
        BDD_ID getLow();
        BDD_ID getTopVar();

    private:

        const BDD_ID high;
        const BDD_ID low;
        const BDD_ID topVar;

    };

}

#endif //VDSPROJECT_TABLEENTRY_H
