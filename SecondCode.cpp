#include"iostream"
#include "vector"
#include "ctime"
using namespace std;
///////////////////////
class node{
public:
	vector<int>matrix;
	int cost;
	double chance;
};
///////////////////////
int n, creatingGenNum, Population;
double mutationChance, recomChance;
int size;
int *check;
long long parent1, parent2;
vector<node>nodes;
vector<node>childs;
vector<node>sorted;
node newChild;
int passedIterations = 0;
int foundAnswer = 0;
int changed = 0;

//////////////////////
void init(){
	nodes.clear();
	childs.clear();
	for (int i = 0; i < Population; i++)
	{
		for (int i = 0; i < size; i++)
		{
			check[i] = 0;
		}
		node temp;
		int point;
		for (int j = 0; j < size; j++){
			point = rand() % size;
			while (check[point] == 1){
				point = rand() % size;
			}
			check[point] = 1;
			temp.matrix.push_back(point + 1);
		}
		nodes.push_back(temp);
	}
}
void cost(){
	int base = (n*(n*n + 1)) / 2;
	for (int i = 0; i < nodes.size(); i++)
	{
		int cost = 0;
		for (int p = 0; p < n; p++)
		{
			int rowsum = 0;
			int colsum = 0;
			for (int q = 0; q < n; q++)
			{
				rowsum += nodes[i].matrix[p*n + q];
				colsum += nodes[i].matrix[p + q*n];
			}
			cost += abs(base - rowsum);
			cost += abs(base - colsum);
		}
		int daig1 = 0;
		int daig2 = 0;
		for (int p = 0; p < n; p++){
			daig1 += nodes[i].matrix[p*n + p];
			daig2 += nodes[i].matrix[p*n + (n - p - 1)];
		}
		cost += abs(base - daig1);
		cost += abs(base - daig2);

		nodes[i].cost = cost;
		if (cost == 0){
			foundAnswer = 1;
		}
	}
}
void chance(){
	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].cost == 0)
		{
			foundAnswer = 1;
			return;
		}
	}

	for (int i = 0; i < nodes.size(); i++){
		for (int j = 0; j < nodes.size(); j++){
			if (nodes[i].cost < nodes[j].cost)
			{
				node temp;
				temp.matrix = nodes[i].matrix;
				temp.chance = nodes[i].chance;
				temp.cost = nodes[i].cost;

				nodes[i].matrix = nodes[j].matrix;
				nodes[i].cost = nodes[j].cost;
				nodes[i].chance = nodes[j].chance;

				nodes[j].chance = temp.chance;
				nodes[j].cost = temp.cost;
				nodes[j].matrix = temp.matrix;
			}
		}
	}
	int maxCost = nodes[nodes.size() - 1].cost;
	int p = 100;
	for (int i = 0; i < nodes.size(); i++)
	{
		nodes[i].chance = (double)maxCost*p / nodes[i].cost;
	}

	/*double p = 0.3;
	double pp = 1;
	nodes[0].chance = p*1000;
	int preCost = nodes[0].cost;
	for (int i = 1; i < nodes.size(); i++)
	{
	if (preCost != nodes[i].cost){
	pp = pp * (1 - p);
	nodes[i].chance = p*pp * 1000;
	preCost = nodes[i].cost;
	}
	else{
	nodes[i].chance = nodes[i - 1].chance;

	}
	}*/

}
void parentSelect(){

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].cost == 0)
		{
			foundAnswer = 1;
			return;
		}
	}

	double sum = 0;
	for (int i = 0; i < nodes.size(); i++)
	{
		sum += nodes[i].chance;
	}
	sum = sum;
	parent1 = rand() % (int)sum;
	long double lower = 0;
	long double upper = nodes[0].chance;
	for (int i = 0; i < nodes.size(); i++)
	{
		if (parent1 >= lower && parent1 <= upper){ parent1 = i; break; }
		else{ lower = upper; upper = upper + nodes[i + 1].chance + 1; }
	}

	int flag = 1;
	while (flag){
		lower = 0;
		upper = nodes[0].chance;
		parent2 = rand() % (int)sum;
		for (int i = 0; i < nodes.size(); i++){
			if (parent2 >= lower && parent2 <= upper){ parent2 = i; break; }
			else{ lower = upper; upper = upper + nodes[i + 1].chance + 1; }
		}
		if (parent2 != parent1)
		{
			flag = 0;
		}
	}
	if (nodes[parent1].cost > nodes[parent2].cost){
		int t = parent1;
		parent1 = parent2;
		parent2 = t;
	}

}
void recom()
{
	for (int i = 0; i < size; i++)
	{
		check[i] = 0;
	}
	/////////////////
	int random = 0;
	for (int i = 0; i <3 * size / 4 + random; i++)
	{
		newChild.matrix.push_back(nodes[parent1].matrix[i]);
		check[nodes[parent1].matrix[i]] = 1;
	}
	for (int i = 3 * size / 4 + random; i <size; i++)
	{
		if (check[nodes[parent2].matrix[i]] == 0)
		{
			check[nodes[parent2].matrix[i]] = 1;
			newChild.matrix.push_back(nodes[parent2].matrix[i]);
		}
		else{
			newChild.matrix.push_back(0);
		}
	}
	for (int i = 0; i <size; i++){
		if (check[newChild.matrix[i]] == 0)
		{
			for (int j = 0; j< size; j++)
			{
				if (check[nodes[parent2].matrix[j]] == 0)
				{
					check[nodes[parent2].matrix[j]] = 1;
					newChild.matrix[i] = nodes[parent2].matrix[j];
					break;
				}
			}
		}
	}
}
void mutate(){
	int i1 = rand() % size;
	int i2 = rand() % size;
	while (i1 == i2){
		i2 = rand() % size;
	}
	int t = newChild.matrix[i1];
	newChild.matrix[i1] = newChild.matrix[i2];
	newChild.matrix[i2] = t;
}
void createChild(){

	newChild.chance = 0;
	newChild.cost = 0;
	newChild.matrix.clear();
	parentSelect();
	//int recChance = rand() % 100;
	//if (recChance < 100*recomChance)
	{
		recom();
		int mutChance = rand() % 101;
		if (mutationChance * 100 > mutChance){
			mutate();
		}
		childs.push_back(newChild);
		return;
	}
	childs.push_back(nodes[parent1]);

}
void forCreateChild(){

	for (int i = 0; i < nodes.size(); i++)
	{
		if (nodes[i].cost == 0)
		{
			foundAnswer = 1;
			return;
		}
	}


	for (int j = 0; j < Population; j++){
		createChild();
	}
	for (int j = 0; j < Population; j++){
		nodes.push_back(childs[j]);
	}

	childs.clear();

}
void survival(){
	cost();
	if (foundAnswer == 1){
		return;
	}
	for (int i = 0; i<nodes.size(); i++)
	{
		if (nodes[i].cost == 0)
		{
			foundAnswer = 1;
			return;
		}
	}
	chance();
	for (int i = Population; i > 0; i--){
		nodes.pop_back();
	}
}
void checkDup(){
	for (int i = 0; i< nodes.size(); i++){
		for (int j = i + 1; j < nodes.size(); j++){
			int flag = 1;
			for (int k = 0; k < size; k++){
				if (nodes[i].matrix[k] != nodes[j].matrix[k])
				{
					flag = 0;
					break;
				}
			}
			if (flag == 1){
				changed++;
				for (int k = 0; k < size; k++){
					check[k] = 0;
				}
				node temp;
				int point;
				for (int k = 0; k < size; k++){
					point = rand() % size;
					while (check[point] == 1){
						point = rand() % size;
					}
					check[point] = 1;
					temp.matrix.push_back(point + 1);
				}
				nodes[j].matrix = temp.matrix;
			}
		}
	}
}
void halfinit(){
	int changesize = Population - 1;
	for (int i = 0; i < changesize; i++)
	{
		nodes.pop_back();
	}
	for (int i = 0; i < changesize; i++)
	{
		for (int j = 0; j < size; j++)
		{
			check[j] = 0;
		}
		node temp;
		int point;
		for (int j = 0; j < size; j++){
			point = rand() % size;
			while (check[point] == 1){
				point = rand() % size;
			}
			check[point] = 1;
			temp.matrix.push_back(point + 1);
		}
		nodes.push_back(temp);
	}

}
void main()
{
	srand(time(0));
	cin >> n >> creatingGenNum >> mutationChance >> recomChance >> Population;
	size = n*n;
	check = new int[size];
	/////////////////////
	init();
	cost();
	chance();
	int premin = 0;
	int count = 0;
	int halfinitnum = 0;
	for (int passedIterations = 0; passedIterations < creatingGenNum; passedIterations++)
	{
		if (foundAnswer == 1)
			break;
		chance();
		forCreateChild();
		survival();
		if (foundAnswer == 1)
			break;
		checkDup();
		int min = nodes[0].cost;
		if (premin == min){
			count++;
		}
		else{
			premin = min;
			count = 0;
		}
		if (count == 250)
		{
			/*if (halfinitnum == n / 2)
			{
			count = 0;
			init();
			cost();
			if (foundAnswer == 1)
			break;
			halfinitnum=0;
			}
			else*/
			{
				count = 0;
				halfinit();
				cost();
				if (foundAnswer == 1)
					break;
				premin = 0;
				halfinitnum++;
			}
		}
		cout << passedIterations << " " << min << endl;
	}
	if (foundAnswer == 1){
		for (int k = 0; k < nodes.size(); k++){
			if (nodes[k].cost == 0){
				for (int p = 0; p < size; p++){
					if (p% n == 0){
						cout << endl;
					}
					cout << nodes[k].matrix[p] << " ";
				}
			}
		}
		system("pause");
		return;
	}
	system("pause");
}



