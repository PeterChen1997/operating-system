//
// Created by peterchen on 18-5-15.
//

#include <iostream>

using namespace std;

struct Time //时间的数据结构
{
    int hour;
    int minute;
};

struct Job //作业
{
    string jobname;   //作业名
    Time intime;      //进入时间
    int runtime;      //作业估计运行时间
    Time starttime;   //作业开始时间
    Time endtime;     //作业结束时间
    float cycletime;    //作业周转时间
    float cltime;     //作业带权周转时间
    bool haverun;      //是否已运行
};

float T=0;//作业平均周转时间
float W=0;//作业带权平均周转时间

void showInput(Job job[],int n)  //输入提示
{
    cout<<"**********请按作业进入时间先后顺序输入*********"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<"作业"<<i+1<<":"<<endl;
        cout<<"作业名(e.g: job1):";
        cin>>job[i].jobname;
        cout<<"作业进入时间(e.g: 8:00):";
        scanf("%d:%d",&job[i].intime.hour,&job[i].intime.minute);
        cout<<"作业估计运行时间(e.g: 30)(min):";
        cin>>job[i].runtime;
        // init
        job[i].starttime.hour=0;
        job[i].starttime.minute=0;
        job[i].endtime.hour=0;
        job[i].endtime.minute=0;
        job[i].cycletime=0;
        job[i].cltime=0;
        job[i].haverun=false; //标记为未运行
        cout<<"*********************"<<endl;
    }
}

void Init(Job job[],int n)//初始化
{
    for(int i=0;i<n;i++)
    {
        job[i].starttime.hour=0;
        job[i].starttime.minute=0;
        job[i].endtime.hour=0;
        job[i].endtime.minute=0;
        job[i].cycletime=0;
        job[i].cltime=0;
        job[i].haverun=false; //标记为未运行
    }
    T=0;
    W=0;
}

void showTime(Time time)    //显示时间
{
    cout<<time.hour<<":"<<time.minute;
}
int timeDiff(Time t1,Time t2)  //计算时间差,时间t1比t2大
{
    return t1.hour*60+t1.minute-(t2.hour*60+t2.minute);
}
Time timeAdd(Time time,int addtime) //时间相加
{
    time.hour+=addtime/60;
    time.minute+=addtime%60;
    if(time.minute>=60)
    {
        time.hour++;
        time.minute-=60;
    }
    return time;
}
bool comtime(Time t1,Time t2)//比较两个时间的大小，第一个大就返回TRUE
{
    if(t1.hour>t2.hour)
        return true;
    else return t1.hour == t2.hour && t1.minute > t2.minute;
}

void showResult(Job job[],int n)  //显示结果
{
    cout<<"jobname\tintime\truntime\tsttime\tendtime\t周转时间(分钟)\t带权周转时间"<<endl;
    cout<<"*******************************************************************"<<endl;
    for(int i=0;i<n;i++)
    {
        cout<<job[i].jobname<<"\t";
        showTime(job[i].intime);
        cout<<"\t"<<job[i].runtime<<"\t";
        showTime(job[i].starttime);
        cout<<"\t";
        showTime(job[i].endtime);
        cout<<"\t  "<<job[i].cycletime<<"\t\t  "<<job[i].cltime<<endl;
    }
    cout<<"作业平均周转时间：T="<<T/(n*1.0)<<endl;
    cout<<"作业带权平均周转时间：W="<<W/(n*1.0)<<endl;
}

int minRuntime(Job job[],int n,Time &t) //找出作业中最短作业下标
{
    int min=-1;
    int hasRunSum = 0;
    // 筛选出一个满足要求的下标
    for(int i=0;i<n;i++)
    {
        // 要求： ×不在运行中 ×达到开始时间
        if(!job[i].haverun && comtime(t,job[i].intime))
        {
            min=i;
            break;
        } else if(job[i].haverun) {
            hasRunSum++;
        }
    }
    // 继续等待后续作业
    if(min == -1 && hasRunSum < n) {
        t = timeAdd(t, 1);
        return -2;
    }
    // 根据下标执行判断
    for(int j=min+1;j<n;j++)
        // 未执行 执行时间短 达到开始时间
        if(!job[j].haverun && job[j].runtime<job[min].runtime && comtime(t,job[j].intime))
            min=j;

    // 返回满足的下标
    return min;
}

void SJF(Job job[],int n)//短作业优先作业调度
{
    // 结束时间容器
    Time t;
    // 第一个作业开始执行
    job[0].starttime.hour=job[0].intime.hour;
    job[0].starttime.minute=job[0].intime.minute;
    job[0].endtime=timeAdd(job[0].starttime,job[0].runtime);
    job[0].haverun=true;
    job[0].cycletime=timeDiff(job[0].endtime,job[0].intime);
    job[0].cltime=job[0].cycletime*1.0/job[0].runtime;

    T+=job[0].cycletime;
    W+=job[0].cltime;
    t=job[0].endtime;

    // 寻找下一个最短作业
    while(minRuntime(job,n,t)!=-1)
    {
        if(minRuntime(job, n, t) == -2) {
            continue;
        }
        // 下标为i
        int i=minRuntime(job,n,t);
        // 记录开始时间
        if(comtime(job[i].intime,t))
            job[i].starttime=job[i].intime;
        else
            job[i].starttime=t;
        // 执行
        job[i].endtime=timeAdd(job[i].starttime,job[i].runtime);
        job[i].haverun=true;
        job[i].cycletime=timeDiff(job[i].endtime,job[i].intime);
        job[i].cltime=job[i].cycletime*1.0/job[i].runtime;

        T+=job[i].cycletime;
        W+=job[i].cltime;
        t=job[i].endtime;
    }
}

