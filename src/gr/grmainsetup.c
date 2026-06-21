#include <gr/ground.h>
#include <sc/scene.h>

#include "stage_registry.h"

// // // // // // // // // // // //
//                               //
//       INITIALIZED DATA        //
//                               //
// // // // // // // // // // // //

// 0x8012E840
GObj* (*dGRMainSetupProcMakeList[/* */])(void) =
{
    grCastleMakeGround,
    grSectorMakeGround,
    grJungleMakeGround,
    grZebesMakeGround,
    grHyruleMakeGround,
    grYosterMakeGround,
    grPupupuMakeGround,
    grYamabukiMakeGround,
    grInishieMakeGround
};

// // // // // // // // // // // //
//                               //
//           FUNCTIONS           //
//                               //
// // // // // // // // // // // //

// 0x801056C0
void grMainSetupMakeGround(void)
{
#ifdef PORT
    s32 gkind = gSCManagerBattleState->gkind;
    PortStageMakeGroundFn proc = port_stage_make_ground_proc(gkind);

    if (gkind <= nGRKindBattleEnd)
    {
        /* Vanilla VS stages: the seeded registry row holds the same
         * make-ground proc as dGRMainSetupProcMakeList[gkind]. */
        if (proc != NULL)
        {
            proc();
        }
    }
    else if (proc != NULL)
    {
        /* Stage registered past the vanilla VS range: run its proc instead
         * of OOB-indexing dGRMainSetupProcMakeList[] / the bonus branches. */
        proc();
    }
    else if (gkind == nGRKindBonus3)
    {
        grBonus3MakeGround();
    }
    else if (gkind >= nGRKindBonus2Start)
    {
        sc1PBonusStageInitBonus2();
    }
    else if (gkind >= nGRKindBonus1Start)
    {
        sc1PBonusStageMakeBonus1Ground();
    }
#else
    if (gSCManagerBattleState->gkind <= nGRKindBattleEnd)
    {
        dGRMainSetupProcMakeList[gSCManagerBattleState->gkind]();
    }
    else if (gSCManagerBattleState->gkind == nGRKindBonus3)
    {
        grBonus3MakeGround();
    }
    else if (gSCManagerBattleState->gkind >= nGRKindBonus2Start)
    {
        sc1PBonusStageInitBonus2();
    }
    else if (gSCManagerBattleState->gkind >= nGRKindBonus1Start)
    {
        sc1PBonusStageMakeBonus1Ground();
    }
#endif
}
