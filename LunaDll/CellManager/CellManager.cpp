#include "CellManager.h"
#include "../SMBXInternal/Blocks.h"
#include "../SMBXInternal/PlayerMOB.h"
#include "../Misc/MiscFuncs.h"

// CELL :: COUNT
Cell::Cell(int _x, int _y)
{ x = _x; y = _y; pNext=0; }

int Cell::CountDownward(int* oObjCounter) {
    int count = 1;
    int objcount = 0;
    Cell* downward = pNext;
    if(oObjCounter != NULL)
        objcount += ContainedObjs.size();
    while(downward != NULL) {
        if(oObjCounter != NULL)
            objcount += downward->ContainedObjs.size();
        count++;
        downward = downward->pNext;
    }
    if(oObjCounter != NULL)
        *oObjCounter = objcount;
    return count;
}

// CELL :: ADD UNIQUE
bool Cell::AddUnique(CellObj new_obj)
{
    for (std::list<CellObj >::const_iterator it = ContainedObjs.begin() ; it != ContainedObjs.end() ; it++)
    {
        CellObj obj = *it;
        if(obj.pObj == new_obj.pObj)
            return false;
    }
    ContainedObjs.push_back(new_obj);
    return true;
}

// CELL MANAGER :: RESET
CellManager::CellManager() { Reset(); }

void CellManager::Reset() {
    ClearAllBuckets();
}

// CELL MANAGER :: CLEAR BUCKETS
void CellManager::ClearAllBuckets() {	
    for(int i = 0; i < BUCKET_COUNT; i++) {
        ClearBucket(i);
    }
}

// CELL MANAGER :: CLEAR BUCKET -- Clear until all contained cells empty
void CellManager::ClearBucket(int bucket_index) {	

    // If there is a cell list head at all...
    while(m_BucketArray[bucket_index].ContainedCellsHead != NULL) {
        Cell* pNextCell = m_BucketArray[bucket_index].ContainedCellsHead->pNext;

        // If head isn't the end of the list, delete head and next elem becomes head of bucket
        // Delete until it does == null
        while(pNextCell != NULL) {			
            delete m_BucketArray[bucket_index].ContainedCellsHead;
            m_BucketArray[bucket_index].ContainedCellsHead = pNextCell;
            pNextCell = m_BucketArray[bucket_index].ContainedCellsHead->pNext;
        }
        // Reached end of list, delete it
        delete m_BucketArray[bucket_index].ContainedCellsHead;
        m_BucketArray[bucket_index].ContainedCellsHead = NULL;		
    }
}

// CELL MANAGER :: COUNT ALL
void CellManager::CountAll(int* oFilledBuckets, int* oCellCount, int* oObjRefs) {	
    int fillcount = 0;
    int cellcount = 0;
    int objcount = 0;

    for(int i = 0; i < BUCKET_COUNT; i++) {
        if(m_BucketArray[i].ContainedCellsHead != NULL) {
            int objtemp = 0;
            fillcount++;
            cellcount += m_BucketArray[i].ContainedCellsHead->CountDownward(&objtemp);
            objcount += objtemp;
        }
    }
    if(oFilledBuckets != NULL)
        *oFilledBuckets = fillcount;
    if(oCellCount != NULL)
        *oCellCount = cellcount;
    if(oObjRefs != NULL)
        *oObjRefs = objcount;
}

// CELL MANAGER :: COMPUTE HASH BUCKET INDEX
int CellManager::ComputeHashBucketIndex(int x, int y) {
    int n = (HASH_PRIME_1 * x) + (HASH_PRIME_2 * y);
    n = n % BUCKET_COUNT;
    if (n < 0)
        n = n + BUCKET_COUNT;
    return n;
}

// CELL MANAGER :: SCAN LEVEL
void CellManager::ScanLevel(bool update_blocks) {
    //char* dbg = "!!! SCAN LEVEL DEBUG !!!";
    PlayerMOB* demo = Player::Get(1);
    ClearAllBuckets();

    if(update_blocks) {
        Block* cur_block = NULL;
        int ct = Blocks::Count();
        for(int i = 0; i <  ct; i++) {
            cur_block = Blocks::Get(i);
            if (cur_block != NULL && demo && demo->CurrentSection + 1 == ComputeLevelSection((int)cur_block->momentum.x, (int)cur_block->momentum.y))
                AddObj((void*)cur_block, CLOBJ_SMBXBLOCK);
        }
    }
}

