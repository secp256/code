#include<iostream>
#include<stdio.h>
#include<vector>
#include<map>
#include<poll.h>
#include<boost/noncopyable.hpp>
#include<boost/function.hpp>
#include<assert.h>
#include<boost/scoped_ptr.hpp>
#include<stdlib.h>
#include<sys/syscall.h>
#include<pthread.h>
#include<unistd.h>
#include<string.h>

using namespace std;
using namespace boost;
//以下类都是在同一个IO线程中运行为线程安全的故不需要同步机制

class Channel;//前向声明，事件分发器主要用于事件注册与事件处理(事件回调)
class Poller;//IO复用机制，主要功能是监听事件集合，即select，poll,epoll的功能

//事件循环，一个线程一个事件循环即one loop per thread，其主要功能是运行事件循环如等待事件发生然后处理发生的事件
class EventLoop:noncopyable {
public:
    EventLoop();
    ~EventLoop();
    void loop();//事件循环主体
    void quit();//终止事件循环，通过设定标志位所以有一定延迟
    void updateChannel(Channel* channel);//更新事件分发器Channel，完成文件描述符fd向事件集合注册事件及事件回调函数
    void assertInLoopThread(){//若运行线程不拥有EventLoop则退出，保证one loop per thread
        if(!isInLoopThread()){
            abortNotInLoopThread();
        }
    }
    bool isInLoopThread() const{return threadID_==syscall(SYS_gettid);}//判断运行线程是否为拥有此EventLoop的线程
private:
    void abortNotInLoopThread();//在不拥有EventLoop线程中终止
    typedef vector<Channel*> ChannelList;//事件分发器Channel容器，一个Channel只负责一个文件描述符fd的事件分发
    bool looping_;//事件循环主体loop是运行标志
    bool quit_;//取消循环主体标志
    const pid_t threadID_;//EventLoop的附属线程ID
    scoped_ptr<Poller> poller_;//IO复用器Poller用于监听事件集合
    ChannelList activeChannels_;//类似与poll的就绪事件集合，这里集合换成Channel(事件分发器具备就绪事件回调功能)
};

//IO Multiplexing Poller即poll的封装，主要完成事件集合的监听
class Poller:noncopyable{
public:
    typedef vector<Channel*> ChannelList;//Channel容器(Channel包含了文件描述符fd和fd注册的事件及事件回调函数)，Channel包含文件描述符及其注册事件及其事件回调函数，这里主要用于返回就绪事件集合
    Poller(EventLoop* loop);
    ~Poller();
    void Poll(int timeoutMs,ChannelList* activeChannels);//监听事件集合，通过activeChannels返回就绪事件集合
    void updateChannel(Channel* channel);//向事件集合中添加描述符欲监听的事件(channel中含有fd及其事件)
    void assertInLoopThread(){//判定是否和EventLoop的隶属关系，EventLoop要拥有此Poller
        ownerLoop_->assertInLoopThread();
    }
private:
    void fillActiveChannels(int numEvents,ChannelList* activeChannels) const;//将就绪的事件添加到activeChannels中用于返回就绪事件集合
    typedef vector<struct pollfd> PollFdList;//struct pollfd是poll系统调用监听的事件集合参数
    typedef map<int,Channel*> ChannelMap;//文件描述符fd到IO分发器Channel的映射，通过fd可以快速找到Channel
    //注意:Channel中有fd成员可以完成Channel映射到fd的功能，所以fd和Channel可以完成双射
    EventLoop* ownerLoop_;//隶属的EventLoop
    PollFdList pollfds_;//监听事件集合
    ChannelMap channels_;//文件描述符fd到Channel的映射
};

