#include <cassert>

#include "Manager.h"


// Local Variables:
// mode: c++
// End:

ClassProject::Manager::Manager() {


    // Add Leaf Node 0
    uniqueTable[falseId] = std::make_shared<TableEntry>(falseId, falseId, falseId, "False");

    // Add Leaf Node 1
    uniqueTable[trueId] = std::make_shared<TableEntry>(trueId, trueId, trueId, "True");

    nextId = 2;

}

ClassProject::BDD_ID ClassProject::Manager::createVar(const std::string &label) {

    uniqueTable[nextId] = std::make_shared<TableEntry>(trueId, falseId, nextId, label);
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

ClassProject::BDD_ID ClassProject::Manager::topVar(const ClassProject::BDD_ID f) {
    auto entry = getUniqueTableEntry(f);
    return entry->getTopVar();
}

ClassProject::BDD_ID ClassProject::Manager::ite(const ClassProject::BDD_ID i, const ClassProject::BDD_ID t, const ClassProject::BDD_ID e) {

    // Terminal cases
    if (isVariable(i) && (t == trueId) && (e == falseId)) {
        return i;
    } else if ((i == trueId) || (t == e)) {
        return t;
    } else if (i == falseId) {
        return e;
    }

    // Find TopVar
    BDD_ID fTopVar;
    if (isConstant(t) && isConstant(e)) {
        fTopVar = topVar(i);
    } else if (isConstant(t)) {
        fTopVar = std::min(topVar(i), topVar(e));
    } else if (isConstant(e)) {
        fTopVar = std::min(topVar(i), topVar(t));
    } else {
        fTopVar = std::min(topVar(i), std::min(topVar(t), topVar(e)));
    }

    // Calculate High and Low successor
    auto fHigh = ite(coFactorTrue(i, fTopVar), coFactorTrue(t, fTopVar), coFactorTrue(e, fTopVar));
    auto fLow = ite(coFactorFalse(i, fTopVar), coFactorFalse(t, fTopVar), coFactorFalse(e, fTopVar));

    // Check if value exists in Unique Table
    if (findUniqueTableEntry(fHigh, fLow, fTopVar) != falseId) {
        return findUniqueTableEntry(fHigh, fLow, fTopVar);
    }

    // Add Unique Table Entry
    uniqueTable[nextId] = std::make_shared<TableEntry>(fHigh, fLow, fTopVar);
    nextId++;

    return (nextId - 1);
}

ClassProject::BDD_ID ClassProject::Manager::coFactorTrue(const ClassProject::BDD_ID f, ClassProject::BDD_ID x) {

    // Terminal Cases
    if (isConstant(f) || isConstant(x) || (topVar(f) > x)) {
        return f;
    }

    auto fEntry = getUniqueTableEntry(f);

    if (topVar(f) == x) {
        return fEntry->getHigh();
    } else {
        auto tCoFac = coFactorTrue(fEntry->getHigh(), x);
        auto fCoFac = coFactorTrue(fEntry->getLow(), x);
        return ite(topVar(f), tCoFac, fCoFac);
    }

}

ClassProject::BDD_ID ClassProject::Manager::coFactorFalse(const ClassProject::BDD_ID f, ClassProject::BDD_ID x) {

    // Terminal Cases
    if (isConstant(f) || isConstant(x) || (topVar(f) > x)) {
        return f;
    }

    auto fEntry = getUniqueTableEntry(f);

    if (topVar(f) == x) {
        return fEntry->getLow();
    } else {
        auto tCoFac = coFactorFalse(fEntry->getHigh(), x);
        auto fCoFac = coFactorFalse(fEntry->getLow(), x);
        return ite(topVar(f), tCoFac, fCoFac);
    }

}

ClassProject::BDD_ID ClassProject::Manager::coFactorTrue(const ClassProject::BDD_ID f) {
    return coFactorTrue(f, topVar(f));
}

ClassProject::BDD_ID ClassProject::Manager::coFactorFalse(const ClassProject::BDD_ID f) {
    return coFactorFalse(f, topVar(f));
}

ClassProject::BDD_ID ClassProject::Manager::and2(const ClassProject::BDD_ID a, const ClassProject::BDD_ID b) {
    return ite(a, b, falseId);
}

ClassProject::BDD_ID ClassProject::Manager::or2(const ClassProject::BDD_ID a, const ClassProject::BDD_ID b) {
    return ite(a, trueId, b);
}

ClassProject::BDD_ID ClassProject::Manager::xor2(const ClassProject::BDD_ID a, const ClassProject::BDD_ID b) {
    return ite(a, neg(b), b);
}

ClassProject::BDD_ID ClassProject::Manager::neg(const ClassProject::BDD_ID a) {
    return ite(a, falseId, trueId);
}

ClassProject::BDD_ID ClassProject::Manager::nand2(const ClassProject::BDD_ID a, const ClassProject::BDD_ID b) {
    return ite(a, neg(b), trueId);
}

ClassProject::BDD_ID ClassProject::Manager::nor2(const ClassProject::BDD_ID a, const ClassProject::BDD_ID b) {
    return ite(a, falseId, neg(b));
}

std::string ClassProject::Manager::getTopVarName(const ClassProject::BDD_ID &root) {
    return getUniqueTableEntry(topVar(root))->getName();
}

void ClassProject::Manager::findNodes(const ClassProject::BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    nodes_of_root.insert(root);
    if (!isConstant(root)) {
        findNodes(getUniqueTableEntry(root)->getHigh(), nodes_of_root);
        findNodes(getUniqueTableEntry(root)->getLow(), nodes_of_root);
    }
}

void ClassProject::Manager::findVars(const ClassProject::BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    if (!isConstant(root)) {
        nodes_of_root.insert(topVar(root));
        findVars(getUniqueTableEntry(root)->getHigh(), nodes_of_root);
        findVars(getUniqueTableEntry(root)->getLow(), nodes_of_root);
    }
}

size_t ClassProject::Manager::uniqueTableSize() {
    return uniqueTable.size();
}

std::shared_ptr<ClassProject::TableEntry> ClassProject::Manager::getUniqueTableEntry(ClassProject::BDD_ID id) {
    return uniqueTable[id];
}

ClassProject::BDD_ID ClassProject::Manager::findUniqueTableEntry(ClassProject::BDD_ID high,
                                                                 ClassProject::BDD_ID low,
                                                                 ClassProject::BDD_ID topVar) {
    for (const auto& entry : uniqueTable) {
        if ((entry.second->getHigh() == high) && (entry.second->getLow() == low) && (entry.second->getTopVar() == topVar)) {
            return entry.first;
        }
    }
    return falseId;
}



