#ifndef PATCHCORDLIST_H
#define PATCHCORDLIST_H

#include <vector>

using namespace::std;

class PatchCordList;

#include "patchcord.h"
#include "patchpoint.h"

class PatchCordList{

    public:
        PatchCordList();

        void add(PatchCord*);
        void remove(PatchCord*);
        PatchCord* last();
        bool is_empty();

        bool duplicateExists(Patchpoint*, Patchpoint*);

        vector<PatchCord *> pcs;
    private:

};

#endif // PATCHCORDLIST_H