// CELL MANAGER :: ADD OBJ
void CellManager::AddObj(void* pObj, CELL_OBJ_TYPE type) {
    switch(type) {
    case CLOBJ_SMBXBLOCK:{
        Block* cur_block = (Block*)pObj;		
        double block_x = cur_block->momentum.x;
        double block_y = cur_block->momentum.y;
        double block_xMax = block_x + cur_block->momentum.width;					// Rightmost block point
        double block_yMax = block_y + cur_block->momentum.height;					// Bottommost block point
        double snapped_x = SnapToGrid(cur_block->momentum.x, DEF_CELL_W);
        double original_snapped_x = snapped_x;
        double snapped_y = SnapToGrid(cur_block->momentum.y, DEF_CELL_H);

        // Check if block spans multiple cells
        int cells_occupied_x = 1;
        int cells_occupied_y = 1;
        if(block_xMax > (snapped_x + DEF_CELL_W)) {
            cells_occupied_x += (int)(floor((block_xMax - snapped_x) /  DEF_CELL_W));
        }
        if(block_yMax > (snapped_y + DEF_CELL_H)) {
            cells_occupied_y += (int)(floor((block_yMax - snapped_y) /  DEF_CELL_H));
        }

        for(int i = 0; i < cells_occupied_y; i++) {
            for(int j = 0; j <  cells_occupied_x; j++) {
                int hash_i = ComputeHashBucketIndex((int)snapped_x, (int)snapped_y);

                // If target bucket doesn't contain a cell head, create new one
                if(m_BucketArray[hash_i].ContainedCellsHead == NULL) {
                    m_BucketArray[hash_i].ContainedCellsHead = new Cell((int)snapped_x, (int)snapped_y);
                }

                // If target bucket doesn't contain cell with these coords, create new one and add it to bucket
                Cell* p_sought_cell = FindCell(hash_i, (int)snapped_x, (int)snapped_y);
                if(p_sought_cell == NULL) {
                    p_sought_cell = new Cell((int)snapped_x, (int)snapped_y);
                    AddCell(hash_i, p_sought_cell);
                }

                // Either way we should now have a pointer to the correct cell
                CellObj new_obj;
                new_obj.Type = CLOBJ_SMBXBLOCK;
                new_obj.pObj = pObj;				
                p_sought_cell->AddUnique(new_obj);

                snapped_x += DEF_CELL_W; // for next iteration
            }
            snapped_x = original_snapped_x;
            snapped_y += DEF_CELL_H; // for next iteration
        }

        break;
                         }
    default:
        break;
    }//< switch
}

// CELL MANAGER :: FIND CELL -- Check a bucket for the cell with the given coords
Cell* CellManager::FindCell(int bucket_index, int ax, int ay) {	
    Cell* cur_cell = m_BucketArray[bucket_index].ContainedCellsHead;
    while(cur_cell != NULL) {
        if(cur_cell->x == ax && cur_cell->y == ay)
            return cur_cell;
        cur_cell = cur_cell->pNext;
    }
    return NULL;
}

// CELL MANAGER :: ADD CELL
void CellManager::AddCell(int bucket_index, Cell* pcell) {
    // Add as head?
    if(m_BucketArray[bucket_index].ContainedCellsHead == NULL) {
        m_BucketArray[bucket_index].ContainedCellsHead = pcell;
        return;
    }
    else { // Add to end of list
        Cell* next_cell = m_BucketArray[bucket_index].ContainedCellsHead;
        while(next_cell->pNext != NULL) {
            next_cell = next_cell->pNext;
        }
        next_cell->pNext = pcell;
    }
}

