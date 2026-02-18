//
//  tet.h
//  emi4
//
//  Created by Toby Simpson on 16.02.2026.
//

#ifndef tet_h
#define tet_h

struct tet_obj
{
    long ne;
    long nv;
};


//tet barycentres
void tet_bary(const struct tet_obj *tet, struct int4 *tet_vtx, struct flt3 *vtx_xyz, struct flt3 *tet_ctr)
{
    for(int tet_idx=0; tet_idx<tet->ne; tet_idx++)
    {
        //verts
        struct flt3 a = vtx_xyz[tet_vtx[tet_idx].x];
        struct flt3 b = vtx_xyz[tet_vtx[tet_idx].y];
        struct flt3 c = vtx_xyz[tet_vtx[tet_idx].z];
        struct flt3 d = vtx_xyz[tet_vtx[tet_idx].w];
        
        struct flt3 ctr = {0.25f*(a.x + b.x + c.x + d.x), 0.25f*(a.y + b.y + c.y + d.y), 0.25f*(a.z + b.z + c.z + d.z)};
        
        tet_ctr[tet_idx] = ctr;
        
        //printf("%e %e %e\n", tet_ctr[tet_idx].x, tet_ctr[tet_idx].y, tet_ctr[tet_idx].z);
    }
}


#endif /* tet_h */