int Firstintime(Job job[],int &n) //找出作业中最先到的
{
    int min=-1;
    for(int i=0;i<n;i++)
    {
        if(!job[i].haverun)
        {
            min=i;
            break;
        }
    }
    for(int j=min+1;j<n;j++)
        if(!job[j].haverun && comtime(job[min].intime,job[j].intime))
            min=j;
    return min;
}
void FCFS(Job job[],int n)//先来先服务作业调度
{
    Time t;

    job[0].starttime.hour=job[0].intime.hour;
    job[0].starttime.minute=job[0].intime.minute;
    job[0].endtime=timeAdd(job[0].starttime,job[0].runtime);
    job[0].haverun=true;
    job[0].cycletime=timeDiff(job[0].endtime,job[0].intime);
    job[0].cltime=job[0].cycletime*1.0/job[0].runtime;

    T+=job[0].cycletime;
    W+=job[0].cltime;
    t=job[0].endtime;

    while(Firstintime(job,n)!=-1)
    {
        int i=Firstintime(job,n);

        if(comtime(job[i].intime,t))
            job[i].starttime=job[i].intime;
        else
            job[i].starttime=t;

        job[i].endtime=timeAdd(job[i].starttime,job[i].runtime);
        job[i].haverun=true;
        job[i].cycletime=timeDiff(job[i].endtime,job[i].intime);
        job[i].cltime=job[i].cycletime*1.0/job[i].runtime;

        T+=job[i].cycletime;
        W+=job[i].cltime;
        t=job[i].endtime;
    }
}

int HighestResponseRadio(Job job[],int n, Time &t) //找出作业中相应比最高的
{
    int max=-1;
    int hasRunSum = 0;
    // 筛选出一个满足要求的下标
    for(int i=0;i<n;i++)
    {
        // 要求： ×不在运行中 ×达到开始时间
        if(!job[i].haverun && comtime(t,job[i].intime))
        {
            max=i;
            break;
        } else if(job[i].haverun) {
            hasRunSum++;
        }
    }
    // 继续等待后续作业
    if(max == -1 && hasRunSum < n) {
        t = timeAdd(t, 1);
        return -2;
    }
    // 根据下标执行判断
    for(int j=max+1;j<n;j++)
        // 未执行 相应比高 达到开始时间
        if(
                !job[j].haverun
                && (job[j].runtime + job[j].cycletime) / job[j].runtime > (job[max].runtime + job[max].cycletime) / job[max].runtime
                && comtime(t,job[j].intime)
                )
            max=j;

    // 返回满足的下标
    return max;
}

void HRN(Job job[],int n)//高相应比优先作业调度
{
    Time t;

    job[0].starttime.hour=job[0].intime.hour;
    job[0].starttime.minute=job[0].intime.minute;
    job[0].endtime=timeAdd(job[0].starttime,job[0].runtime);
    job[0].haverun=true;
    job[0].cycletime=timeDiff(job[0].endtime,job[0].intime);
    job[0].cltime=job[0].cycletime*1.0/job[0].runtime;

    T+=job[0].cycletime;
    W+=job[0].cltime;
    t=job[0].endtime;

    // TODO: 加上时间
    while(HighestResponseRadio(job,n,t)!=-1)
    {
        if(HighestResponseRadio(job, n, t) == -2) {
            continue;
        }

        int i=HighestResponseRadio(job,n,t);

        if(comtime(job[i].intime,t))
            job[i].starttime=job[i].intime;
        else
            job[i].starttime=t;

        job[i].endtime=timeAdd(job[i].starttime,job[i].runtime);
        job[i].haverun=true;
        job[i].cycletime=timeDiff(job[i].endtime,job[i].intime);
        job[i].cltime=job[i].cycletime*1.0/job[i].runtime;

        T+=job[i].cycletime;
        W+=job[i].cltime;
        t=job[i].endtime;
    }
}

int main()
{
    cout<<"请输入作业数：";
    int n;   //作业数
    cin>>n;
    Job* job=new Job[n];
    if(n<=0)
    {
        cout<<"输入不合法!";
        exit(-1);
    }
    else
    {
        showInput(job,n);
        cout<<endl;

        FCFS(job,n);
        cout<<"先来先服务："<<endl;
        showResult(job,n);
        cout<<endl;

        Init(job,n);

        SJF(job,n); //短作业优先
        cout<<"短作业优先："<<endl;
        showResult(job,n);

        Init(job,n);

        HRN(job,n); //高相应比优先
        cout<<"高相应比优先："<<endl;
        showResult(job,n);

    }
    system("pause");
    return 0;
}