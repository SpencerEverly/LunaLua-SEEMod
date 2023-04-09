#include <vector>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "LevelCodes.h"
#include "../SMBXInternal/PlayerMOB.h"
#include "../Globals.h"
#include "../SMBXInternal/NPCs.h"

#define NPC_FIREBAR 260
#define NPC_DOUGHNUT 210
#define NPC_SCIENCE 209

using namespace std;

namespace ScienceBattle
{
    NPCMOB* FindNPC(short identity);
    vector<NPCMOB*> FindAllNPC(short identity);
    bool TriggerBox(double x1, double y1, double x2, double y2);
    void HurtPlayer();

    bool init_doonce;
    NPCMOB* hurt_npc, *science_npc, *friendly_doughnut;
    vector<NPCMOB*> doughnuts;
    PlayerMOB* demo;
    int hurt_timer;
    int grace_timer;
    int throw_timer;

    void ScienceInitCode()
    {
        init_doonce = false;
    }

    void ScienceCode()
    {
        if (!init_doonce)
        {
            init_doonce		= true;
            hurt_timer		= 0;
            throw_timer		= 0;
            demo			= Player::Get(1);
        }

        hurt_npc = FindNPC(NPC_FIREBAR);
        
        if (hurt_npc == NULL)
            return;


        if (hurt_timer <= 0)
            hurt_npc->momentum.y = demo->momentum.y - 128;
        else
        {
            hurt_timer--;
            hurt_npc ->momentum.y = demo->momentum.y;
        }
        hurt_npc->momentum.x = demo->momentum.x;

        doughnuts = FindAllNPC(NPC_DOUGHNUT);

        if (demo->HeldNPCIndex > 0)
            throw_timer = 30;

        //Renderer::Get().SafePrint(std::wstring(L"ID: " + std::to_wstring(demo->HeldNPCIndex)), 3, 0, 256);

        
        if (grace_timer >= 0)
        {
            for(std::vector<NPCMOB*>::const_iterator it = doughnuts.begin();it!=doughnuts.end();it++)
            {
                NPCMOB* doughnut=*it;
                double x_diff, y_diff, m;

                x_diff = doughnut->momentum.x - demo->momentum.x;
                y_diff = doughnut->momentum.y - demo->momentum.y;
                m = sqrt(x_diff * x_diff + y_diff * y_diff);

                if (m == 0)
                    continue;

                x_diff /= m;
                y_diff /= m;

                doughnut->momentum.x += x_diff * 15;
                doughnut->momentum.y += y_diff * 15;
            }
            grace_timer--;
        }
        else
        {
            if (throw_timer <= 0)
            {
                for(std::vector<NPCMOB*>::const_iterator it = doughnuts.begin();it!=doughnuts.end();it++)
                {
                    NPCMOB* doughnut=*it;
                    //Ignore generators
                    if ((*((int*)doughnut + 16)) != 0)
                        continue;
                
                    double x1, x2, y1, y2;
            
                    x1 = doughnut->momentum.x + 28 * 0.42;
                    y1 = doughnut->momentum.y + 32 * 0.42;
                    x2 = doughnut->momentum.x + 28 * 0.57;
                    y2 = doughnut->momentum.y + 32 * 0.57;

                    if (TriggerBox(x1, y1, x2, y2))
                        HurtPlayer();
                }
            }
        }
        

        if (throw_timer > 0)
            throw_timer--;
    }

    NPCMOB* FindNPC(short identity)
    {
        NPCMOB* currentnpc = NULL;

        for(int i = 0; i < GM_NPCS_COUNT; i++)
        {
            currentnpc = NPC::Get(i);
            if (currentnpc->id == identity)
                return currentnpc;
        }

        return NULL;
    }

    vector<NPCMOB*> FindAllNPC(short identity)
    {
        vector<NPCMOB*> npcs_found = vector<NPCMOB*>();
        NPCMOB* currentnpc = NULL;

        for(int i = 0; i < GM_NPCS_COUNT; i++)
        {
            currentnpc = NPC::Get(i);
            if (currentnpc->id == identity)
                npcs_found.push_back(currentnpc);
        }

        return npcs_found;
    }

    void HurtPlayer()
    {
        hurt_timer = 3;
        grace_timer = 120;
    }

    bool TriggerBox(double x1, double y1, double x2, double y2)
    {
        return (demo->momentum.x + demo->momentum.width		> x1 &&
            demo->momentum.x					< x2 &&
            demo->momentum.y + demo->momentum.height	> y1 &&
            demo->momentum.y					< y2);
    }
}
