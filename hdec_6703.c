/* KEYUR PATEL cs610 6703 prp */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
struct heap{
	struct heap *right;
	struct heap *left;
	int frequency;
	unsigned char *nodeValue;
};
struct heap* createHeapNode_6703(char * nodeValue,int frequency){

	struct heap* newHeapNode= (struct heap*)malloc(sizeof (struct heap));
	if(newHeapNode==NULL){
		printf("Could not allocate memory to new node");
		exit(0);
	}
	newHeapNode->nodeValue= nodeValue;
	newHeapNode->frequency= frequency;
	newHeapNode->right=NULL;
	newHeapNode->left=NULL;
	return newHeapNode;
}
struct heapArray{
	int size;
	struct heap **Array;
};
struct heapArray* createHeapArray_6703(struct heapArray *head,int sizeOfArray){

	if(sizeOfArray==0 ){
		head  =(struct heapArray *)malloc (sizeof (struct heapArray));
		head->size=sizeOfArray+1;
		head->Array = (struct heap **) malloc (sizeof (struct heap *)*1 );
	}		
	else{ 	struct heap **tempArray ;
		tempArray =(struct heap **) realloc(head->Array,sizeof (struct heap*)*sizeOfArray+1 );
		if(tempArray==NULL){ printf("running out of memory");exit(0);}
		else {
			head->Array=tempArray;
			head->size=sizeOfArray+1;
		}
	}
	return head;
}
void exchange_6703(struct heap ** current,struct heap ** minimum){

	struct heap *temp = *current;
	*current = *minimum;
	*minimum = temp;
}
void  minDownHeap_6703( struct heapArray * head,int index){
	int current = head->Array[index]->frequency,indexOfMinimum;	
	if (2*index+1>=head->size) indexOfMinimum=index;
	else if (2*index+2>=head->size){
		int left =  head->Array[2*index+1]->frequency;
		indexOfMinimum = current<=left ?index :2*index+1;
	}	
	else {	
		int left =  head->Array[2*index+1]->frequency;
		int right= head->Array[2*index+2]->frequency;
		indexOfMinimum = ((current<=right)&&(current<=left) )? index : ((right<current)&&(right<left))? (2*index)+2 : (2*index)+1;
	}	
	if(index!=indexOfMinimum){
		exchange_6703(&head->Array[index],&head->Array[indexOfMinimum]);
		minDownHeap_6703(head,indexOfMinimum);
	}
}
struct heapArray*  buildMinHeap_6703(struct heapArray * head){

	int i=0;
	i=((head->size)-2)/2;
	for(i;i>=0;i--)	minDownHeap_6703(head,i);
	return head;
}
struct heapArray* insertIntoHeapArray_6703(struct heapArray* head,struct heap * addNode){

	if (!head)
		head=createHeapArray_6703(head,0);
	else head=createHeapArray_6703(head,head->size);	
	int size =  head->size;
	head->Array[size-1] = addNode;
	head=buildMinHeap_6703(head);
	return head;
}
struct heap * extractMin_6703 (struct heapArray *head){

