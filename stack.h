struct sNode{
	int data;
	sNode *next;
};
struct Stack{
	sNode *first;
	Stack(){
		first=NULL;
	}
	bool empty(){
		return(first==NULL);
	}
	void push(int value){//day v�
		sNode *p = new sNode;//tao mot con tro p co kieu sNode
		p->data=value;//gan gia tri cua tro p = value nhap vao
		p->next=first;//tro ph�a sau n�t p cho n�t first
		first=p;//g�n p cho bien first
	}
	int pop(int &tmp){ //lay v� xo�
		if (empty())	return -1;
		sNode *p = first;//tao con tro tmp thuoc sNode v� g�n bang n�t first
		tmp=first->data; //lay gi� tri cua n�t first de g�n cho bien tmp
		first=first->next;//cap nhat node cu th�nh node sau cua node cu
		delete p;//xo� tro p;
		return 1;
	}
	int get()
	{
		if (empty()) return INT_MIN; //ooINT_MIN
		return first->data;
	}
	// giai phong bo nho
	~Stack(){
		while(first!=NULL){ //khi d�y kh�c rong
			sNode *tmp; //tao con trotmp thuoc sNode v� g�n bang n�t first
			tmp = first;
			first=first->next;//cap nhat node cu th�nh node sau cua node cux
			delete tmp;//xo� tro tmp
		}
	}
};


