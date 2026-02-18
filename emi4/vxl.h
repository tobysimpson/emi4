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


//map tet data to voxels
void vxl_map(const struct tet_obj *tet, const struct vxl_obj *vxl, struct int4 *tet_vtx, struct flt3 *vtx_xyz, struct flt3 *tet_ctr, float *tet_tag, float *vxl_tag)
{
    struct int3 pos;
    
    //loop vxl
    for(pos.z=0; pos.z<vxl->ne.z; pos.z++)
    {
        for(pos.y=0; pos.y<vxl->ne.y; pos.y++)
        {
            for(pos.x=0; pos.x<vxl->ne.x; pos.x++)
            {
                int vxl_idx = pos.x + pos.y*vxl->ne.x + pos.z*vxl->ne.x*vxl->ne.y;
                
                //reset
                vxl_tag[vxl_idx] = 0e0f;
                
                struct flt3 x = {vxl->x0.x + vxl->dx*(pos.x + 0.5f), vxl->x0.y + vxl->dx*(pos.y + 0.5f), vxl->x0.z + vxl->dx*(pos.z + 0.5f)};
                
                //printf("%6d %e %e %ef\n", vxl_idx, x.x, x.y, x.z);
                
                //loop tets
                for(int tet_idx=0; tet_idx<tet->ne; tet_idx++)
                {
                    struct flt3 ctr = tet_ctr[tet_idx];
                    struct flt3 dst = {x.x - ctr.x, x.y - ctr.y, x.z - ctr.z};
                    float nrm = sqrtf(dot3(dst,dst));
                    
                    if(nrm<10e0f)
                    {
                        //verts
                        struct flt3 a = vtx_xyz[tet_vtx[tet_idx].x];
                        struct flt3 b = vtx_xyz[tet_vtx[tet_idx].y];
                        struct flt3 c = vtx_xyz[tet_vtx[tet_idx].z];
                        struct flt3 d = vtx_xyz[tet_vtx[tet_idx].w];
                        
                        //expand bottom row
                        float detA = - det3(b,c,d) + det3(a,c,d) - det3(a,b,d) + det3(a,b,c);
                        
                        //barycentric coords
                        struct flt4 lam;
                        lam.x = (- det3(b,c,d) + det3(x,c,d) - det3(x,b,d) + det3(x,b,c))/detA;
                        lam.y = (- det3(x,c,d) + det3(a,c,d) - det3(a,x,d) + det3(a,x,c))/detA;
                        lam.z = (- det3(b,x,d) + det3(a,x,d) - det3(a,b,d) + det3(a,b,x))/detA;
                        lam.w = (- det3(b,c,x) + det3(a,c,x) - det3(a,b,x) + det3(a,b,c))/detA;
                        
                        //all +ve
                        int tst = (lam.x>=0e0f)&&(lam.y>=0e0f)&&(lam.z>=0e0f)&&(lam.w>=0e0f);
                        
                        if(tst)
                        {
                            vxl_tag[vxl_idx] = tet_tag[tet_idx];
                            
                            break;
                        }
                        
                    }//nrm
                    
                } //tets
                
                if(vxl_idx%10000 == 0)
                {
                    printf("%6d %f\n",vxl_idx, vxl_tag[vxl_idx]);
                }
            }//vxls
        }
    }
}



#endif /* vxl_h */