/*int max = 0;
int maxi = 0;
for (int i = 0; i < nodes.size(); i++){
if (max < nodes[i].cost){
max = nodes[i].cost;
maxi = i;
}
}
for (int i = 0; i < nodes.size(); i++)
{
nodes[i].chance = (1000*max / nodes[i].cost);
}*/

/*
for (int i = 0; i < size; i++){
check[i] = 0;
}
for (int i = 0; i < (2*size / 3); i++){
newChild.matrix.push_back (nodes[parent1].matrix[i]);
check[nodes[parent1].matrix[i]-1] = 1;
}
for (int i = (2*size / 3); i < size; i++)
{
if (check[nodes[parent2].matrix[i]-1] == 0)
{
check[nodes[parent2].matrix[i]-1] = 1;
newChild.matrix.push_back(nodes[parent2].matrix[i]);
}
else
{
newChild.matrix.push_back(0);
}
}
for (int i = 0; i < size; i++)
{
if (newChild.matrix[i] == 0)
{
for (int j = 0; j < size; j++)
{
if (check[nodes[parent2].matrix[j]-1] == 0)
{
newChild.matrix[i] = nodes[parent2].matrix[j];
check[nodes[parent2].matrix[j]-1] = 1;
break;
}
}
}
}
*/

/*
double p = 0.7;
for (int i = 0; i < size; i++){
check[i] = 0;
}
for (int i = 0; i < nodes.size(); i++)
{
int min = 99999999;
int mini = 0;
for (int j = 0; j < nodes.size(); j++){
if (min > nodes[j].cost && check[j] == 0){
min = nodes[j].cost;
mini = j;
}
}
check[mini] = 1;
double opp = 1-p;
for (int j = 0; j < i - 1; j++){
opp = opp*opp;
}
p = p*opp;
nodes[mini].chance = p;
}*/



