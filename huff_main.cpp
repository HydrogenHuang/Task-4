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
				cout<<"\n �������ѹ�����ļ�����";
				cin>>sourceFile;
				cout<<" ������Ŀ���ļ�����";
				cin>>targetFile;
				htree.encode(sourceFile,targetFile);
				break;
			case 2:
				cout<<"\n ��������������ļ�����";
				cin>>sourceFile;
				cout<<" ������Ŀ���ļ�����";
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
	cout<<"\n��ѡ�����:��\n";
	cout<<" -----------------------\n";
	cout<<"|     1��ѹ���ı�\t|\n";
	cout<<"|     2����ѹ�ı�\t|\n";
	cout<<"|     0���˳�    \t|\n";
	cout<<" -----------------------\n";
	cout<<"\n �������ţ�";
	cin>>x;
	return x>0?true:false; 
}
