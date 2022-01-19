#include "common.h"
#include "string.h"



typedef struct
{
	char *name;
	uint32_t size;
	uint32_t disk_offset;
} file_info;

typedef struct
{
	bool used;
	uint32_t index;
	uint32_t offset;
} Fstate;

enum
{
	SEEK_SET,
	SEEK_CUR,
	SEEK_END
};

static const file_info file_table[] = {
	{"1.rpg", 188864, 1048576}, {"2.rpg", 188864, 1237440}, {"3.rpg", 188864, 1426304}, {"4.rpg", 188864, 1615168}, {"5.rpg", 188864, 1804032}, {"abc.mkf", 1022564, 1992896}, {"ball.mkf", 134704, 3015460}, {"data.mkf", 66418, 3150164}, {"desc.dat", 16027, 3216582}, {"fbp.mkf", 1128064, 3232609}, {"fire.mkf", 834728, 4360673}, {"f.mkf", 186966, 5195401}, {"gop.mkf", 11530322, 5382367}, {"map.mkf", 1496578, 16912689}, {"mgo.mkf", 1577442, 18409267}, {"m.msg", 188232, 19986709}, {"mus.mkf", 331284, 20174941}, {"pat.mkf", 8488, 20506225}, {"rgm.mkf", 453202, 20514713}, {"rng.mkf", 4546074, 20967915}, {"sss.mkf", 557004, 25513989}, {"voc.mkf", 1997044, 26070993}, {"wor16.asc", 5374, 28068037}, {"wor16.fon", 82306, 28073411}, {"word.dat", 5650, 28155717}, {"credits_bgm.wav", 6631340, 1048576},
};

#define NR_FILES (sizeof(file_table) / sizeof(file_table[0]))

Fstate files[NR_FILES + 3];

void ide_read(uint8_t *, uint32_t, uint32_t);
void ide_write(uint8_t *, uint32_t, uint32_t);

/* Please implement a simple file system here */

int fs_open(const char *pathname, int flags)
{
    int index = -1;
    /*if(strcmp(pathname, "stdin") == 0) index = 0;
    else if(strcmp(pathname, "stdout") == 0) index = 1;
    else if (strcmp(pathname, "stderr") == 0) index = 2;
    if(index != -1){
        files[index].used = true;
        files[index].offset = 0;
        files[index].index = index;
        return index;
    }*/
    for(int i = 3; i < NR_FILES+3; ++i){
        if(strcmp(file_table[i-3].name, pathname) == 0){
            index = i;
            files[i].used = true;
            files[i].index = i;
            files[i].offset = 0;
            return index;
        }
    }
    printk("No such a file whose name is %s\n", pathname);
    assert(0);
	return -1;
}

size_t fs_read(int fd, void *buf, size_t len)
{
	assert(fd > 2);
    assert(fd < NR_FILES + 3);
	//panic("Please implement fs_read at fs.c");
    
    if(!files[fd].used){
        printk("FIle has not yet been opened\n");
        assert(0);
        return -1;
    }
    size_t to_read_size = file_table[fd-3].size - files[fd].offset;
    len = (len > to_read_size ? to_read_size : len);
    ide_read(buf, file_table[fd-3].disk_offset + files[fd].offset, len);
    files[fd].offset += len;
	return len;
}

size_t fs_write(int fd, void *buf, size_t len)
{
	assert(fd <= 2);
#ifdef HAS_DEVICE_SERIAL
	int i;
	extern void serial_printc(char);
	for (i = 0; i < len; i++)
	{
		serial_printc(((char *)buf)[i]);
	}
#else
	asm volatile(".byte 0x82"
				 : "=a"(len)
				 : "a"(4), "b"(fd), "c"(buf), "d"(len));
#endif

	return len;
}

off_t fs_lseek(int fd, off_t offset, int whence)
{
    
	//panic("Please implement fs_lseek at fs.c");
    assert(fd > 2);
    assert(fd < NR_FILES + 3);
	switch(whence){
        case SEEK_SET:{
            if(offset >= 0 && offset <= file_table[fd - 3].size){
                files[fd].offset = offset;
                return files[fd].offset;
            }
            break;
        }
        case SEEK_CUR:{
            if(files[fd].offset + offset >= 0 && files[fd].offset + offset <= file_table[fd - 3].size){
                files[fd].offset = files[fd].offset + offset;
                return files[fd].offset;
            }
            break;
        }
        case SEEK_END:{
            if(file_table[fd - 3].size + offset >= 0 && file_table[fd - 3].size + offset <= file_table[fd - 3].size){
                files[fd].offset = file_table[fd - 3].size + offset;
                return files[fd].offset;
            }
            break;
        }
    }
	return -1;
}

int fs_close(int fd)
{
    assert(fd >= 0);
    assert(fd < NR_FILES + 3);
    files[fd].used = false;
    files[fd].index = 0;
    files[fd].offset = 0;
	return 0;
	//panic("Please implement fs_close at fs.c");
}
