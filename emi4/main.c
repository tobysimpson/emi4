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
    
    //read
//    read_int4("tet_vtx.dat", tet_vtx, tet.ne);
    read_file("tet_vtx.dat", tet_vtx, tet.ne, sizeof(struct int4));
    read_file("vtx_xyz.dat", vtx_xyz, tet.nv, sizeof(struct flt3));
    
    
    //disp
    for(int i=0; i<3; i++)
    {
        printf("%6d %6d %6d %6d\n", tet_vtx[i].x, tet_vtx[i].y, tet_vtx[i].z, tet_vtx[i].w);
    }
    
    //disp
    for(int i=0; i<3; i++)
    {
        printf("%e %e %e\n", vtx_xyz[i].x, vtx_xyz[i].y, vtx_xyz[i].z);
    }

    
    //clean
    free(tet_vtx);
    
    printf("done\n");
    
    return EXIT_SUCCESS;
}
