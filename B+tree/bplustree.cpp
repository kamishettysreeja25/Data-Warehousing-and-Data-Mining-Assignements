#include<bits/stdc++.h>
//#include "insertbplus.h"
using namespace std;

#define MAX 10000
int M,B;
int numberOfPointers ;//= numberOfNodes + 1;

struct Block{
    //number of nodes
    int tNodes;
    //for parent Block and index
    Block *parentBlock;
    //values
    int value[MAX];
    //child Blocks
    Block *childBlock[MAX];
    Block(){ //constructor to initialize a block
        tNodes = 0;
        parentBlock = NULL;
        for(int i=0; i<MAX; i++){
            value[i] = INT_MAX;
            childBlock[i] = NULL;
        }
    }
};
//declare root Block
Block *rootBlock = new Block();

//function to split the leaf nodes
void splitLeaf(Block *curBlock){
    int x, i, j;

    //split the greater half to the left when numberOfPointer is odd
    //else split equal equal when numberOfPointer is even
    if(numberOfPointers%2)
        x = (numberOfPointers+1)/2;
    else
        x = numberOfPointers/2;

    //we don't declare another block for leftBlock, rather re-use curBlock as leftBlock and
    //take away the right half values to the rightBlock
    Block *rightBlock = new Block();

    //so leftBlock has x number of nodes
    curBlock->tNodes = x;
    //and rightBlock has numberOfPointers-x
    rightBlock->tNodes = numberOfPointers-x;
    //so both of them have their common parent [even parent may be null, so both of them will have null parent]
    rightBlock->parentBlock = curBlock->parentBlock;

    for(i=x, j=0; i<numberOfPointers; i++, j++){
        //take the right-half values from curBlocks and put in the rightBlock
        rightBlock->value[j] = curBlock->value[i];
        //and erase right-half values from curBlock to make it real leftBlock
        //so that it does not contain all values only contains left-half values
        curBlock->value[i] = INT_MAX;
    }
    //for splitting the leaf blocks we copy the first item from the rightBlock to their parentBlock
    //and val contains that value
    int val = rightBlock->value[0];

    //if the leaf itself is a parent then
    if(curBlock->parentBlock==NULL){
        //it has null parent, so create a new parent
        Block *parentBlock = new Block();
        //and new parent should have a null parent
        parentBlock->parentBlock = NULL;
        //new parent will have only one member
        parentBlock->tNodes=1;
        //and that member is val
        parentBlock->value[0] = val;
        //so the parent has two child, so assign them (don't forget curBlock is actually the leftBlock)
        parentBlock->childBlock[0] = curBlock;
        parentBlock->childBlock[1] = rightBlock;
        //their parent of the left and right blocks is no longer null, so assign their parent
        curBlock->parentBlock = rightBlock->parentBlock = parentBlock;
        //from now on this parentBlock is the rootBlock
        rootBlock = parentBlock;
        return;
    }
    else{   //if the splitted leaf block is not rootBlock then

        // we have to put the val and assign the rightBlock to the right place in the parentBlock
        // so we go to the parentBlock and from now we consider the curBlock as the parentBlock of the splitted Block

        curBlock = curBlock->parentBlock;

        //for the sake of insertNodeion sort to put the rightBlock and val in the exact position
        //of th parentBlock [here curBlock] take a new child block and assign rightBlock to it
        Block *newChildBlock = new Block();
        newChildBlock = rightBlock;

        //simple insertion sort to put val at the exact position of values[] in the parentBlock [here curBlock]

        for(i=0; i<=curBlock->tNodes; i++){
            if(val < curBlock->value[i]){
                swap(curBlock->value[i], val);
            }
        }

        //after putting val number of nodes gets increase by one
        curBlock->tNodes++;

         //simple insertNodeion sort to put rightBlock at the exact position
         //of childBlock[] in the parentBlock [here curBlock]

        for(i=0; i<curBlock->tNodes; i++){
            if(newChildBlock->value[0] < curBlock->childBlock[i]->value[0]){
                swap(curBlock->childBlock[i], newChildBlock);
            }
        }
        curBlock->childBlock[i] = newChildBlock;

        //we reordered some blocks and pointers, so for the sake of safety
        //all childBlocks' should have their parent updated
        for(i=0; curBlock->childBlock[i]!=NULL; i++){
            curBlock->childBlock[i]->parentBlock = curBlock;
        }
    }
}

