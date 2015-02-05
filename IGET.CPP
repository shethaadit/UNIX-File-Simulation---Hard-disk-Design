
#include<stdio.h>
#include<conio.h>
#include<fstream.h>
#include<stdlib.h>
#include<iostream.h>
#include<string.h>
#include<dos.h>


FILE *free_inode_list,*file_system_table,*alloc_inode_list_ram,*temp;

int file_no=50,incore_inode,isfree,free_incore=20,is_incore_free[20];
char *s,name[25];
char *fn="file";

int flag=0,choice,avail,count;
char search_filename[10];
int islock[1201],ref_count[1201];
void bmap();
void create_file();
void opening_file();
void delete_file();
void ls();
void ls1();
void get_choice();
void close_file();

struct free_inode_list
{
 int free_inode_no;
 int type;

}free_inode;


struct incore_list
{
 int  incore_no;
 int  inode_no;
 char file_name[25];
 int  refcnt;

}incore;


struct inode_bmap
{
 int direct_block[5];
}block_map;

struct file_system_table_list
{

int inode_no;
char file_name[25];
//int type;
}file_system;


void main()
{

int i;
clrscr();
free_inode_list=fopen("free_inode_list.txt","wb");
free_inode.type=0;
file_system_table=fopen("file_system_table.txt","rb");

alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","wb");
fclose(alloc_inode_list_ram);
temp=fopen("temp.txt","wb");
fclose(temp);


for(i=1001;i<=1200;i++)
{
  	islock[i]=0;
}

while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
{
  	islock[file_system.inode_no]=1;
}

for(i=1001;i<=1200;i++)
{
  	if(islock[i]==0)
  	{
		free_inode.free_inode_no=i;
      		fwrite(&free_inode,sizeof(struct free_inode_list),1,free_inode_list);
  	}
}

for(i=1001;i<=1200;i++)
{
  	ref_count[i]=0;
}

for(i=1;i<=20;i++)
{
  	is_incore_free[i]=1;
}


fclose(free_inode_list);

get_choice();

getch();

}

void get_choice()
{
	clrscr();
	printf("\nEnter your choice : \n\n 1. Create new file\n 2. Open the file\n 3. Close the file\n 4. Delete the file\n 5. List of created files (ls -l)\n 6. List of inodes on main memory(RAM)\n\n");
	char choice= getche();

	switch(choice)
	{
	 case '1':
		create_file();
		break;
	 case '2':
		opening_file();
	 	break;
 		case '3': close_file();
	   	break;
	 case '4':
	   	delete_file();
	   	break;

 	case '5':
	 	ls();
	 	break;
 	case '6':
	 	ls1();
	 	break;
 		case '7':
	 	bmap();
	 break;
 default:
	 get_choice();
	 break;

}


}


void create_file()
{
 printf("\nenter the file name to be created: " );
 gets(search_filename);
  file_system_table=fopen("file_system_table.txt","rb");

while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
{
 if(strcmp(file_system.file_name,search_filename)==0)
 {
  printf("\n\n==> File %s already exist give different name..",search_filename);
	fclose(file_system_table);

  get_choice();
}

}

 for(int i=1001;i<=1200;i++)     //searching in to free inode list
 {
    if(islock[i]==0)
    {
       islock[i]=1; //lock the inode
       file_system_table=fopen("file_system_table.txt","ab");
       file_system.inode_no=i;
       strcpy(file_system.file_name,search_filename);
       fwrite(&file_system,sizeof(struct file_system_table_list),1,file_system_table);
       isfree=1;
       printf("\n\n==> File created successfully..");
       printf("\n\n==> Assigned inode no is : %d ", i);

       fclose(file_system_table);
       break;

    }
}
    if(isfree==0)
    {
     printf("\n\n Sorry!!! \n no more inode is free in free inode list ");
     printf("\t can't create new file \n \t wait till one of the inode gets free");

    }
 printf("\n==> Wants to continue??? ");

char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}



 }