// CELL MANAGER :: GET OBJECTS OF INTEREST
void CellManager::GetObjectsOfInterest(std::list<CellObj>* objs, double x, double y, int w, int h) {	
    double rect_xMax = x + w;		// Rightmost block point
    double rect_yMax = y + h;		// Bottommost block point
    double snapped_x = SnapToGrid(x, DEF_CELL_W);
    double original_snapped_x = snapped_x;
    double snapped_y = SnapToGrid(y, DEF_CELL_H);

    // Check if block spans multiple cells
    double cells_occupied_x = 1;
    double cells_occupied_y = 1;
    if(rect_xMax > (snapped_x + DEF_CELL_W)) {
        cells_occupied_x += (int)(floor((rect_xMax - snapped_x) /  DEF_CELL_W));
    }
    if(rect_yMax > (snapped_y + DEF_CELL_H)) {
        cells_occupied_y += (int)(floor((rect_yMax - snapped_y) /  DEF_CELL_H));
    }

    // Put all the objects into the list
    for(int i = 0; i < cells_occupied_y; i++) {
        for(int j = 0; j <  cells_occupied_x; j++) {

            GetUniqueObjs(objs, snapped_x, snapped_y);
            
            snapped_x += DEF_CELL_W; // for next iteration
        }
        snapped_x = original_snapped_x;
        snapped_y += DEF_CELL_H; // for next iteration
    }
}

// CELL MANAGER :: GET UNIQUE OBJS
void CellManager::GetUniqueObjs(std::list<CellObj>* objlist, double x, double y) {
    int hash_i = ComputeHashBucketIndex((int)x, (int)y);
    Cell* sought_cell = FindCell(hash_i, (int)x, (int)y);

    if(sought_cell != NULL) {		
        bool add = true;
        for( std::list<CellObj >::const_iterator it = sought_cell->ContainedObjs.begin();
             it != sought_cell->ContainedObjs.end(); it++)
        {
            CellObj cellobj = *it;
            // Loop over all objs in this cell
            for(std::list<CellObj>::iterator iter = objlist->begin(), end = objlist->end(); iter != end; ++iter) { // Compare to each passed obj
                if((*iter).pObj == cellobj.pObj) { // Set to not add if pointing to same obj
                    add = false;
                    break;
                }
            }
            if(add)
                objlist->push_back(cellobj);
            add = true;
        }
    }
}

// CELL MANAGER :: SORT BY NEAREST
void CellManager::SortByNearest(std::list<CellObj>* objlist, double cx, double cy) {
    std::vector<CellObj> objvec(objlist->begin(), objlist->end());
    std::vector<double> distlist(objvec.size(), 99999);
    std::list<CellObj> sortedlist;

    for (unsigned int i = 0; i < objvec.size(); i++) {		
        CellObj obj = objvec[i];
        switch(obj.Type) {
        case CLOBJ_SMBXBLOCK: {
            Block* block = (Block*)obj.pObj;
            double block_cx = (block->momentum.x + (block->momentum.width / 2));
            double block_cy = (block->momentum.y + (block->momentum.height / 2));
            double x_dist = cx - block_cx;
            double y_dist = cy - block_cy;
            double sqrd_dist = abs((x_dist * x_dist) + (y_dist * y_dist));
            distlist[i] = sqrd_dist;
            break;
                              }
        default:
            break;
        }		
    }//<switch

    // distlist now contains distances aligned with object vector
    double lowest_dist = 9999999;
    int lowest_index = 0;
    unsigned int extent = 0;
    while(extent < objvec.size()) {
        for(unsigned int i = extent; i < objvec.size(); i++) {
            if(distlist[i] < lowest_dist) {
                lowest_dist = distlist[i];
                lowest_index = i;
            }
        }
        // Should have lowest dist index now. Swap extent content (head / part of array to be cut off after this) with lowest index
        CellObj swap = objvec[lowest_index];
        objvec[lowest_index] = objvec[extent];
        objvec[extent] = swap;
        double dswap = distlist[lowest_index];
        distlist[lowest_index] = distlist[extent];
        distlist[extent] = dswap;

        lowest_dist = 9999999;		
        extent++;
        lowest_index = extent;
    }

    // Replace stuff in objlist with ordered ones
    objlist->clear();
    for(int i = objvec.size() - 1; i >= 0; i--) {		
        objlist->push_front(objvec[i]);
    }
}


double SnapToGrid(double coord, double span) { return floor(coord / span) * span; }
