//
//  io.h
//  emi4
//
//  Created by Toby Simpson on 16.02.2026.
//

#ifndef io_h
#define io_h

#define ROOT_READ   "/Users/toby/Downloads"
#define ROOT_WRITE  "/Users/toby/Downloads"


//file length in bytes
long file_size(char* file_name)
{
    FILE*   file_ptr;
    char    file_path[250];
    
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);
    file_ptr = fopen(file_path,"rb");
    fseek(file_ptr, 0, SEEK_END);
    long n = ftell(file_ptr);
    fseek(file_ptr, 0, SEEK_SET);
    fclose(file_ptr);
    
    return n;
}


void read_file(char *file_name, void *buf, size_t n, size_t w)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);

    FILE* file_ptr = fopen(file_path,"rb");
    fread(buf, w, n, file_ptr);
    
    fclose(file_ptr);
    
    return;
}


void read_int4(char *file_name, struct int4 *buf, size_t n)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);

    FILE* file_ptr = fopen(file_path,"rb");
    fread(buf, sizeof(struct int4), n, file_ptr);
    
    fclose(file_ptr);
    
    return;
}


void read_flt3(char *file_name, struct flt3 *buf, size_t n)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);

    FILE* file_ptr = fopen(file_path,"rb");
    fread(buf, sizeof(struct flt3), n, file_ptr);
    
    fclose(file_ptr);
    
    return;
}


#endif /* io_h */
