#include "Manager.h"

ClassProject::Manager::Manager() {

    // Add Leaf Node 0
    uniqueTable.emplace(falseId, std::make_shared<TableEntry>(falseId, falseId, falseId, "False"));
    reverseUniqueTable[{falseId, falseId, falseId}] = falseId;

    // Add Leaf Node 1
    uniqueTable.emplace(trueId, std::make_shared<TableEntry>(trueId, trueId, trueId, "True"));
    reverseUniqueTable[{trueId, trueId, trueId}] = trueId;
}

ClassProject::BDD_ID ClassProject::Manager::createVar(const std::string &label) {

    BDD_ID newId = uniqueTableSize();
    uniqueTable.emplace(newId, std::make_shared<TableEntry>(trueId, falseId, newId, label));
    reverseUniqueTable[{trueId, falseId, newId}] = newId;

    return newId;
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
    auto entry = uniqueTable.find(f)->second;
    return ((entry->getHigh() == trueId) && (entry->getLow() == falseId) && (entry->getTopVar() == f));
}

ClassProject::BDD_ID ClassProject::Manager::topVar(const ClassProject::BDD_ID f) {
    auto entry = uniqueTable.find(f)->second;
    return entry->getTopVar();
}

ClassProject::BDD_ID
ClassProject::Manager::ite(const ClassProject::BDD_ID i, const ClassProject::BDD_ID t, const ClassProject::BDD_ID e) {

    // Terminal cases
    if ((t == trueId) && (e == falseId)) {
        return i;
    } else if ((i == trueId) || (t == e)) {
        return t;
    } else if (i == falseId) {
        return e;
    }

    // Standard Triples
    if (i == t) {
        return ite(i, trueId, e);
    } else if (i == e) {
        return ite(i, t, falseId);
    } else if (i == neg(e)) {
        return ite(i, t, trueId);
    } else if (i == neg(t)) {
        return ite(i, falseId, e);
    } else if ((t == trueId) && (topVar(i) > topVar(e))) {
        return ite(e, trueId, i);
    } else if ((e == falseId) && (topVar(i) > topVar(t))) {
        return ite(t, i, falseId);
    } else if ((e == trueId) && (topVar(i) > topVar(t)) && (t != falseId)) {
        return ite(neg(t), neg(i), trueId);
    } else if ((t == falseId) && (topVar(i) > topVar(e)) && (e != trueId)) {
        return ite(neg(e), falseId, neg(i));
    }

    // Check if value already exists in computedTable
    auto computedTableEntry = computedTable.find({i, t, e});
    if (computedTableEntry != computedTable.end()) {
        return computedTableEntry->second;
    }


    // Find TopVar
    //TODO: Check which method is faster
    std::set<BDD_ID> topVars;
    if (!isConstant(topVar(i))) topVars.insert(topVar(i));
    if (!isConstant(topVar(t))) topVars.insert(topVar(t));
    if (!isConstant(topVar(e))) topVars.insert(topVar(e));
    BDD_ID fTopVar = *std::min_element(topVars.begin(), topVars.end());
 /*   BDD_ID fTopVar;
    if (isConstant(t) && isConstant(e)) {
        fTopVar = topVar(i);
    } else if (isConstant(t)) {
        fTopVar = std::min(topVar(i), topVar(e));
    } else if (isConstant(e)) {
        fTopVar = std::min(topVar(i), topVar(t));
    } else {
        fTopVar = std::min(topVar(i), std::min(topVar(t), topVar(e)));
    }*/

    // Calculate High and Low successor
    auto fHigh = ite(coFactorTrue(i, fTopVar), coFactorTrue(t, fTopVar), coFactorTrue(e, fTopVar));
    auto fLow = ite(coFactorFalse(i, fTopVar), coFactorFalse(t, fTopVar), coFactorFalse(e, fTopVar));

    if (fHigh == fLow) {
        return fHigh;
    }

    // Check if value already exists in uniqueTable
    auto reverseUniqueTableEntry = reverseUniqueTable.find({fHigh, fLow, fTopVar});
    if (reverseUniqueTableEntry != reverseUniqueTable.end()) {
        computedTable[{i, t, e}] = reverseUniqueTableEntry->second;
        return reverseUniqueTableEntry->second;
    }

    // Add Unique and Computed Table Entry
    BDD_ID newId = uniqueTableSize();
    uniqueTable.emplace(newId, std::make_shared<TableEntry>(fHigh, fLow, fTopVar));
    reverseUniqueTable[{fHigh, fLow, fTopVar}] = newId;
    computedTable[{i, t, e}] = newId;

    return newId;
}

ClassProject::BDD_ID ClassProject::Manager::coFactorTrue(const ClassProject::BDD_ID f, ClassProject::BDD_ID x) {

    // Terminal Cases
    if (isConstant(f) || isConstant(x) || (topVar(f) > x)) {
        return f;
    }

    auto fEntry = uniqueTable.find(f)->second;

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

    auto fEntry = uniqueTable.find(f)->second;

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
    return uniqueTable.find(topVar(root))->second->getName();
}

void ClassProject::Manager::findNodes(const ClassProject::BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    nodes_of_root.insert(root);
    if (!isConstant(root)) {
        findNodes(uniqueTable.find(root)->second->getHigh(), nodes_of_root);
        findNodes(uniqueTable.find(root)->second->getLow(), nodes_of_root);
    }
}

void ClassProject::Manager::findVars(const ClassProject::BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
    if (!isConstant(root)) {
        nodes_of_root.insert(topVar(root));
        findVars(uniqueTable.find(root)->second->getHigh(), nodes_of_root);
        findVars(uniqueTable.find(root)->second->getLow(), nodes_of_root);
    }
}

size_t ClassProject::Manager::uniqueTableSize() {
    return uniqueTable.size();
}


