//
//  vxl.h
//  emi4
//
//  Created by Toby Simpson on 16.02.2026.
//

#ifndef vxl_h
#define vxl_h

struct vxl_obj
{
    float dx;
    
    struct flt3 x0;
    struct flt3 x1;
    
    struct int3 ne;
    struct int3 nv;
    
    int         ne_tot;
    int         nv_tot;
};



//init
void vxl_ini(struct vxl_obj *vxl)
{
    vxl->ne.x = ceilf((vxl->x1.x - vxl->x0.x)/vxl->dx);
    vxl->ne.y = ceilf((vxl->x1.y - vxl->x0.y)/vxl->dx);
    vxl->ne.z = ceilf((vxl->x1.z - vxl->x0.z)/vxl->dx);
    
    vxl->nv.x = vxl->ne.x+1;
    vxl->nv.y = vxl->ne.y+1;
    vxl->nv.z = vxl->ne.z+1;

    vxl->ne_tot = vxl->ne.x*vxl->ne.y*vxl->ne.z;
    vxl->nv_tot = vxl->nv.x*vxl->nv.y*vxl->nv.z;

    printf("vxl.dx %f\n", vxl->dx);
    printf("vxl.ne [%u,%u,%u]\n", vxl->ne.x, vxl->ne.y, vxl->ne.z);
    printf("vxl.ne_tot %3u\n", vxl->ne_tot);

    return;
}

#endif /* vxl_h */
