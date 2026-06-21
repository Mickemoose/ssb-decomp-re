/**
 * grport.c - decomp-side helper for the port stage registry.
 *
 * Lives in decomp/ because it needs access to the source-of-truth vanilla
 * arrays (dMPCollisionGroundFileInfos[] / dGRMainSetupProcMakeList[]) and
 * the GRKind enum + GRFileInfo type. The port-side .cpp can't pull these
 * decomp headers across the C++ boundary.
 *
 * Export:
 *   port_stage_seed_vanilla() - registers the vanilla gkinds (the full
 *       common+bonus range covered by dMPCollisionGroundFileInfos) from the
 *       vanilla decomp tables. Called once at PortInit.
 *
 * Mirrors decomp/src/ft/ftport.c (the fighter-registry seeder).
 */

#ifdef PORT

#include <gr/ground.h>

#include "stage_registry.h"

extern void port_log(const char *fmt, ...);

/* Per-gkind map-file lookup: {file_id, header offset}. 41 entries spanning
 * the full GRKind common+bonus range (nGRKindCastle .. nGRKindBonus2End). */
extern GRFileInfo dMPCollisionGroundFileInfos[];

/* Per-stage make-ground procs. Only the VS stages (0 .. nGRKindBattleEnd)
 * have entries; other gkinds are dispatched via grMainSetupMakeGround's
 * else-branches. */
extern GObj *(*dGRMainSetupProcMakeList[])(void);

/* Seed the vanilla gkind rows of the registry from the decomp arrays.
 * Synth rows past the vanilla range may be registered later via
 * port_stage_register. */
void port_stage_seed_vanilla(void)
{
    s32 gkind;
    StageDescriptor desc;

    /* dMPCollisionGroundFileInfos spans nGRKindCastle(0) .. nGRKindBonus2End,
     * so the vanilla row count is that last index + 1. */
    const s32 count = nGRKindBonus2End + 1;

    for (gkind = 0; gkind < count; gkind++)
    {
        s32 i;
        u8 *p = (u8 *)&desc;
        for (i = 0; i < (s32)sizeof(desc); i++) p[i] = 0;

        /* Vanilla rows are NOT placed on the grid via the registry (they use
         * mnmaps' own ROM page table); -1 keeps them out of the synth overlay
         * lookup (port_stage_gkind_at). name/emblem_franchise stay NULL. */
        desc.page = -1;
        desc.cell = -1;

        desc.map_file_id       = dMPCollisionGroundFileInfos[gkind].file_id;
        desc.map_header_offset = dMPCollisionGroundFileInfos[gkind].offset;

        if (gkind <= nGRKindBattleEnd)
        {
            desc.make_ground_proc = (PortStageMakeGroundFn)dGRMainSetupProcMakeList[gkind];
        }
        else
        {
            desc.make_ground_proc = NULL;
        }

        port_stage_register(gkind, &desc);
    }

    port_log("[stage] registry seeded: %d vanilla gkinds (file_id[0]=%d file_id[%d]=%d)\n",
             (int)count,
             (int)dMPCollisionGroundFileInfos[nGRKindCastle].file_id,
             (int)nGRKindBonus2End,
             (int)dMPCollisionGroundFileInfos[nGRKindBonus2End].file_id);
}

#endif /* PORT */