//function to split the non leaf nodes
void splitNonLeaf(Block *curBlock){
    int x, i, j;

    //split the less half to the left when numberOfPointer is odd
    //else split equal equal when numberOfPointer is even.  n/2 does it nicely for us

    x = numberOfPointers/2;

    //declare rightBlock and we will use curBlock as the leftBlock
    Block *rightBlock = new Block();

    //so leftBlock has x number of nodes
    curBlock->tNodes = x;
    //rightBlock has numberOfPointers-x-1 children, because we won't copy and paste
    //rather delete and paste the first item of the rightBlock
    rightBlock->tNodes = numberOfPointers-x-1;
    //both children have their common parent
    rightBlock->parentBlock = curBlock->parentBlock;


    for(i=x, j=0; i<=numberOfPointers; i++, j++){
        //copy the right-half members to the rightBlock
        rightBlock->value[j] = curBlock->value[i];
        //and also copy their children
        rightBlock->childBlock[j] = curBlock->childBlock[i];
        //erase the right-half values from curBlock to make it perfect leftBlock
        //which won't contain only left-half values and their children
        curBlock->value[i] = INT_MAX;
        //erase all the right-half childBlocks from curBlock except the x one
        //because if left child has 3 nodes then it should have 4 childBlocks, so don't delete that child
        if(i!=x)curBlock->childBlock[i] = NULL;
    }

    //we will take a copy of the first item of the rightBlock
    //as we will delete that item later from the list
    int val = rightBlock->value[0];
    //just right-shift value[] and childBlock[] by one from rightBlock
    //to have no repeat of the first item for non-leaf Block
    memcpy(&rightBlock->value, &rightBlock->value[1], sizeof(int)*(rightBlock->tNodes+1));
    memcpy(&rightBlock->childBlock, &rightBlock->childBlock[1], sizeof(rootBlock)*(rightBlock->tNodes+1));

    //we reordered some values and positions so don't forget
    //to assign the children's exact parent

    for(i=0;curBlock->childBlock[i]!=NULL;i++){
        curBlock->childBlock[i]->parentBlock = curBlock;
    }
    for(i=0;rightBlock->childBlock[i]!=NULL;i++){
        rightBlock->childBlock[i]->parentBlock = rightBlock;
    }

    //if the splitted block itself a parent
    if(curBlock->parentBlock==NULL){
        //create a new parent
        Block *parentBlock = new Block();
        //parent should have a null parent
        parentBlock->parentBlock = NULL;
        //parent will have only one node
        parentBlock->tNodes=1;
        //the only value is the val
        parentBlock->value[0] = val;
        //it has two children, leftBlock and rightBlock
        parentBlock->childBlock[0] = curBlock;
        parentBlock->childBlock[1] = rightBlock;

        //and both rightBlock and leftBlock has no longer null parent, they have their new parent
        curBlock->parentBlock = rightBlock->parentBlock = parentBlock;

        //from now on this new parent is the root parent
        rootBlock = parentBlock;
        return;
    }
    else{   //if the splitted leaf block is not rootBlock then

        // we have to put the val and assign the rightBlock to the right place in the parentBlock
        // so we go to the parentBlock and from now we consider the curBlock as the parentBlock of the splitted Block
        curBlock = curBlock->parentBlock;

        //for the sake of insertNodeion sort to put the rightBlock and val in the exact position
        //of th parentBlock [here curBlock] take a new child block and assign rightBlock to it

        Block *newChildBlock = new Block();
        newChildBlock = rightBlock;

        //simple insertion sort to put val at the exact position of values[] in the parentBlock [here curBlock]


        for(i=0; i<=curBlock->tNodes; i++){
            if(val < curBlock->value[i]){
                swap(curBlock->value[i], val);
            }
        }

        //after putting val number of nodes gets increase by one
        curBlock->tNodes++;

        //simple insertNodeion sort to put rightBlock at the exact position
         //of childBlock[] in the parentBlock [here curBlock]

        for(i=0; i<curBlock->tNodes; i++){
            if(newChildBlock->value[0] < curBlock->childBlock[i]->value[0]){
                swap(curBlock->childBlock[i], newChildBlock);
            }
        }
        curBlock->childBlock[i] = newChildBlock;

        //we reordered some blocks and pointers, so for the sake of safety
        //all childBlocks' should have their parent updated
         for(i=0;curBlock->childBlock[i]!=NULL;i++){
            curBlock->childBlock[i]->parentBlock = curBlock;
        }
    }

}

void insertNode(Block *curBlock, int val){

    for(int i=0;i<=curBlock->tNodes; i++){
        if(val < curBlock->value[i] && curBlock->childBlock[i]!=NULL){
            insertNode(curBlock->childBlock[i], val);
            if(curBlock->tNodes==numberOfPointers)
                splitNonLeaf(curBlock);
            return;
        }
        else if(val < curBlock->value[i] && curBlock->childBlock[i]==NULL){
            swap(curBlock->value[i], val);
            //swap(curBlock->childBlock[i], newChildBlock);
            if(i == curBlock->tNodes){
                    curBlock->tNodes++;
                    break;
            }
        }
    }

    if(curBlock->tNodes==numberOfPointers){

            splitLeaf(curBlock);
    }
}


