#include "LevelCodes.h"
#include "../Globals.h"
#include "../Defines.h"
#include "../SMBXInternal/PlayerMOB.h"
#include "../Rendering/Rendering.h"
#include "../Rendering/RenderOps/RenderOp.h"
#include "../Rendering/RenderOps/RenderEffectOp.h"

void KilArmoryCode() {
    PlayerMOB* demo = Player::Get(1);
    if(demo) {

        // Section 20(19) glow effect code
        if(gFrames > 60 && demo->CurrentSection == 19) {			
            int intensity = (int)(sin((float)(gFrames) / 22) * 35) + 60;
            intensity <<= 16;
            RenderEffectOp* op = new RenderEffectOp(RNDEFF_ScreenGlow, BLEND_Additive, intensity, 100);
            op->m_FramesLeft = 1;
            Renderer::Get().AddOp(op);
        }

        // Section 1(0) glow effect code
        if(gFrames > 60 && demo->CurrentSection == 0 && gAutoMan.GetVar(L"GOTSANGRE") == 0) {
            int intensity = (int)(sin((float)(gFrames) / 10) * 45) + 48;
            intensity <<= 16;
            RenderEffectOp* op = new RenderEffectOp(RNDEFF_ScreenGlow, BLEND_Additive, intensity, 100);
            op->m_FramesLeft = 1;
            Renderer::Get().AddOp(op);
        }
    }
}