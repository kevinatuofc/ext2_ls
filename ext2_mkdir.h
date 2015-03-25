
#include "ext2_ls.h"

void ext2_mkdir(char* path){
    const int NOT_FOUND = -1; 
    int n = -1;
    int is_regular_file;
    int leaf_node = NOT_FOUND;
    int parent_inode = 0; 
    int i; 

    dir_entry_list_t* parent_entry_list = NULL;

     /* read root dir */
    dir_entry_list_t*  current_entry_list = read_root_entry_list();

    /* split the path into n splits */
    char** splits = str_split(path, '/', &n);
    int parent_path_exist = -1;

    /* loop through 0 to n-2 of splits and make sure the parent inode exists */
    for ( i=0; i < n; i++ ){
	int found_inode = NOT_FOUND;
	int is_dir = -1; 
	dir_entry_list_t* tmp; 

	list_for_each_entry(tmp, &current_entry_list->list, list){
	    if ( strcmp(tmp->e->name, splits[i]) == 0 ){
		found_inode = tmp->e->inode;
		is_regular_file = 0 ;
		/* found, then update the current entry list*/
		dir_entry_list_t* tmp2 = read_inode(found_inode, &is_regular_file);	     
	    /* if it is a regular file, then */
		if( tmp == NULL ){		    
		    assert(is_regular_file); /* invalid combination */
		    /* found_inode = -1; */
		}else{
		    current_entry_list = tmp2;
		}		  
		break;
	    }
	}

	if ( found_inode == NOT_FOUND ){		
	    if(i != (n-1)){	/* not reaching the end. Some parent directories  */
		parent_path_exist = 0;
	    /* TODO: free resources, too */
		current_entry_list = NULL;
	    }
	    else		/* NOT FOUND and it is not the leaf, meaning the parent path does not exist */
		parent_path_exist = 1;
	    break;
	}else{ 			/* if found */
	    if ( i == (n-2) ){	/* the intermediat parent directory */
		parent_inode = found_inode;
		parent_entry_list = current_entry_list;
	    }
	    if ( i == (n-1) ){	/* the intermediat parent directory */
		leaf_node = found_inode;
	    }
	}
    }

    if ( leaf_node == NOT_FOUND && parent_path_exist ){	/* not found the inode */
	/* create a new inode structure */
	struct ext2_inode* new_inode = (struct ext2_inode*) malloc(sizeof(struct ext2_inode));
	assert (parent_entry_list);
	assert (parent_inode);
	
	/* 2 things to determine: 
	   A. inode number of the new inode, determined by reading the inode bitmap block from group desc
	   B. data block no. (in which to store '.' and '..' entries, by reading the data bitmap block of the group descriptor */
	unsigned int new_inode_no = allocate_inode(); 
	unsigned int new_data_block = allocate_data_block();

#if DEBUG
	printf("new inode no.  %d\n", new_inode_no);
	printf("new data block no.  %d\n", new_data_block);
#endif
	/* write the new inode structure and data blocks into the disk */
	/* update superblock, free blocks and inodes count
	   update group descriptor: two bitmaps, free blocks and inodes count, diretory count */    	        
    }else{

	if (leaf_node != NOT_FOUND){	/* found the inode */
	    printf("ext2_mkdir: cannot create directory ‘%s’: File exists\n", path);
	    exit(0);
	}

	if ( !parent_path_exist ){	    	   
	    printf("ext2_mkdir: cannot create directory ‘%s’: No such file or directory\n", path);
	    exit(0);
	}
    }
    
}
