#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <elf.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


char* getType(int x){
    return x == PT_LOAD? "LOAD":
            x == PT_NULL? "NULL":
            x == PT_DYNAMIC? "DYNAMIC":
            x == PT_INTERP? "INTERP":
            x == PT_SHLIB? "SHLIB":
            x == PT_PHDR? "PHDR":
            x == PT_GNU_STACK? "GNU_STACK":
            x == PT_TLS? "TLS":
            x == PT_NUM? "NUM":
            x == PT_LOOS? "LOOS":
            x == PT_GNU_EH_FRAME? "GNU_EH_FRAME":
            x == PT_GNU_RELRO? "GNU_RELRO":
            x == PT_LOSUNW? "LOSUNW":
            x == PT_SUNWBSS? "SUNWBSS":
            x == PT_SUNWSTACK? "SUNWSTACK":
            x == PT_HISUNW? "HISUNW":
            x == PT_HIOS? "HIOS":
            x == PT_LOPROC? "LOPROC":
            "HIPROC";
}
char* getFlag(int x){
    return x == 5? "R E":
        x == 4? "R":
        "RW";
}



int main(int argc, char **argv){
    int fd = open(argv[1],O_RDONLY);
    void* map_start;
    struct stat fd_stat;
    Elf32_Ehdr *header;
    if(fstat(fd,&fd_stat)!=0){
        perror("error in open");
        munmap(map_start,fd_stat.st_size);
        close(fd);
        exit(-1);
    }
    if((map_start = mmap( NULL,fd_stat.st_size,PROT_READ , MAP_SHARED, fd, 0))==MAP_FAILED){
        perror("ERROR: ");
        close(fd);
        exit(-1);
    }
    header = (Elf32_Ehdr*)map_start;
    if(header->e_type == ET_EXEC){
        Elf32_Phdr* program_headers = (Elf32_Phdr*)(map_start + header->e_phoff);
        printf("Type\t\tOffset\t\tVirtAddr\tPhysAddr\tFileSiz\tMemSiz\tFlg\tAlign\n");
        for (size_t i = 0; i < header->e_phnum; i++)
        {
            printf("%-10s\t%#09x\t%#09x\t%#09x\t%#06x\t%#06x\t%s\t%#0x\n",
            getType(program_headers[i].p_type),
            program_headers[i].p_offset,
            program_headers[i].p_vaddr,
            program_headers[i].p_paddr,
            program_headers[i].p_filesz,
            program_headers[i].p_memsz,
            getFlag(program_headers[i].p_flags),
            program_headers[i].p_align);
        }
    }
    munmap(map_start,fd_stat.st_size);
    close(fd);
    return 0;
}
