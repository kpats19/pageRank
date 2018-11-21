/* KEYUR PATEL cs610 6703 prp */
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
struct storage{
	int size;
	float current;
	float previous;
	int outDegree;
	int inDegree;
	int *listOFedge;
};

void setDegreeAndInitialValue_6703(struct storage ver[],float numberOfVertex,int initialValue){

	int row,col;
	for(row=0;row<numberOfVertex;row++){
		if (initialValue==0)	ver[row].previous =0;
		else if(initialValue==1) ver[row].previous = 1;
		else if (initialValue== (-1))ver[row].previous = (1/numberOfVertex); 	
		else if (initialValue==(-2))ver[row].previous = (1/sqrt(numberOfVertex));
	}
}

void main(int argc, char *argv[]){

	int iteration,initialValue,vertex_1,vertex_2,n,m,k,i,j,node_count_errorrate=0;
	float dValue=0.85,secondterm=0,errorrate=0;
       	
	if (argc!=4){
		printf("\nPlease run the code i.e. ""./pgrk iterations initialvalue filename\n");
		exit(0);
	}
	iteration = atoi(argv[1]);	
	if (iteration==0&&strlen(argv[1])>1){
	
	printf("please enter the valid argument,instead:\t%s\n",argv[1]);
	exit(0);
	}else if (strlen(argv[1])==1&&iteration==0){
	char check[2];
	strcpy(check,argv[1]);
	int checker=check[0]-'\0';
	if (checker!=48){
	printf("please enter the valid argument, instead:\t%s\n",check);
	exit(0);
		}
	}
	else  {
		int check;
		int len = strlen(argv[1]);
		char  str[len];
		strcpy(str,argv[1]);
		for (i=0;i<len;i++)
		{	check= str[i]-'\0';
			if ((check<48 || check > 57)&& check!=45) {
			
				printf("please enter valid argument, instead:\t%s\n",argv[1]);
				exit(0);
			}
		}

	}
	
	
	
	initialValue =atoi(argv[2]);
  	if (initialValue==0&&strlen(argv[2])>1){
	printf("please enter the valid argument, instead:\t%s\n",argv[2]);
	exit(0);
	}else if (strlen(argv[2])==1&&initialValue==0){
		char check[2];
		strcpy(check,argv[2]);
		int checker=check[0]-'\0';
		if (checker!=48){
		printf("please enter the valid argument, instead:\t%s\n",check);
		exit(0);
		}
	}
  	else {
		int check;
		int len = strlen(argv[2]);
		char  str[len];
		strcpy(str,argv[2]);
		for (i=0;i<len;i++)
		{	check= str[i]-'\0';
			if ((check<48 || check > 57)&& check!=45) {
			
				printf("please enter valid argument, instead:\t%s\n",argv[2]);
				exit(0);
			}
		}

	}	
	if (-2>initialValue ||1<initialValue){
		printf("Can not have such initialvalue can only use: -2,-1,0,1");
	exit(0);
}
	printf("\niteration:%d\nintitialValue:%d\nFile path:%s\n",iteration,initialValue,argv[3]);
	
		if(iteration<1){
		if (iteration==0) errorrate=0.00001;
		else errorrate= (1/pow(10,(-1)*iteration));
		iteration=1;
		}
	FILE *openfile=fopen(argv[3],"rb");
	fscanf(openfile,"%d %d",&n,&m);
	
	struct storage *vertexDetail= (struct storage *)malloc(sizeof (struct storage)*n);	
	for(i=0;i<n;i++){
	vertexDetail[i].outDegree=0;
	vertexDetail[i].size=0;
	}
 
	for(i=0;i<m;i++){
	fscanf(openfile,"%d %d",&vertex_1,&vertex_2);
	vertexDetail[vertex_1].outDegree=vertexDetail[vertex_1].outDegree+1;
	vertexDetail[vertex_2].inDegree=vertexDetail[vertex_2].inDegree+1;
	}
	
	for (i=0;i<n;i++)
	vertexDetail[i].listOFedge = (int *) malloc(sizeof(int) * vertexDetail[i].inDegree);

	if( fseek(openfile ,0L, SEEK_SET) != 0 ) {
	printf("could not rewind the file SORRY ");
	exit(0);
	}
	fscanf(openfile,"%d %d",&vertex_1,&vertex_2);

	for(i=0;i<m;i++){
	fscanf(openfile,"%d %d",&vertex_1,&vertex_2);
	vertexDetail[vertex_2].listOFedge[vertexDetail[vertex_2].size]=vertex_1;
	vertexDetail[vertex_2].size++;
	}
	
	for (k=0;k<=iteration;k++){
	
		if(errorrate!=0) iteration++;
		if (k==0){
			if (n>10){
				float	temp = (1/(float)n); 
				errorrate= 0.00001;
				printf("\nN>10 N:%d \t Defualt Errorate (iteration=0): 0.00001 Initialvalue : %0.7f\n\n",n,temp);
				setDegreeAndInitialValue_6703(vertexDetail,n,-1);
			}
			else setDegreeAndInitialValue_6703(vertexDetail,n,initialValue);
			if ((n<=10))	printf("Base: 0 :");
			for (i=0;i<n;i++){
			if ((n<=10))	printf("P[%d]=%.7f\t",i,vertexDetail[i].previous);	}
		}			
		else{
			if((n<=10))printf("\nIter: %d :",k);
			for(j=0;j<n;j++){
				secondterm=0; 
				for(i=0;i<vertexDetail[j].size;i++)
					secondterm=secondterm+(vertexDetail[vertexDetail[j].listOFedge[i]].previous/vertexDetail[vertexDetail[j].listOFedge[i]].outDegree);
				vertexDetail[j].current = ((1-dValue)/n) + dValue*secondterm;
				if((n<=10))	printf ("P[%d]%.7f\t",j,vertexDetail[j].current);
			}	
			for (i=0;i<n;i++){
				if ((vertexDetail[i].previous-vertexDetail[i].current)<errorrate)node_count_errorrate++; 
				vertexDetail[i].previous= vertexDetail[i].current;
			}	
			if (node_count_errorrate==n&&errorrate!=0){
				if (n>10)printf("Iteration:%d\nP[0]=%0.7f\nP[1]=%0.7f\nP[2]=%0.7f\nP[3]=%0.7f ...other Vertices omitted\n",k,vertexDetail[0].current,vertexDetail[1].current,vertexDetail[2].current,vertexDetail[3].current);
				break;}
			node_count_errorrate=0;
		}
	}
}
