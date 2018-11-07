//停车场管理系统，车辆信息用结构体，停车场用栈（顺序结构），等候车道用队列（链接结构），中转道用栈（顺序结构）
#include <iostream>

using namespace std;

struct car_inf
{
  int car_num;
  int eve_time;
};

class park
{
    private:
        car_inf *elem;
        int maxsize;
    public:
        int top_p;
        park(int initSize=10);
        ~park();
        bool isEmpty() const;
        void push(const car_inf &x);
        car_inf pop(){return elem[top_p--];};

};

park::park(int initSize)
{
    elem=new car_inf[initSize];
    maxsize=initSize;
    top_p=-1;
}

park::~park()
{
    delete[] elem;
}

bool park::isEmpty() const
{
    return top_p==-1;
}

void park::push(const car_inf &x)
{
    elem[++top_p]=x;
}

struct node
{
    car_inf data;
    node *next;
    node(const car_inf &x, node *N=NULL)
    {
        data=x;
        next=N;
    }
    node():next(NULL){}
    ~node(){}
};

class waitline
{
    public:
        node *front,*rear;
        waitline();
        ~waitline();
        bool isEmpty() const;
        void enQuene(const car_inf &x);
        car_inf deQuene();
        car_inf getHead()const;
};

waitline::waitline()
{
    front=rear=NULL;
}

waitline::~waitline()
{
    node *tmp;
    while(front!=NULL)
    {
        tmp=front;
        front=front->next;
        delete tmp;
    }
}

bool waitline::isEmpty()const
{
    return front==NULL;
}

void waitline::enQuene(const car_inf &x)
{
    if(rear==NULL)
    {
        front=rear=new node(x);
    }
    else
        rear=rear->next=new node(x);
}

car_inf waitline::deQuene()
{
    node *tmp=front;
    car_inf value=front->data;
    front=front->next;
    if(front==NULL) rear = NULL;
    delete tmp;
    return value;
}

car_inf waitline::getHead()const
{
    return front->data;
}

class transfer_line
{
    private:
        car_inf *elem;
        int maxsize;
    public:
        int top_p;                          //后面要用到这个指针，所以用公有的了
        transfer_line(int initSize=10);
        ~transfer_line();
        bool isEmpty() const;
        void push(const car_inf &x);
        car_inf pop(){return elem[top_p--];};
};

transfer_line::transfer_line(int initSize)
{
    elem=new car_inf[initSize];
    maxsize=initSize;
    top_p=-1;
}

transfer_line::~transfer_line()
{
    delete[] elem;
}

bool transfer_line::isEmpty() const
{
    return top_p==-1;
}

void transfer_line::push(const car_inf &x)
{
    elem[++top_p]=x;
}

int main()
{
    bool flag=true;//判断是否继续要求用户输入车辆信息，执行循环结构
    char car_condition;      //记录车辆到达或者离开或者结束时间
    car_inf arrcar;
    int car_number,happentime;   //记录车牌号信息与相应的事件发生时间
    int parksize=2;  //停车场的车位数
    park ori_park(parksize);
    waitline ori_waitline;
    transfer_line ori_transferline(parksize);
    int amont_of_waitline=0;//记录便车道还有几辆车
    while(flag)
    {
        cout<<"请输入此时的状态信息"<<endl;
        cin>>car_condition;
        cout<<"请输入车牌照号码"<<endl;
        cin>>car_number;
        cout<<"请输入此时的时间"<<endl;
        cin>>happentime;
        if(car_condition=='A')
        {
            arrcar.car_num=car_number;
            arrcar.eve_time=happentime;
            if(ori_park.top_p==parksize-1)  //停车场已满
            {
                amont_of_waitline++;
                ori_waitline.enQuene(arrcar);
                cout<<"Sorry,the parking space is full,your car will be waiting at the driveway, the number is:"<<amont_of_waitline<<endl;//此处记得有个缺点，还没写便车道上面的停车顺序
            }
            else
            {
                ori_park.push(arrcar);
                cout<<"Your car has been parked in the park, and the position of it is: "<<ori_park.top_p+1<<endl;
            }
        }
        else if (car_condition=='D')
        {
            car_inf leaving_car;
            while(true)
            {
                car_inf trans_car;
                trans_car=ori_park.pop();
                if(trans_car.car_num==car_number)
                {
                    leaving_car=trans_car;
                    break;
                }
                else
                    ori_transferline.push(trans_car);
            }
            while(ori_transferline.top_p!=-1)
            {
                ori_park.push(ori_transferline.pop());
            }
            if(ori_waitline.isEmpty()!=1)
            {
                car_inf car_entering;
                car_entering=ori_waitline.getHead();
                car_entering.eve_time=happentime;
                ori_park.push(car_entering);
                amont_of_waitline--;
            }
            int parkingtime=happentime-leaving_car.eve_time;
            cout<<"The car is leaving now,and the parking time is:"<<parkingtime<<endl;
        }
        else if(car_condition=='E')
        {
            flag=false;
        }
    }
    return 0;
}
