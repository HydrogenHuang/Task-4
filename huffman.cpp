#include "huffman.h"

huff_node::huff_node()
{
	letter='\0';
	weight = 0;
	parent = false;
	lchild = -1;
	rchild = -1;
}

huff_node::huff_node(char l,int w)
{
	letter=l;
	weight = w;
	parent = -1;
	lchild = -1;
	rchild = -1;
}

bool huff_node::isleaf()
{
	return (lchild==-1)&&(rchild==-1);
}

bool huff_node::operator >(const huff_node& b)
{
	return weight>b.weight;
}

huffman_tree::huffman_tree()
{
	source_length = 0;
	target_length = 0;
	node_size = 0;
	size = 0;
	for(int i=0;i<oo/2;i++)
	char_code[i]="";
}

void huffman_tree::precode(FILE* infile)
{
	unsigned char x;
	map<char,int> num_char;
	for(int i=0;i<oo;i++)
		num_char[i] = 0;
	while(!feof(infile)) //record all the charaters and its weight
	{
		x = fgetc(infile);///////////////////get a -1 at the last!!!!!!!
		if(x==255) break;
		//cout<<long(x)<<endl;//test
		source_length++;
		num_char[x]++;
	}
	cout<<endl<<"原文本长度："<<source_length<<endl;//test
	
	for(int i=0;i<oo;i++) //insert all the charaters into the nodes
	{
		if(num_char[i]!=0)
		{
			//cout<<i<<' '<<num_char[i]<<endl;//test
			node[size].letter = i;
			node[size].weight = num_char[i];
			size++;
		}
	}
	//cout<<"size:"<<size<<endl;//test
	//system("pause");//test
	buildtree();
	
	//cout<<"node_size:"<<node_size<<endl;//test
	string code="";
	coding(node_size-1,code);
}

void huffman_tree::buildtree()
{
	huff_node x1,x2;
	int pos = 0;
	sort();
	node_size = size;
	while(pos<node_size-1)
	{
		x1 = node[pos];
		x2 = node[pos+1];
		node[node_size].weight = x1.weight+x2.weight;
		node[node_size].lchild = pos;
		node[node_size].rchild = pos+1;
		node_size++;///
		
		x1.parent = true;
		x2.parent = true;
		pos += 2;
		update();
	}
}

void huffman_tree::sort()
{
	huff_node temp;
	bool needNext = true;
	for(int i=1;i<size;i++)
	{
		needNext = false;
		for(int j=0;j<size-i;j++)
		{
			if(node[j]>node[j+1])
			{
				temp = node[j];
				node[j] = node[j+1];
				node[j+1] = temp;
				needNext = true;
			}
		}
	}
}

void huffman_tree::update()
{
	int x=node_size-1;
	huff_node temp;
	while(node[x-1]>node[x])
	{
		temp = node[x-1];
		node[x-1] = node[x];
		node[x] = temp;
		x--;
	}
}

void huffman_tree::coding(int loc,string code)
{
	if(node[loc].isleaf())
	{
		char_code[node[loc].letter]=code;
		//cout<<int(node[loc].letter)<<' '<<node[loc].letter<<' '<<code<<endl;//test
		return;
	}
	coding(node[loc].lchild,code+"0");
	coding(node[loc].rchild,code+"1");
}

void huffman_tree::compact(FILE* infile,FILE* outfile)
{
	//cout<<"source_length:"<<source_length<<endl;//test
	//cout<<"size:"<<size<<endl;//test
	fwrite(&source_length,sizeof(unsigned long),1,outfile);//length of the file
	target_length++;
	fwrite(&size,sizeof(int),1,outfile);//the number of all the charaters
	target_length++;
	
	unsigned char x;
	int len=0;
	string temp;
	for(int i=0;i<oo/2;i++)
	{
		temp = char_code[i];  
		len = temp.length();//最长编码可能有20位！！！！！！
		if(len>0)
		{
			fwrite(&i,1,1,outfile);
			fwrite(&len,1,1,outfile);
			target_length += 2;
			//cout<<i<<' '<<len<<' '<<temp<<endl;//test
			
			int m;
			if(len%8!=0) m = len/8+1;
			else m = len/8;
			for(int ii=0;ii<m;ii++)
			{
				x=0;
				for(int jj=0;(jj<8)&&(jj<len);jj++)
				{
					x = x<<1;
					if(temp[jj+ii*8]=='1')//超过len值后，后面的数值没意义但会有隐患！！ 
					x = x+1;
				}
				fwrite(&x,1,1,outfile);
				target_length++;
				len -= 8;
			}
		}
	}
	//cout<<"target_length:"<<target_length<<endl;//test
	
	int l=0;//length of c
	unsigned char c=0;//to record the compacted message
	int ii=0;//position of code
	while(!feof(infile)) //record all the charaters and its weight
	{
		ii = 0;
		x = fgetc(infile);//// be careful of the EOF at last!!!
		if(x==255) break;
		temp = char_code[x];
		len = temp.length();
		//cout<<x<<' '<<temp<<endl;//test
		
		do{
			c = c<<1;
			if(temp[ii]=='1')
				c = c+1;
			ii++;
			l++;
			if(l==8)
			{
				fwrite(&c,1,1,outfile);
				target_length++;
				//cout<<target_length<<endl;//test
				l = 0;
				c = 0;
			}
		}while(ii<len);
	}
	if((l>0)&&(l<8)) //last word
	{
		//cout<<l<<' ';//test
		while(l<8)
		{
			c = c<<1;
			l++;
		}
		fwrite(&c,1,1,outfile);
		target_length++;
		//cout<<"||||||||||"<<endl;//test
	}
	cout<<"压缩后文本长度："<<target_length<<endl;
	double div = double(target_length)/double(source_length);
	cout<<endl<<"文件的压缩率为："<<(div*100)<<"%\n\n";
}

