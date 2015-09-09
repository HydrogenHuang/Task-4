#include <iostream>
#include <stdlib.h>
#include <string>
#include "huffman.h" 

using namespace std;
bool menu(int& x);

int main()
{
	int x;
	string sourceFile,targetFile;
	huffman_tree htree; ////
	
	while(menu(x))
	{
		switch(x){
			case 1:
				cout<<"\n 请输入待压缩的文件名：";
				cin>>sourceFile;
				cout<<" 请输入目标文件名：";
				cin>>targetFile;
				htree.encode(sourceFile,targetFile);
				break;
			case 2:
				cout<<"\n 请输入待解缩的文件名：";
				cin>>sourceFile;
				cout<<" 请输入目标文件名：";
				cin>>targetFile;
				htree.decode(sourceFile,targetFile);
				break;
		}
		system("pause");
	}
	
	return 0;
}

bool menu(int& x)
{
	system("cls");
	cout<<"\n请选择操作:：\n";
	cout<<" -----------------------\n";
	cout<<"|     1、压缩文本\t|\n";
	cout<<"|     2、解压文本\t|\n";
	cout<<"|     0、退出    \t|\n";
	cout<<" -----------------------\n";
	cout<<"\n 请输入编号：";
	cin>>x;
	return x>0?true:false; 
}