void delete_file()
{
 int cp;
 int track;
 track=0;
 cp=0;
 printf("\n\nenter the file name to be closed : " );
 gets(search_filename);


//  file_system_table=fopen("file_system_table.txt","rb");

  file_system_table=fopen("file_system_table.txt","rb");
//  temp1=fopen("alloc_inode_list_ram1.txt","wb");

while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
{
    if(strcmp(file_system.file_name,search_filename)==0)
    {
	if(ref_count[file_system.inode_no]>0)
	{
		printf("\n\n File currently in use....try deleting later ");
		break;
	}
	islock[file_system.inode_no]=0;

       //here


	  file_system_table=fopen("file_system_table.txt","rb");
	  temp=fopen("temp.txt","wb");

	   while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
	   {
	      if(strcmp(file_system.file_name,search_filename)==0)
	      {
			continue;
	      }
	      fwrite(&file_system,sizeof(struct file_system_table_list),1,temp);
	   }

	     fclose(file_system_table);
	     fclose(temp);
	   file_system_table=fopen("file_system_table.txt","wb");
	   temp=fopen("temp.txt","rb");
	   while(fread(&file_system,sizeof(struct file_system_table_list),1,temp))
	   {
	      fwrite(&file_system,sizeof(struct file_system_table_list),1,file_system_table);
	   }
	     fclose(file_system_table);
	     fclose(temp);

      printf("\n\n File deleted successfully.... ");

       break;

    }

}


fclose(file_system_table);
fclose(temp);

printf("\n\n==> Wants to continue??? ");
char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}



}






void opening_file()
{
  int i;
  printf("\n\nenter the file name to be open : " );
  gets(search_filename);

  if(free_incore<=0)
      goto t;
  file_system_table=fopen("file_system_table.txt","rb");
  avail=0;



while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
{

  if(strcmp(file_system.file_name,search_filename)==0)
  {

       printf("\nPlease wait..\nSearching for inode into the cache.... " );
       delay(2000);



    ref_count[file_system.inode_no]+=1;
    if(ref_count[file_system.inode_no]>1)
    {

	alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","r+b");
	int track=0;
	printf("\n\nFile available in cache...reference count incremented");
	avail=1;

       while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
       {
	    if(strcmp(incore.file_name,file_system.file_name)==0)
	    {

///		    delay(2000);
//		    printf("gyyyy");


		    fseek(alloc_inode_list_ram,track*sizeof(struct incore_list) , 0);
		    incore.refcnt=ref_count[file_system.inode_no];

		    fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);

		    fclose(alloc_inode_list_ram);
		    fclose(file_system_table);
		    break;
	    }
	    track++;
       }
	    break;

    }

       printf("\n\nRespective inode not available in the cache.... " );


       printf("\nPlease wait..\nSearching for inode into the file system table.... " );
       delay(2000);



    printf("\n==> file opened successfully ");
    printf("\n\n==> file descriptor(disk inode) for file %s is : %d",file_system.file_name,file_system.inode_no);
    printf("\n\nWait Loading file into main memory...");
    delay(2000);
    printf("\n\nAllocating incore inode...");


    free_incore-=1;

    for(i=1;i<=20;i++)
    {
       if(is_incore_free[i]==1)
       {
	  incore_inode=i;
	  is_incore_free[i]=0;
	  break;
       }

     }


    printf("\n\n==> In core inode for file %s is : %d",file_system.file_name,incore_inode);

    alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","ab");
    incore.incore_no=incore_inode;
    incore.inode_no=file_system.inode_no;

    strcpy(incore.file_name,file_system.file_name);
    incore.refcnt=ref_count[file_system.inode_no];

    fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);

    fclose(alloc_inode_list_ram);

    avail=1;
    fclose(file_system_table);
    break;

    t:
    printf("\n\nRam memory full....can't assign incore inodes");

}

}

if(avail==0)
{
 printf("\n\n==> File %s doesn't exist ", search_filename);

}
 printf("\n==> Wants to continue??? ");

char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}


}



