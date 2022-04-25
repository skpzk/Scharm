#include "patchcordList.h"

PatchCordList::PatchCordList(){}

bool PatchCordList::is_empty(){
    if(this->pcs.size() == 0){
        return true;
    }else{
        return false;
    }
}
void PatchCordList::add(PatchCord* pc){
    remove(pc);
    pcs.push_back(pc);
}

void PatchCordList::remove(PatchCord* pc){
    if(find(pcs.begin(), pcs.end(), pc) != pcs.end()){
        pcs.erase(std::remove(pcs.begin(), pcs.end(), pc), pcs.end());
    }
}

PatchCord* PatchCordList::last(){
    return pcs[pcs.size() - 1];
}

bool PatchCordList::duplicateExists(Patchpoint* pp1, Patchpoint* pp2){
    Patchpoint* a, *b;
    if(pp1==nullptr || pp2 == nullptr)
        return false;
    for(auto pc:pcs){
        a = pc->getStartPp();
        b = pc->getEndPp();

        if(a==nullptr || b == nullptr)
            continue;

        if((*pp1 == *a) && (*pp2 == *b)){
            return true;
        }else if((*pp2 == *a) && (*pp1 == *b)){
            return true;
        }
    }
    return false;
}