//事件分发器该类包含：文件描述符fd、fd欲监听的事件、事件的处理函数(事件回调函数)
class Channel:noncopyable{
public:
    typedef function<void()> EventCallback;//事件回调函数类型,回调函数的参数为空，这里将参数类型已经写死了
    Channel(EventLoop* loop,int fd);//一个Channel只负责一个文件描述符fd但Channel不拥有fd，可见结构应该是这样的：EventLoop调用Poller监听事件集合，就绪的事件集合元素就是Channel。但Channel的功能不仅是返回就绪事件，还具备事件处理功能
    void handleEvent();//处理事件回调
    void setReadCallBack(const EventCallback& cb){//可读事件回调
        readCallback=cb;
    }
    void setWriteCallback(const EventCallback& cb){//可写事件回调
        writeCallback=cb;
    }
    void setErrorCallback(const EventCallback& cb){//出错事件回调
        errorCallback=cb;
    }
    int fd() const{return fd_;}//返回Channel负责的文件描述符fd，即建立Channel到fd的映射
    int events() const{return events_;}//返回fd域注册的事件类型
    void set_revents(int revt){//设定fd的就绪事件类型，再poll返回就绪事件后将就绪事件类型传给此函数，然后此函数传给handleEvent，handleEvent根据就绪事件的类型决定执行哪个事件回调函数
        revents_=revt;
    }
    bool isNoneEvent() const{//fd没有想要注册的事件
        return events_==kNoneEvent;
    }
    void enableReading(){//fd注册可读事件
        events_|=kReadEvent;
        update();
    }
    void enableWriting(){//fd注册可写事件
        events_|=kWriteEvent;
        update();
    }
    int index(){return index_;}//index_是本Channel负责的fd在poll监听事件集合的下标，用于快速索引到fd的pollfd
    void set_index(int idx){index_=idx;}
    EventLoop* ownerLoop(){return loop_;}
private:
    void update();
    static const int kNoneEvent;//无任何事件
    static const int kReadEvent;//可读事件
    static const int kWriteEvent;//可写事件

    EventLoop* loop_;//Channel隶属的EventLoop(原则上EventLoop，Poller，Channel都是一个IO线程)
    const int fd_;//每个Channel唯一负责的文件描述符，Channel不拥有fd
    int events_;//fd_注册的事件
    int revents_;//通过poll返回的就绪事件类型
    int index_;//在poll的监听事件集合pollfd的下标，用于快速索引到fd的pollfd
    EventCallback readCallback;//可读事件回调函数，当poll返回fd_的可读事件时调用此函数执行相应的事件处理，该函数由用户指定
    EventCallback writeCallback;//可写事件回调函数
    EventCallback errorCallback;//出错事件回调函数
};

/*
 *EventLoop成员实现
 */
__thread EventLoop* t_loopInThisThread=0;//线程私有数据表示线程是否拥有EventLoop
const int kPollTimeMs=10000;//poll等待时间

EventLoop::EventLoop():looping_(false),quit_(false),threadID_(syscall(SYS_gettid)),poller_(new Poller(this)){
    if(!t_loopInThisThread){
        t_loopInThisThread=this;//EventLoop构造时线程私有数据记录
    }
}

EventLoop::~EventLoop(){
    assert(!looping_);
    t_loopInThisThread=NULL;//EventLoop析构将其置空
}

void EventLoop::loop() {//EventLoop主循环，主要功能是监听事件集合，执行就绪事件的处理函数
    cout << "begin event loop" << endl;
    assert(!looping_);
    assertInLoopThread();
    looping_=true;
    quit_=false;
    while(!quit_){
        activeChannels_.clear();
        poller_->Poll(kPollTimeMs,&activeChannels_);//activeChannels是就绪事件
        for(ChannelList::iterator it=activeChannels_.begin();it!=activeChannels_.end();it++){
            printf("handle events...\n");
            (*it)->handleEvent();//处理就绪事件的回调函数
        }
    }
    cout << "quit event loop" << endl;
    looping_=false;
}

void EventLoop::quit(){
    quit_=true;//停止主循环标志，主循环不会马上停止有延迟
}

void EventLoop::updateChannel(Channel* channel){//主要用于文件描述符添加到poll的监听事件集合中
    assert(channel->ownerLoop()==this);
    assertInLoopThread();
    poller_->updateChannel(channel);
}

void EventLoop::abortNotInLoopThread() {
    printf("abort not in Loop Thread\n");
    abort();//非本线程调用强行终止
}

/*
 *Poller成员实现
 */
Poller::Poller(EventLoop* loop):ownerLoop_(loop){}//Poller明确所属的EventLoop
Poller::~Poller(){}