/*if (premin == min){
count++;
}
else{
premin = min;
count = 0;
}
if (count == creatingGenNum/n)
{
init();
cost();
}*/




/*double p = 0.3;
for (int i = 0; i < size; i++){
check[i] = 0;
}
for (int i = 0; i < nodes.size(); i++)
{
for (int j = 0; j < nodes.size(); j++){
if (nodes[i].cost < nodes[j].cost)
{
node temp;
temp.matrix = nodes[i].matrix;
temp.chance = nodes[i].chance;
temp.cost = nodes[i].cost;

nodes[i].matrix = nodes[j].matrix;
nodes[i].cost = nodes[j].cost;
nodes[i].chance = nodes[j].chance;

nodes[j].chance = temp.chance;
nodes[j].cost = temp.cost;
nodes[j].matrix = temp.matrix;
}
}
}
int k = 0;

int preCost = -1;
for (int i = 0; i < nodes.size(); i++){
double pp = 1;
for (int j = 0; j < k; j++){
pp = pp * (1 - p);
}
if (nodes[i].cost != preCost)
{
nodes[i].chance = pp * p;
k++;
preCost = nodes[i].cost;
}
else{
nodes[i].chance = pp*p;
}
}*/

/*
int sum = 0;
for (int i = 0; i < nodes.size(); i++){
sum += nodes[i].cost;
}


}
*/