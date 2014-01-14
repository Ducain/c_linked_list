#include <stdio.h>
#include <stdlib.h> //For mac/linux
//#include <windows.h> //Have to include this to make use of malloc()

//STRUCT DEFS

struct LinkNode
{
	int node_val;
	struct LinkNode *next_node;
};

//FUNCTION DEFS

void create_list();
int count_list_nodes(struct LinkNode *root_node);
void print_list_values(struct LinkNode *root_node);
struct LinkNode * find_address_of_last_node(struct LinkNode *root_node);
struct LinkNode * add_item_to_list(int value_to_add, struct LinkNode *root_node);
void remove_item_from_list(struct LinkNode *node_to_remove, struct LinkNode *root_node);
void free_list_memory(struct LinkNode **root_node);

//FUNCTIONS

int main()
{
	printf("\n");
	create_list();
	printf("\n");
	printf("\n");
}

//Launch method, instead of having all this main basically...
void create_list()
{
	printf("Assembling list nodes...\n");

	//We're assembling all this, correctly malloc()ing,
	//instead of creating inside this function with struct
	//vars, which would only be scoped to this function.
	//
	//First, setup the root.  If anything else in this app needs this,
	//we'll pass it as a param, instead of using a global var.
	struct LinkNode *root_node = malloc(sizeof(struct LinkNode));
	root_node->node_val = 9;
	root_node->next_node = NULL;

	add_item_to_list(12, root_node);

	//Add an item and store a pointer to it for future deletion test
	struct LinkNode *node_to_delete = add_item_to_list(10, root_node);

	add_item_to_list(22, root_node);
	add_item_to_list(37, root_node);
	
	printf("\nIterate through nodes and print values...\n\n");
	print_list_values(root_node);
	
	printf("\nAdding value 55 to end of list...\n");
	add_item_to_list(55, root_node);

	printf("\nNow reading all values again...\n\n");
	print_list_values(root_node);

	printf("\nRemoving item with value 10 from the list\n");

	//Do removal...
	remove_item_from_list(node_to_delete, root_node);

	printf("\nNow reading all values again...\n\n");
	print_list_values(root_node);

	printf("\nAdding 2 new values: 66 and 77...\n");

	add_item_to_list(66, root_node);
	add_item_to_list(77, root_node);

	printf("\nGet count of nodes...\n");
	int node_count = count_list_nodes(root_node);
	printf("\nNode count: %d\n",node_count);

	printf("\nNow reading all values again...\n\n");
	print_list_values(root_node);

	printf("\nFree()ing list memory...\n\n");
	free_list_memory(&root_node);

	printf("\nNow reading all values again...\n\n");
	print_list_values(root_node);
}

/*
Using a given root node, print all values from
nodes in the list.  We pass a POINTER to the 
root node because there would be no need to pass
this by value (copy).  We just need the address of it.
*/
void print_list_values(struct LinkNode *root_node)
{
	if(root_node == NULL)
	{
		printf("List does not exist.\n");
		return;
	}

	struct LinkNode *current_node;
	
	//Start at the root
	current_node = root_node;

	//NOTE: to run this for loop, I had to compile with the -std=c99 option
	for(struct LinkNode *i = root_node; i != NULL; i = i->next_node)
	{
		printf("Current node val: %d next address: %p\n",i->node_val,i->next_node);
	}
}

//Notice the way we return a pointer to a struct here.
//
struct LinkNode * find_address_of_last_node(struct LinkNode *root_node)
{
	struct LinkNode *current_node;
	
	//Start at the root
	current_node = root_node;

	for(struct LinkNode *i = root_node; i != NULL; i = i->next_node)
	{
		current_node = i;
	}

	//This threw me for a minute.  Remember, the return value of this
	//function is a pointer to a LinkNode struct.  Our internal variable
	//current_node IS a pointer.  Because it is a pointer, you return it, 
	//NOT the address of it, such as &current_node.  That produces an error,
	//as it should, because that is trying to return the address of the 
	//current_node variable itself, and NOT the address it is pointing to.
	//Ugh.
	return current_node;
}

