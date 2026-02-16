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

void file_read(char *file_name, void *buf, size_t n, size_t w)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_READ, file_name);

    FILE* file_ptr = fopen(file_path,"rb");
    fread(buf, w, n, file_ptr);
    
    fclose(file_ptr);
    
    return;
}

void file_write(char *file_name, void *buf, size_t n, size_t w)
{
    char file_path[250];
    sprintf(file_path, "%s/%s", ROOT_WRITE, file_name);

    FILE* file_ptr = fopen(file_path,"wb");
    fwrite(buf, w, n, file_ptr);
    
    fclose(file_ptr);
    
    return;
}


#endif /* io_h */
