//
// Created by ludwig on 27.11.18.
//

#ifndef VDSPROJECT_IMGCOMP_H
#define VDSPROJECT_IMGCOMP_H


#include <map>
#include "ReachableInterface.h"

namespace ClassProject {

    class Reachable : public ReachableInterface {

    public:

        explicit Reachable(unsigned int stateSize);

        BDD_ID xnor2(BDD_ID a, BDD_ID b) override;

        const std::vector<BDD_ID> &getStates() const override;

        void setDelta(const std::vector<BDD_ID> &transitionFunctions) override;

        void setInitState(const std::vector<bool>& stateVector) override;

        BDD_ID compute_reachable_states() override;

        bool is_reachable(const std::vector<bool>& stateVector) override;

    private:

        std::vector<BDD_ID> currentStatesVars;
        std::vector<BDD_ID> nextStatesVars;

        std::vector<BDD_ID> deltaFunctions;

        BDD_ID transitionRelation;

        BDD_ID initialState;

        void computeTransitionRelation();


    };

}
#endif //VDSPROJECT_IMGCOMP_H