void Poller::Poll(int timeoutMs,ChannelList* activeChannels) {
    int numEvents=poll(&*pollfds_.begin(),pollfds_.size(),timeoutMs);//poll监听事件集合pollfds_
    if(numEvents>0){
        fillActiveChannels(numEvents,activeChannels);//将就绪的事件添加到activeChannels
    }
    else if(numEvents==0){
    }
    else{
        printf("Poller::Poll error\n");
    }
}

void Poller::fillActiveChannels(int numEvents,ChannelList* activeChannels) const{//将就绪事件通过activeChannels返回
    for(PollFdList::const_iterator pfd=pollfds_.begin();pfd!=pollfds_.end()&&numEvents>0;++pfd){
        if(pfd->revents>0){
            --numEvents;//若numEvents个事件全部找到就不需要再遍历容器剩下的部分
            ChannelMap::const_iterator ch=channels_.find(pfd->fd);
            assert(ch!=channels_.end());
            Channel* channel=ch->second;
            assert(channel->fd()==pfd->fd);
            channel->set_revents(pfd->revents);
            activeChannels->push_back(channel);
        }
    }
}

void Poller::updateChannel(Channel* channel){
    assertInLoopThread();
    if(channel->index()<0){//若channel的文件描述符fd没有添加到poll的监听事件集合中
        assert(channels_.find(channel->fd())==channels_.end());
        struct pollfd pfd;
        pfd.fd=channel->fd();
        pfd.events=static_cast<short>(channel->events());
        pfd.revents=0;
        pollfds_.push_back(pfd);
        int idx=static_cast<int>(pollfds_.size())-1;
        channel->set_index(idx);
        channels_[pfd.fd]=channel;
    }
    else{//若已经添加到监听事件集合中，但是需要修改
        assert(channels_.find(channel->fd())!=channels_.end());
        assert(channels_[channel->fd()]==channel);
        int idx=channel->index();
        assert(0<=idx&&idx<static_cast<int>(pollfds_.size()));
        struct pollfd& pfd=pollfds_[idx];
        assert(pfd.fd==channel->fd()||pfd.fd==-1);
        pfd.events=static_cast<short>(channel->events());//修改注册事件类型
        pfd.revents=0;
        if(channel->isNoneEvent()){
            pfd.fd=-1;//若无事件则poll忽略
        }
    }
}

/*
 *Channel成员实现
 */
const int Channel::kNoneEvent=0;//无事件
const int Channel::kReadEvent=POLLIN|POLLPRI;//可读事件
const int Channel::kWriteEvent=POLLOUT;//可写事件
Channel::Channel(EventLoop* loop,int fdArg):loop_(loop),fd_(fdArg),events_(0),revents_(0),index_(-1){}
void Channel::update(){//添加或修改文件描述符的事件类型
    loop_->updateChannel(this);
}

void Channel::handleEvent(){//处理就绪事件的处理函数
    if(revents_&POLLNVAL){
        printf("Channel::handleEvent() POLLNVAL\n");
    }
    if(revents_&(POLLERR|POLLNVAL)){//出错回调
        if(errorCallback)
            errorCallback();
    }
    if(revents_&(POLLIN|POLLPRI|POLLRDHUP)){//可读回调
        if(readCallback) {
            cout << "readCallback" << endl;
            readCallback();
        }
    }
    if(revents_&POLLOUT){//可写回调
        if(writeCallback) {
            cout << "writeCall back" << endl;
            writeCallback();
        }
    }
}
/*
 *测试代码，主线程往管道写数据，子线程通过EventLoop监听管道读端然后执行相应的可读回调(读取数据并输出)
 */
int pipefd[2];
EventLoop* loop;

void ReadPipe(){
    char buf[1024];
    read(pipefd[0],buf,1024);
    printf("[%s]\n",buf);
    loop->quit();//执行完可读回调后终止EventLoop的事件循环loop
}

void* threadFun(void* arg){
    loop=new EventLoop();
    Channel channel(loop, pipefd[0]);
    channel.setReadCallBack(ReadPipe);
    channel.enableReading();
    loop->loop();
}

int main(){
    pipe(pipefd);

    pthread_t pid;
    pthread_create(&pid,NULL,&threadFun,NULL);

    const char* ptr="Can you get this data?";
    write(pipefd[1], ptr, strlen(ptr));

    pthread_join(pid,NULL);

    return 0;
}
