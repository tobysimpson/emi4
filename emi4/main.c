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



//tet to voxel mesh on cpu
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
    struct flt3 *tet_ctr = malloc(tet.ne*sizeof(struct flt3));
    float       *vxl_tag = malloc(vxl.ne_tot*sizeof(float));
    
    //read
    file_read("vtx_xyz.dat", vtx_xyz, tet.nv, sizeof(struct flt3));
    file_read("tet_vtx.dat", tet_vtx, tet.ne, sizeof(struct int4));
    file_read("tet_tag.dat", tet_tag, tet.ne, sizeof(float));
    
    
    //centres
    tet_bary(&tet, tet_vtx, vtx_xyz, tet_ctr);
    
    //map tags
    vxl_map(&tet, &vxl, tet_vtx, vtx_xyz, tet_ctr, tet_tag, vxl_tag);
    
    
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
