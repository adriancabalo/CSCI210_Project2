#include "types.h"

extern struct NODE* root;
extern struct NODE* cwd;

// Attach node to parent helper function
void attachNodeToParent(struct NODE* parent, const char* name, char type) {
    struct NODE* newNode = (struct NODE*)malloc(sizeof(struct NODE));
    strcpy(newNode->name, name);
    newNode->fileType = type;
    newNode->childPtr = NULL;
    newNode->siblingPtr = NULL;
    newNode->parentPtr = parent;

    // Attach the node to the parent's child list
    if (parent->childPtr == NULL) {
        parent->childPtr = newNode;
    } else {
        struct NODE* child = parent->childPtr;
        while (child->siblingPtr != NULL) {
            child = child->siblingPtr;
        }
        child->siblingPtr = newNode;
    }
}

//make directory
void mkdir(char pathName[]){

    // TO BE IMPLEMENTED
    //
    // YOUR CODE TO REPLACE THE PRINTF FUNCTION BELOW
    //
    char baseName[64];
    char dirName[256];

    struct NODE* parentNode = splitPath(pathName, baseName, dirName);

    if (strlen(baseName) == 0) {
        printf("MKDIR ERROR: no path provided\n");
        return;
    }

    if (parentNode == NULL) {
        return;  // splitPath() handles non-existent directories
    }

    // Check if the directory already exists
    struct NODE* child = parentNode->childPtr;
    while (child != NULL) {
        if (strcmp(child->name, baseName) == 0 && child->fileType == 'D'){
		printf("MKDIR ERROR: directory %s already exists\n", baseName);
		return;
        }
        child = child->siblingPtr;
    }

    // Create the new directory node
    attachNodeToParent(parentNode, baseName, 'D');
    printf("MKDIR SUCCESS: node %s successfully created\n", pathName);
    
}

//handles tokenizing and absolute/relative pathing options
struct NODE* splitPath(char* pathName, char* baseName, char* dirName){

    // TO BE IMPLEMENTED
    // NOTE THAT WITHOUT COMPLETING THIS FUNCTION CORRECTLY
    // rm, rmdir, ls, cd, touch COMMANDS WILL NOT EXECUTE CORRECTLY
    // SEE THE PROVIDED EXECUTABLE TO SEE THEIR EXPECTED BEHAVIOR

    // YOUR CODE HERE
    struct NODE* currentNode;
    
    // Determine if the path is absolute or relative
    if (pathName[0] == '/') {
        currentNode = root; // Start from root for absolute paths
    }
    else {
        currentNode = cwd;  // Start from current directory for relative paths
    }

    // Root case
    if (strcmp(pathName, "/") == 0) {
        strcpy(dirName, "/");
        strcpy(baseName, "");
        return root;
    }

	
    int i = 0;
    int lastIndex = -1;

    while(pathName[i] != 0){
	if(pathName[i] == '/'){
		lastIndex = i;
	}
	i++;

    }
	
    if (lastIndex == -1) {
        strcpy(baseName, pathName); // Copy the entire pathName as baseName
        strcpy(dirName, "");        // dirName remains an empty string
        return currentNode;         // Return current node (cwd or root)
    }

    strcpy(baseName, pathName + lastIndex + 1);

    if (lastIndex == 0){
	    strcpy(dirName, "/");
    }
    else if (lastIndex > 0) {
	    strncpy(dirName, pathName, lastIndex +1);
	    dirName[lastIndex] = '\0';
    }
    else{
	    strcpy(dirName,"");
    }

    // Debugging Line
    //printf("Base Name is: %s\n",baseName);
    //printf("Dir Name is: %s\n",dirName);

    // Tokenizing
    
    char tempDirName[200];
    strcpy(tempDirName, dirName);
    char* token = strtok(tempDirName, "/");

    while (token != NULL) {
        // Check if the current node has a child with the name of the token
        struct NODE* child = currentNode->childPtr;
	int found = 0;

        // Traverse the children of the current nod
        while (child != NULL) {
            if (strcmp(child->name, token) == 0 && child->fileType == 'D') {
                currentNode = child;
		found = 1;
                break;
            }
            child = child->siblingPtr; // Move to the next sibling
        }

        // If the directory does not exist, print an error message and return NULL
        if (!found) {
            printf("ERROR: directory %s does not exist\n", token);
            return NULL; // Return NULL for non-existing directory
        }

        token = strtok(NULL, "/"); // Get the next token
    }

    // Return the current node, which is now the last valid directory in the path
    return currentNode; // Return the current directory node
	
}