void close_file()
{
 int cp;
 int track;
 track=0;
 cp=0;
 printf("\n\nenter the file name to be closed : " );
 gets(search_filename);




  alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","rb");

while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
{
    if(strcmp(incore.file_name,search_filename)==0)
    {
	ref_count[incore.inode_no]-=1;

	if(ref_count[incore.inode_no]>0)
	{

	   fclose(alloc_inode_list_ram);
  //	   WinExec("RealPlayer.exe",SW_SHOWNORMAL);
	   alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","r+b");
	   while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
	   {
	      if(strcmp(incore.file_name,search_filename)==0)
	      {
		    fseek(alloc_inode_list_ram,track*sizeof(struct incore_list) , 0);
		    incore.refcnt=ref_count[incore.inode_no];

		    fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);

		    fclose(alloc_inode_list_ram);
		  //  fclose(file_system_table);
		    break;
	      }
	       track++;
	   }

	   break;
	}

       //here
	      free_incore+=1;
	  is_incore_free[incore.incore_no]=1;
	   alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","rb");
	   temp=fopen("temp.txt","wb");

	 /* cp=0;
	   track=0;
	   while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
	   {
	      if(strcmp(incore.file_name,incore.file_name)==0)
	      {

		    cp=1;
		    printf("dgdgd");
		    delay(2000);


		    //fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);

	      }

	      if(cp==1)
	      {

		     //	    printf("dgdgd");
		    //  printf("   %d",track);
		   // delay(2000);

		 fseek(temp,(track+1)*sizeof(struct incore_list),0);

		 fread(&incore,sizeof(struct incore_list),1,temp);

		 fseek(alloc_inode_list_ram,track*sizeof(struct incore_list) , 0);
		 fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);
	      }
		 track++;
	   } */

	   while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
	   {
	      if(strcmp(incore.file_name,search_filename)==0)
	      {
			continue;
	      }
	      fwrite(&incore,sizeof(struct incore_list),1,temp);
	   }

	     fclose(alloc_inode_list_ram);
	     fclose(temp);
	   alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","wb");
	   temp=fopen("temp.txt","rb");
	   while(fread(&incore,sizeof(struct incore_list),1,temp))
	   {
	      fwrite(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram);
	   }
	     fclose(alloc_inode_list_ram);
	     fclose(temp);


       break;

    }




}


fclose(alloc_inode_list_ram);
fclose(temp);


 printf("\n\n==> File %s closed successfully  ", search_filename);
 printf("\n==> Wants to continue??? ");

char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}


}


void ls()
{
file_system_table=fopen("file_system_table.txt","rb");
printf("\n\nInode no\t File Name\n");
printf("--------\t ---------\n");
//printf("\n200 inode has been created successfully: \n\n" );

while(fread(&file_system,sizeof(struct file_system_table_list),1,file_system_table))
{
  printf("\n%d",file_system.inode_no);
  printf("\t\t%s",file_system.file_name);
  //printf("\t%d",file_system.type);

}
 fclose(file_system_table);
 printf("\n\n==> Wants to continue??? ");
char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}



}








void ls1()
{

alloc_inode_list_ram=fopen("alloc_inode_list_ram.txt","rb");
printf("\n\nIncore no\tInode no\t File Name\tRef cnt\n\n");
printf("--------\t ---------\t--------\t--------\n");
//printf("\n200 inode has been created successfully: \n\n" );

while(fread(&incore,sizeof(struct incore_list),1,alloc_inode_list_ram))
{
  printf("%d",incore.incore_no);
  printf("\t\t%d",incore.inode_no);

  printf("\t\t%s",incore.file_name);
  printf("\t\t%d\n",incore.refcnt);
  //printf("\t%d",file_system.type);

}
 fclose(alloc_inode_list_ram);
 printf("\n\nFree incore inodes: %d",free_incore);
 printf("\n\n==> Wants to continue??? ");
char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();
}



}


