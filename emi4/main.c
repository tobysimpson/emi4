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
    vxl.dx = 10e0f;
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
    float       *vxl_tag = malloc(vxl.ne_tot*sizeof(float));
    
    //read
    file_read("vtx_xyz.dat", vtx_xyz, tet.nv, sizeof(struct flt3));
    file_read("tet_vtx.dat", tet_vtx, tet.ne, sizeof(struct int4));
    file_read("tet_tag.dat", tet_tag, tet.ne, sizeof(float));
    
    
//    //disp
//    for(int i=0; i<3; i++)
//    {
//        printf("%6d %6d %6d %6d\n", tet_vtx[i].x, tet_vtx[i].y, tet_vtx[i].z, tet_vtx[i].w);
//        printf("%e %e %e\n", vtx_xyz[i].x, vtx_xyz[i].y, vtx_xyz[i].z);
//    }

    struct int3 pos;
    
    //disp
    for(pos.z=0; pos.z<vxl.ne.z; pos.z++)
    {
        //disp
        for(pos.y=0; pos.y<vxl.ne.y; pos.y++)
        {
            //disp
            for(pos.x=0; pos.x<vxl.ne.x; pos.x++)
            {
                int vxl_idx = pos.x + pos.y*vxl.ne.x + pos.z*vxl.ne.x*vxl.ne.y;
                
                struct flt3 x = {vxl.x0.x + vxl.dx*(pos.x + 0.5f), vxl.x0.y + vxl.dx*(pos.y + 0.5f), vxl.x0.z + vxl.dx*(pos.z + 0.5f)};
                
                //printf("%6d %e %e %ef\n", vxl_idx, x.x, x.y, x.z);
                
                for(int tet_idx=0; tet_idx<tet.ne; tet_idx++)
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
                    
                    if((lam.x>=0e0f)&&(lam.y>=0e0f)&&(lam.z>=0e0f)&&(lam.w>=0e0f))
                    {
//                        printf("%6d %6d\n",vxl_idx,tet_idx);
                        
                        vxl_tag[vxl_idx] = tet_tag[tet_idx];
                        
                        break;
                    }
                }
            }
        }
    }
    
    
    
    //write
    write_xmf(&tet, &vxl, 0);
    file_write("vxl_tag.dat", vxl_tag, vxl.ne_tot, sizeof(float));
    
    
    //clean
    free(vtx_xyz);
    free(tet_vtx);
    free(vxl_tag);
    
    printf("done\n");
    
    return EXIT_SUCCESS;
}
