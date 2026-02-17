//
//  main.c
//  emi4
//
//  Created by Toby Simpson on 16.02.2026.
//

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "vec.h"
#include "tet.h"
#include "vxl.h"
#include "io.h"
#include "xdmf.h"


//doing mesh on cpu
int main(int argc, const char * argv[])
{
    printf("hello\n");
    
    struct vxl_obj vxl;
    vxl.dx = 2e0f;
    vxl.x0 = (struct flt3){0e0f,0e0f,0e0f};
    vxl.x1 = (struct flt3){664.59f,139.78f,139.92f};
    vxl_ini(&vxl);
    
    struct tet_obj tet;
    tet.nv = file_size("vtx_xyz.dat")/sizeof(struct flt3);
    tet.ne = file_size("tet_vtx.dat")/sizeof(struct int4);
    
    printf("tet.nv %zu\n", tet.nv);
    printf("tet.ne %zu\n", tet.ne);
    
    //alloc
    struct flt3 *vtx_xyz = malloc(tet.nv*sizeof(struct flt3));
    struct int4 *tet_vtx = malloc(tet.ne*sizeof(struct int4));
    float       *tet_tag = malloc(tet.ne*sizeof(float));
    struct flt3 *tet_ctr = malloc(tet.ne*sizeof(struct flt3));
    float       *vxl_tag = malloc(vxl.ne_tot*sizeof(float));
    
    //read
    file_read("vtx_xyz.dat", vtx_xyz, tet.nv, sizeof(struct flt3));
    file_read("tet_vtx.dat", tet_vtx, tet.ne, sizeof(struct int4));
    file_read("tet_tag.dat", tet_tag, tet.ne, sizeof(float));
    
    
    //centres
    for(int tet_idx=0; tet_idx<tet.ne; tet_idx++)
    {
        //verts
        struct flt3 a = vtx_xyz[tet_vtx[tet_idx].x];
        struct flt3 b = vtx_xyz[tet_vtx[tet_idx].y];
        struct flt3 c = vtx_xyz[tet_vtx[tet_idx].z];
        struct flt3 d = vtx_xyz[tet_vtx[tet_idx].w];
        
        struct flt3 ctr = {0.25f*(a.x + b.x + c.x + d.x), 0.25f*(a.y + b.y + c.y + d.y), 0.25f*(a.z + b.z + c.z + d.z)};
        
        tet_ctr[tet_idx] = ctr;
    }
    
    
    
    
    
    struct int3 pos;
    
    
    //loop vxl
    for(pos.z=0; pos.z<vxl.ne.z; pos.z++)
    {
        for(pos.y=0; pos.y<vxl.ne.y; pos.y++)
        {
            for(pos.x=0; pos.x<vxl.ne.x; pos.x++)
            {
                int vxl_idx = pos.x + pos.y*vxl.ne.x + pos.z*vxl.ne.x*vxl.ne.y;
                
                //reset
                vxl_tag[vxl_idx] = 0e0f;
                
                struct flt3 x = {vxl.x0.x + vxl.dx*(pos.x + 0.5f), vxl.x0.y + vxl.dx*(pos.y + 0.5f), vxl.x0.z + vxl.dx*(pos.z + 0.5f)};
                
                //printf("%6d %e %e %ef\n", vxl_idx, x.x, x.y, x.z);
                
                //loop tets
                for(int tet_idx=0; tet_idx<tet.ne; tet_idx++)
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
    
    
    //write
    write_xmf(&tet, &vxl, 0);
    file_write("vxl_tag.dat", vxl_tag, vxl.ne_tot, sizeof(float));
    file_write("tet_ctr.dat", tet_ctr, vxl.ne_tot, sizeof(struct flt3));
    
    //archive
    char file_name[50];
    sprintf(file_name, "vxl_res/vxl_tag%02.0f.dat", vxl.dx);
    file_write(file_name, vxl_tag, vxl.ne_tot, sizeof(float));
    
    
    //clean
    free(vtx_xyz);
    free(tet_vtx);
    free(vxl_tag);
    
    printf("done\n");
    
    return EXIT_SUCCESS;
}