/*void bmap(int INODE_NO)
{

   unsigned long int bs,as,off,i,j,k,qu,rem,offset,tem,block,tri,t=10;
   double qu1;
   int gdriver =  DETECT, gmode, errorcode;  initgraph(&gdriver, &gmode, "D://TC//BGI");
//   cleardevice();
//   setcolor(RED);
//   for(i=1;i<=13;i++)
//   {
//      rectangle(10,10+25*i,100,30+25*i);
//   }

   printf("\n\nEnter the block size (in bytes)= ");
   scanf("%ld",&bs);
   printf("\nEnter the size of address (in bytes)= ");
   scanf("%ld",&as);

      i=bs/as;
   printf("\nNo. of addresses contained within each block= %ld",i);

   printf("\nEnter the offset you are looking for= ");
  // scanf("%ld",&off);
   //scanf("%f",&qu1);
   cin>>qu1;
   //printf("\nOffset in that block: %f",qu1);
   cout<<"\n\n"<<qu1;

   //block no. starting from zero!!!!!!
   rem=off%bs;

   offset=rem;
//  cout<<"\n\n"<<rem;

//   printf("\nOffset in that block: %ld",rem);

   qu=(off)/bs;


//   qu1=double(off);


   block=qu;    //no of blocks to be scanned


   if(qu<10)
   {
	printf("\nBlock number: %ld",qu);
	printf("\nOffset in that block: %ld",rem);
   }
   else
   {
     qu-=10;
     rem=qu%i;
     qu=qu/i;
     if(qu==0)
     {
	printf("\nSingle indirect....\n");
	printf("\nBlock number specified at : %ld",rem);
	printf("\nOffset in that block: %ld",offset);

     }
     else if(qu>0 && qu<=i-1)
     {
	printf("\nDouble indirect....\n");
	printf("\nBlock number specified at: %ld -> %ld ",qu-1,rem);
	printf("\nOffset in that block: %ld",offset);
     }
     else// if(qu>=i && qu<=(i-1)*(i-1))
     {

	printf("\nTriple indirect....\n");
	qu=block-(t+i+(i*i));
	printf("%ld",qu);
	rem=qu%i;
	qu=qu/i;

	tri=qu/i;     //here always tri=0;
/*	if(qu>i-1 && qu<i*i)
	{
	   printf("hello");
	   tri=qu/i;
	   qu=qu-tri*256;            //67381248
//	   rem=qu%256;
	}
	printf("\nBlock number specified at: %ld -> %ld -> %ld",tri,qu,rem);

	printf("\nOffset in that block: %ld",offset);
     }

  }


char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}


} */


void bmap()
{

   int bs,as,na,i,j,k,l,m,n,done=0;
   long double off;
   i=0;    //direct or indirect
   j=0;     //block no               j,k==== for direct
   k=0;     //offset in dat block
   printf("\n\nEnter the block size (in bytes)= ");
   scanf("%d",&bs);
   printf("\nEnter the size of address (in bytes)= ");
   scanf("%d",&as);

   na=bs/as;
   printf("\nNo. of addresses contained within each block= %d",na);


   printf("\nEnter the offset you are looking for= ");
   cin>>off;
   while(off!=0)
   {
       off--;
       k++;
       if(k==bs)
       {
	   k=0;
	   j++;
       }
       if(j==10)
       {
	  i++;
	  k=0;
	  j=0;
	  break;
       }
   }
   if(i==0)
   {
	printf("\nDirect address");
	cout<<"\nBlock number specified at: "<<j;

	cout<<"\nOffset in that block: "<<k;
   }
   if(i==1)

   {

       for(j=0;j<na;j++)
       {
	   for(k=0;k<bs;k++)
	   {
	       if(off==0)
	       {
		 done=1;
		 break;
	       }
	      off--;
	   }
	   if(off==0 && done==1)
	   {
	      break;
	   }
       }
       if(off==0 && done==1)
       {
	printf("\nSingle Indirect address");
	printf("\nBlock number specified at : %d ",j);

	cout<<"\nOffset in that block: "<<k;
       }
       else
       {
	  i++;
       }
   }




   if(i==2)

   {

       for(j=0;j<na;j++)
       {
	   for(k=0;k<na;k++)
	   {
	      for(l=0;l<bs;l++)
	      {
	       if(off==0)
	       {
		 done=1;
		 break;
	       }
		 off--;
	      }
	      if(off==0 && done==1)
		 break;

	   }
	   if(off==0 && done==1)
	      break;
       }
       if(off==0 && done==1)
       {
	printf("\nDouble Indirect address");
	printf("\nBlock number specified at : %d -> %d",j,k);

	cout<<"\nOffset in that block: "<<l;
       }
       else
       {
	  i++;
       }
   }




   if(i==3)
   {

       //cout<<"dgdgd";
       for(j=0;j<na;j++)
       {
	   for(k=0;k<na;k++)
	   {
	      for(l=0;l<na;l++)
	      {
		 for(m=0;m<bs;m++)
		 {
		    if(off==0)
		    {
		       done=1;
		       break;
		    }
		     off--;
		 }
			      if(off==0 && done==1)
				break;

	      }
	      if(off==0 && done==1)
		 break;

	   }
	   if(off==0 && done==1)
	      break;
       }
       if(off==0)
       {
	printf("\nTriple Indirect address");
	printf("\nBlock number specified at : %d -> %d -> %d",j,k,l);

	cout<<"\nOffset in that block: "<<m;
       }
   }
printf("\n\n==> Wants to continue??? ");
char ch=getche();
if(ch=='y' || ch=='Y')
{
  get_choice();

}


}


