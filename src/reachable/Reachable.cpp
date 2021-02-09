//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"

ClassProject::Reachable::Reachable(unsigned int stateSize) : ReachableInterface(stateSize) {

    for (int i=0; i<stateSize; i++) {
        currentStatesVars.push_back(this->createVar("s"+std::to_string(i)));
        nextStatesVars.push_back(this->createVar("s"+std::to_string(i)+"_n"));
    }

    transitionRelation = this->False();
    initialState = this->False();

}

ClassProject::BDD_ID ClassProject::Reachable::xnor2(ClassProject::BDD_ID a, ClassProject::BDD_ID b) {
    return ite(a, b, neg(b));
}

const std::vector<ClassProject::BDD_ID> &ClassProject::Reachable::getStates() const {
    return currentStatesVars;
}

void ClassProject::Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions) {
    deltaFunctions = transitionFunctions;

    computeTransitionRelation();
}

void ClassProject::Reachable::setInitState(const std::vector<bool> &stateVector) {
    //TODO: Assert equal size for stateVector and states
    initialState = True();
    auto stateVarIt = currentStatesVars.begin();

    for (const auto &stateBit : stateVector) {
        if (stateBit) {
            initialState = and2(initialState, *stateVarIt);
        } else {
            initialState = and2(initialState, neg(*stateVarIt));
        }
        stateVarIt++;
    }
}

ClassProject::BDD_ID ClassProject::Reachable::compute_reachable_states() {

    //! Step 6
    BDD_ID c_R_it = initialState;
    BDD_ID c_R;

    while (true) {
        //! Step 7
        c_R = c_R_it;

        //! Step 8
        BDD_ID img = and2(c_R, transitionRelation);
        for (const auto &stateVar : currentStatesVars) {
            img = or2(coFactorTrue(img, stateVar), coFactorFalse(img, stateVar));
        }

        //! Step 9
        auto stateVarIt = currentStatesVars.begin();
        for (const auto &nextStateVar : nextStatesVars) {
            img = and2(xnor2(*stateVarIt, nextStateVar), img);
            stateVarIt++;
        }
        for (const auto &nextStateVar : nextStatesVars) {
            img = or2(coFactorTrue(img, nextStateVar), coFactorFalse(img, nextStateVar));
        }

        //! Step 10
        c_R_it = or2(c_R, img);

        //! Step 11
        if (c_R == c_R_it) {
            break;
        }
    }

    return c_R;
}

bool ClassProject::Reachable::is_reachable(const std::vector<bool> &stateVector) {

    BDD_ID reachableStateSpace = compute_reachable_states();
    auto stateVarIt  = currentStatesVars.begin();

    for (const auto &stateBit : stateVector) {
        if (stateBit) {
            reachableStateSpace = coFactorTrue(reachableStateSpace, *stateVarIt);
        } else {
            reachableStateSpace = coFactorFalse(reachableStateSpace, *stateVarIt);
        }
        stateVarIt++;
    }

    return (reachableStateSpace == True());
}

void ClassProject::Reachable::computeTransitionRelation() {
    transitionRelation = True();
    auto nextStateVarIt = nextStatesVars.begin();

    for (const auto& deltaFunction : deltaFunctions) {
        transitionRelation = and2(transitionRelation,
                                  or2(and2(*nextStateVarIt, deltaFunction),
                                         and2(neg(*nextStateVarIt), neg(deltaFunction))));
        nextStateVarIt++;
    }
}

