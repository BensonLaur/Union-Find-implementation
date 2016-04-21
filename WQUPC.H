/* 
*		file name :ConnectSimulation.h
*		descriptioin: ConnectSimulation implemented with algorithm—— Weighted-Quick-Union Find with Path Compression (WQUPC).
*					  In a imaginary view, there is a big square which consist of N*N little square.
*					  Every little square has a state opened or not opened,separately indicated by 1 and 0 in <Array gridOpenSet>
*					  Every little square has a root,this can changes when it uinon with other little square.
*					  Having the same root with other square means they are connected to each other.
*					  
*					In the class <ConnectSimulation>,the front 3 method is mainly implementation of WQUPC
*						  int root(POINT p)						//Path Compression when searching its final root
*						  void Union(POINT a,POINT b)			//Weighted-Quick-Union decide the smaller tree hang on the bigger tree
*					 					 					     with the help of <Integer countOfNode>
*						  bool IsConnective(POINT a,POINT b)    //compare there root to see if they are on the same tree
*					  
*					The followed 5 method is mainly design to API called by window in main.cpp 
*					  void reinitialize()					// when push the button <hRestartButton>,this will be called .
*					  void setOpen(POINT p)					// set the state of little square to open or not
*					  bool isOpen(POINT p)					// 
*					  bool IsCTConective(POINT p)			// means whether the Current Point is connective with any point in Top line
*					  bool IsBTConective()					// means whether the any of square in bottom line is connectivw with any top one
*					  
*					  
*					  
*					  
*		learn more about WQUPC see: Prinston University course -- Algorithm Part I Week I:Union-Find 
*									with link "https://www.coursera.org/course/algs4partI"
*/

class ConnectSimulation
{
private:
	int N;					//正方形的行与列数
	int *gridOpenSet;		//存放该点是否开放的信息，1为开，0为闭
	int *gridRootSet;		//存放该点的根节点
	int *countOfNode;		//存放当前树的总节点数
	POINT topPoint,bottomPoint;	//将topPoint与第一行的开放节点连通，bottomPoint与第最后一行连通


	//求根节点
	int root(POINT p)
	{
		int i=p.x * N + p.y;
		while(i !=gridRootSet[i]) 
		{
			gridRootSet[i]=gridRootSet[gridRootSet[i]];//在寻根过程同时改变根
			i=gridRootSet[i];
		}
		
		return i;
	}

	//合并同根
	void Union(POINT a,POINT b){
		int i= root(a);
		int j =root(b);
		if( i!=j )
		{
			if(countOfNode[i] < countOfNode[j])
			{
				countOfNode[j]+= countOfNode[i];
				gridRootSet[i] = j;
			}
			else 
			{
				countOfNode[i] += countOfNode[j];
				gridRootSet[j] = i;
			}
		}
	}




	//是否连接
	bool IsConnective(POINT a,POINT b){
		int i=root(a);
		int j =root(b);
		if(i==j)
			return true;
		else 
			return false;
	}

public:
	//初始化N和数组
	ConnectSimulation(int n)
	{
		N=n;
		gridOpenSet = (int*)malloc(sizeof(int)*N*N);
		gridRootSet = (int*)malloc(sizeof(int)*N*N+2);//为第一行和最后一行各保留一个位置作为其根节点
		countOfNode = (int*)malloc(sizeof(int)*N*N+2);//为第一行和最后一行各保留一个位置作为其根节点

		reinitialize();
	}

	//重新初始化模拟
	void reinitialize()
	{
		memset(gridOpenSet,0,sizeof(int)*N*N);	//全设为0
		for(int i=0;i<N*N+2;i++)				//每个节点的根为自己
		{
			gridRootSet[i]=i;
			countOfNode[i]=1;
		}

		//初始化topPoint 和 bottomPoint
		topPoint.x = N;
		topPoint.y = 0;
		bottomPoint.x = N;
		bottomPoint.y = 1;
	}

	//设置点p为开放
	void setOpen(POINT p)
	{
		POINT a;
		if(gridOpenSet[p.x*N+p.y]==0)
		{
			gridOpenSet[p.x*N+p.y]=1;
			//连接4个方向上同样开放的点
			if(p.y>0)
				if(gridOpenSet[p.x*N+(p.y-1)]==1){
					a.x=p.x;
					a.y=p.y-1;
					Union(a,p);
				}
			if(p.y<N-1)
				if(gridOpenSet[p.x*N+(p.y+1)]==1){
					a.x=p.x;
					a.y=p.y+1;
					Union(a,p);
				}
			if(p.x>0)
				if(gridOpenSet[(p.x-1)*N+p.y]==1){
					a.x=p.x-1;
					a.y=p.y;
					Union(a,p);
				}
			if(p.x<N-1)
				if(gridOpenSet[(p.x+1)*N+p.y]==1){
					a.x=p.x+1;
					a.y=p.y;
					Union(a,p);
				}
			//如果该点是第一行的点，则与topPoint连通
			if(p.y==0) Union(p,topPoint);
				
			//如果该点是第最后一行的点，则与bottomPoint连通
			if(p.y==N-1) Union(p,bottomPoint);
		}
		
	}

	//当前点p是否开放
	bool isOpen(POINT p)
	{
		if(gridOpenSet[p.x*N+p.y]==0)
			return false;
		else 
			return true;
	}

	//当前点p是否与上边界相连
	bool IsCTConective(POINT p)
	{
		return IsConnective(p,topPoint);

	}

	//下边界是否与上边界相连
	bool IsBTConective()
	{
		return IsConnective(bottomPoint,topPoint);
	}

};