struct LinkNode * add_item_to_list(int value_to_add, struct LinkNode *root_node)
{
	//I have to use malloc here to create a truly new LinkNode.  This
	//is similar to the concept of New in the OO world.  I had defined this
	//as a static struct earlier, but then failed to realize that static
	//variable stays active throughout the life of the app, and I was 
	//overwriting it.  Ouch.  This takes care of that.  Each time a 
	//new item is needed, this code gets memory for it, and adds it to the 
	//end of the list.
	//
	//Malloc returns a void pointer, of the sizeof(whatever).  We cast the
	//void pointer to a pointer of type(whatever) at the beginning.
	//
	//Correction: I don't need this:
	//http://stackoverflow.com/questions/605845/do-i-cast-the-result-of-malloc
	//
	//struct LinkNode *new_node = (struct LinkNode *)malloc(sizeof(struct LinkNode));
	//
	struct LinkNode *new_node = malloc(sizeof(struct LinkNode));
	new_node->node_val = value_to_add;
	new_node->next_node = NULL;

	//find address of last node
	struct LinkNode *last_node; //pointer to last node in the list
	last_node = find_address_of_last_node(root_node); //Returns address of last node

	if(last_node != NULL)
	{
		//Must access property through dereference operator
		last_node->next_node = new_node; //Add new node to the end
		return new_node;
	}else
	{
		return NULL;
	}
}

void remove_item_from_list(struct LinkNode *node_to_remove, struct LinkNode *root_node)
{
	//We have to:
	//1) find node in list
	//2) set the next pointer of the prev node to the address
	//   of the node AFTER it

	struct LinkNode *node_before = root_node;
	struct LinkNode *node_after = NULL;
	
	for(struct LinkNode *i = root_node; i != NULL; i = i->next_node)
	{
		//Is this the node/droid we're looking for?
		if(i == node_to_remove)
		{
			//Set next_node pointer of node BEFORE this one to the
			//pointer of the next_node of THIS one.  Essentially, we
			//just unplug this node.
			node_before->next_node = i->next_node;
			break; //no need to go further
		}else
		{
			node_before = i;
		}
	}
}

int count_list_nodes(struct LinkNode *root_node)
{
	int counter = 0;

	for(struct LinkNode *i = root_node; i != NULL; i = i->next_node)
	{
		counter += 1;
		//printf("Node: %d\n",ii);
	}

	return counter;
}

//Free all elements of the linked list
void free_list_memory(struct LinkNode **root_node)
{
	//NOTE: **root_node is a pointer VAR that points
	//to the ADDRESS of the *root_node pointer VAR.
	//
	//List the address of all the pointers in the list,
	//held in the next_node property
	//

	struct LinkNode **p = &(*root_node); //Used for iterating

	int node_count = count_list_nodes(*root_node); //Total nodes

	struct LinkNode **ptr_array[node_count]; //Array of pointers to nodes

	//Fill array with pointers to nodes
	ptr_array[0] = &(*root_node);

	int i = 1;

	while(1 == 1)
	{
		if((*p)->next_node != NULL)
		{
			ptr_array[i] = &(*p)->next_node;
			p = &(*p)->next_node;
		}else
		{
			break;
		}

		i += 1;
	}

	//Note: We need to free the child nodes FIRST  before the root
	//node, because of the next_node pointer dependencies.  If we free
	//the root node, then the entire block allocated to that structure, including
	//the part reserved for the next_node pointer, can be used at will by
	//the OS.
	for(int c = node_count - 1; c > -1; c -= 1)
	{
		//printf("Ptr address at array index [%d]: %p\n",c,&(ptr_array[c]));
		struct LinkNode **ptr = ptr_array[c];

		if(ptr != NULL)
		{
			//printf("c is: %d\n",c);
			//printf("pointer var address: %p\n",&(*ptr));
			//printf("pointer value address: %p\n",*ptr);
			//printf("pointer value: %d\n",(*ptr)->node_val);

			//Note that *ptr (1st level dereference of ptr), gets
			//item pointed to by ptr, which IS a pointer.
			//
			//Further note: if you wanted to get the item pointed at
			//by the pointer stored at *ptr, you would do **ptr.
			//
			free(*ptr);
			*ptr = NULL;
		} 
	}//end for loop

}




