	if (head->size<1)	printf("there is no such element left in heap...");
	struct heap * min= head->Array[0];
	int size= head->size;
	exchange_6703(&head->Array[0],&head->Array[size-1]);
	head->size=size-1;
	minDownHeap_6703(head,0);
	return (min);
}
int countDigit_6703(int number){

	int digit=0;
	while(number!=0){
		number=number/10;
		digit++;	}
	return digit;
}
struct heap *  Huffman_6703(struct heapArray *head){
	struct heap *root=NULL,*left=NULL,*right=NULL;	
	while(head->size>1){	
		root=createHeapNode_6703("#",0);
		right = extractMin_6703(head);
		left = extractMin_6703(head);
		root->frequency = (left->frequency) + (right->frequency);
		root->left= left;
		root->right= right;
		head=insertIntoHeapArray_6703(head,root);
	}
	return extractMin_6703(head);
}
struct storage{
	int frequancy, stop, *precode;
};
void prefixCode_6703(struct heap *root,struct storage code[],int precode[],int current){

	if (root->left){
		precode[current]=1;	
		prefixCode_6703(root->left,code,precode,current+1);
	}
	if(root->right){
		precode[current]=0;
		prefixCode_6703(root->right,code,precode,current+1);
	}
	if ((root->right==NULL)&&(root->left==NULL)){
		int ch= root->nodeValue[0],i;
		code[ch].precode = (int *) malloc(sizeof(int) * current );
		for (i=0;i<current;i++)	code[ch].precode[i] = precode[i];
		code[ch].stop=current;
		current=current-1;
	}
}
int  mergeBits_6703(int * bits){
	int i,finalValue=0;
	for (i=7;i>=0;i--)
		finalValue=finalValue|(bits[i]<<7-i);
	return finalValue;
}
void readFrequeancytable_6703(FILE *ftemp,struct storage code[]){

	int ch,frelen,i, lenght;
	char *temp,tempfrequancy[2024];
	for (i=0;i<256;i++){
		frelen=getc(ftemp);	
		temp= (char*)(&frelen);
		lenght= atoi(temp);
		if (lenght==0){
			ch=getc(ftemp);
			code[i].frequancy=0;
		}
		else{
			fgets(tempfrequancy ,lenght+1,ftemp);
			sscanf(tempfrequancy,"%d",&code[i].frequancy);
		}
	}		
}
int * getBits_6703(int n, int bits[],int bitswanted){

//	int *bits =  malloc(sizeof(int) *bitswanted*8);
	int k,y=0,mask;
	for(k=bitswanted-1;k >=0;){
		mask =  1 << k;
		int masked_n = n & mask;
		int thebit = masked_n >> k;
		bits[8-k-1] = thebit;
		k=k-1;  
	}
	return bits;
}
void decodeHuffman_6703(FILE *decodefile,struct heap *sroot,char filepath[]){
  
	struct heap *dumroot=sroot;
	int dot=0, i,ch,*decodedBits,curser=0,*temp;
	int *bits =  malloc(sizeof(int)*8);
  	if (bits==NULL){
		printf("running out of memory");
		exit(0);
	}
	int len=strlen(filepath);
	char originalfile[len];
	for (i=0;i<strlen(filepath);i++)if (filepath[i]=='.')dot=i;
	strncpy(originalfile,filepath,dot);
	originalfile[dot]='\0';
	FILE *decodedfile = fopen(originalfile,"wb");
	if (decodedfile==NULL){
		printf("Could not able to open the file");
		exit(0);
	}
	while(1){

		ch=getc(decodefile);
		if (ch==EOF)break;
    else{
			decodedBits=getBits_6703(ch,bits,8);		
			for(i=0;i<8;i++){	
				if(decodedBits[i]==1){
					dumroot=dumroot->left;	
					if (dumroot->left==NULL&&dumroot->right==NULL){
						putc(dumroot->nodeValue[0],decodedfile);
						dumroot=sroot;
						curser=i;
					}	
				}
				else {
					dumroot= dumroot->right;
					if(dumroot->right==NULL&&dumroot->left==NULL){
						putc(dumroot->nodeValue[0],decodedfile);
						dumroot=sroot;
						curser=i;
					}
				}	
				if (i==8&&(curser!=i)){
					ch=getc(decodefile);
					if(ch==EOF)break;
					decodedBits=getBits_6703(ch,bits,8);
					i=0;
				}				
			}		
		}	
	}
}
void main(int argc,char  *argv[]){

	int count,precode[2048],current=0, i=0,j,t=0;
	struct heap *root=NULL,*sroot=NULL,*newNode=NULL;
	struct heapArray *head=NULL,*rhead=NULL;
	struct storage code[256],readcode[256];
	char *temp;
	memset(&readcode,0,sizeof readcode);
	memset(&code,0,sizeof code);
	if (argc!=2){
		printf("Please run the code i.e.""/dec filename"" no other arguments are accepted..\n");
		exit(0);
	}
	FILE *ftemp=fopen(argv[1],"rb");
	if(ftemp==NULL){printf("\nCould not open file\n");exit(0);}
	readFrequeancytable_6703(ftemp,readcode);
	for (i=0;i<256;i++){
		if(readcode[i].frequancy!=0){ 
			temp= (char*) malloc(sizeof (char)*2);	
			sprintf(temp,"%c",i);
			newNode=createHeapNode_6703(temp,readcode[i].frequancy);
			rhead=insertIntoHeapArray_6703(rhead,newNode);
		}
	}	
	current=0;
	sroot=Huffman_6703(rhead);	
	prefixCode_6703(sroot,readcode,precode,current);
	decodeHuffman_6703(ftemp,sroot,argv[1]);	
	remove(argv[1]);
	printf("Done\n");

}
