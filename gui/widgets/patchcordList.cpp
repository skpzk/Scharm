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