void huffman_tree::encode(string sourcefile,string targetfile)
{
	//cout<<"3"<<endl;//test 3
	FILE* infile,*outfile;
	infile = fopen(sourcefile.c_str(),"rb");
	if(infile==NULL)
	{
		cout<<endl<<"待压缩文件名无效！！请重新输入..."<<endl;
		return; 
	}
	outfile = fopen(targetfile.c_str(),"wb");
	if(outfile==NULL)
	{
		cout<<endl<<"目标文件名无效！！请重新输入..."<<endl;
		return; 
	}
	//cout<<"1"<<endl;//test 1
	precode(infile);
	fclose(infile);
	infile = fopen(sourcefile.c_str(),"rb");
	compact(infile,outfile);
	
	fclose(infile);
	fclose(outfile);
	canceal();
}

void huffman_tree::decode(string sourcefile,string targetfile)
{
	FILE* infile,*outfile;
	infile = fopen(sourcefile.c_str(),"rb");
	if(infile==NULL)
	{
		cout<<"待解压缩文件名无效！！请重新输入..."<<endl;
		return; 
	}
	outfile = fopen(targetfile.c_str(),"wb");
	if(outfile==NULL)
	{
		cout<<"目标文件名无效！！请重新输入..."<<endl;
		return; 
	}
	
	int x=0;
	int len=0;
	unsigned long file_len=0;
	unsigned char p;
	
	string com_temp,temp;
	map<string,char> code_char;
	map<string,char>::iterator l_it;
	
	fread(&source_length,sizeof(unsigned long),1,infile);
	//cout<<"source_length:"<<source_length<<endl;//test
	fread(&size,sizeof(int),1,infile);
	//cout<<"size:"<<size<<endl;//test
	target_length += 2;
	
	for(int i=0;i<size;i++)
	{
		com_temp = "";
		fread(&x,1,1,infile);
		fread(&len,1,1,infile);
		target_length += 2;
		int l=0;
		for(int ii=0;ii<len/8;ii++)//when len is 8*n at less loop for one time
		{
			temp = "";
			fread(&p,1,1,infile);
			target_length++;
			temp = uits(p,8);
			com_temp = com_temp+temp;
		}
		if(len%8!=0)
		{
			fread(&p,1,1,infile);
			target_length++;
			temp = uits(p,len%8);
			com_temp = com_temp+temp;
		}
		
		//cout<<x<<' '<<len<<' '<<com_temp<<endl;//test
		code_char[com_temp] = x;// Be careful when make change!!!!
	}
	
	temp = "";
	while(!feof(infile)) //record all the charaters and its weight
	{
		p = fgetc(infile);
		target_length++;
		com_temp = uits(p,8);
		for(int i=0;i<8;i++)
		{
			temp = temp+com_temp[i];
			l_it = code_char.find(temp);
			if(l_it==code_char.end()) continue;
			else
			{
				fwrite(&(l_it->second),1,1,outfile);
				//cout<<temp<<' '<<long(l_it->second)<<endl;//test
				file_len++;
				//cout<<"file_len:"<<file_len<<endl;//test
				//system("pause");//test
				if(file_len==source_length)
				break;
				temp = "";
			}
		}
	}
	
	target_length--; //不知为何会多一个？？？？？ 
	cout<<endl<<"原文本长度："<<target_length<<endl;
	cout<<"解压后文本长度："<<source_length<<endl; 
	cout<<endl<<"解压缩成功！！"<<endl<<endl;
	canceal();
	fclose(infile);
	fclose(outfile);
}

string huffman_tree::uits(long f,int len)
{
	string temp = "";
	for(int i=0;i<len;i++)
	{
		if(f%2==1)
		temp = "1"+temp;
		else
		temp = "0"+temp;
		f = f/2;
	}
	return temp;
}

void huffman_tree::canceal()
{
	for(int i=0;i<node_size;i++)
	{
		node[i].letter='\0';
		node[i].weight = 0;
		node[i].parent = false;
		node[i].lchild = -1;
		node[i].rchild = -1;
	}
	source_length=0;
	target_length=0;
	node_size = 0;
	size = 0;
	char_code.clear();
}

huffman_tree::~huffman_tree()
{
	for(int i=0;i<node_size;i++)
	{
		node[i].letter='\0';
		node[i].weight = 0;
		node[i].parent = false;
		node[i].lchild = -1;
		node[i].rchild = -1;
	}
	source_length=0;
	target_length=0;
	node_size = 0;
	size = 0;
	char_code.clear();
}