int find(vector < Block* > Blocks, int num){
    vector < Block* > newBlocks;
    for(int i=0; i<Blocks.size(); i++){ //for every block
        Block *curBlock = Blocks[i];
        int j;
        for(j=0; j< curBlock->tNodes; j++){  //traverse the childBlocks, print values and save all the childBlocks
            if(num == curBlock->value[j])
                cout << "YES" << endl;
            if(curBlock->childBlock[j]!=NULL)
                newBlocks.push_back(curBlock->childBlock[j]);
        }
        if(curBlock->value[j]==INT_MAX && curBlock->childBlock[j]!=NULL)
            newBlocks.push_back(curBlock->childBlock[j]);

    }

    if(newBlocks.size()==0){ //if there is no childBlock block left to send out then just the end of the recursion
        Blocks.clear();
        //exit(0);
    }
    else {                    //else send the childBlocks to the recursion to continue to the more depth
        Blocks.clear();
        find(newBlocks, num);
    }
    return 0;
}

void count_number(vector < Block* > Blocks, int num, int tmp){
        vector < Block* > newBlocks;
    for(int i=0; i<Blocks.size(); i++){ //for every block
        Block *curBlock = Blocks[i];
        int j;
        for(j=0; j<curBlock->tNodes; j++){  //traverse the childBlocks, print values and save all the childBlocks
            if(num == curBlock->value[j])
                tmp++;
            if(curBlock->childBlock[j]!=NULL)
                newBlocks.push_back(curBlock->childBlock[j]);
        }
        if(curBlock->value[j] == INT_MAX && curBlock->childBlock[j]!=NULL)
            newBlocks.push_back(curBlock->childBlock[j]);

    }

    if(newBlocks.size() == 0){ //if there is no childBlock block left to send out then just the end of the recursion
        Blocks.clear();
        //exit(0);
    }
    else {                    //else send the childBlocks to the recursion to continue to the more depth
        Blocks.clear();
        count_number(newBlocks, num,tmp);
    }
    cout << tmp << endl;
}

void range_number(vector < Block* > Blocks, int x1, int x2, int tmp){
    vector < Block* > newBlocks;
    for(int i=0; i<Blocks.size(); i++){ //for every block
        Block *curBlock = Blocks[i];
        int j;
        for(j=0; j<curBlock->tNodes; j++){  //traverse the childBlocks, print values and save all the childBlocks
            if(curBlock->value[j]>=x1 && curBlock->value[j]<=x2)
                tmp++;
            if(curBlock->childBlock[j]!=NULL)
                newBlocks.push_back(curBlock->childBlock[j]);
        }
        if(curBlock->value[j] == INT_MAX && curBlock->childBlock[j]!=NULL)
            newBlocks.push_back(curBlock->childBlock[j]);
    }

    if(newBlocks.size() == 0){ //if there is no childBlock block left to send out then just the end of the recursion
        Blocks.clear();
        //exit(0);
    }
    else {                    //else send the childBlocks to the recursion to continue to the more depth
        Blocks.clear();
        range_number(newBlocks, x1, x2, tmp);
    }
    cout << tmp << endl;
}

int main(int argc, char const *argv[]){
    int num;
    vector < Block* > Blocks;
    int ch;
    int i = 0;
    int totalValues = 0;
    if(argc != 4)
        cout <<"Insufficient input arguments\n";
    else{
        FILE *p;
        p = fopen(argv[1], "r");
        M = atoi(argv[2]);
        B = atoi(argv[3]);
        numberOfPointers = int((B-8)/12)+1;
        vector<int> v;
        //input from file
        char s[10];
        while(fscanf(p, "%s", s)!=EOF){

            if(!strcmp(s,"INSERT")){
                fscanf(p, "%d", &num);
                //printf("**** Insert %d ****\n\n", num);
                v.push_back(num);
                insertNode(rootBlock, num);
                totalValues++;

            }else if(!strcmp(s,"FIND")){
                Blocks.clear();
                Blocks.push_back(rootBlock);
                //print(Blocks);
                fscanf(p, "%d", &num);
                if ( find(v.begin(), v.end(), num) != v.end() )
                    printf("YES\n");
                else
                    printf("NO\n");

            }else if(!strcmp(s,"COUNT")) {
                int val;
                int count=0;
                fscanf(p, "%d", &val);
                Blocks.clear();
                Blocks.push_back(rootBlock);
                count_number(Blocks, val, count);
            }else if(!strcmp(s,"RANGE")) {
                int val1, val2;
                int count=0;
                fscanf(p, "%d %d", &val1, &val2);
                Blocks.clear();
                Blocks.push_back(rootBlock);
                range_number(Blocks, val1, val2, count);
            } else if(!strcmp(s,"EXIT")){
                exit(0);
            } else {
                printf("Invalid Option\n");
            }
            char s[10];
        }
    }
    return 0;
}
