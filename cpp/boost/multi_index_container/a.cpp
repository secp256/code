#include <cstdio>

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/shared_ptr.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <string>

using boost::multi_index_container ;
using namespace boost::multi_index ;

struct file_node
{
    int id ;                             // ---- 这个是文件的 id 号码
    std::string path_name ;              // ---- 这个是文件的 路径， 在设定索引的时候，我们将其设定为非重复的主索引项
    long length ;                        // ---- 这个是文件的 长度， 在本实验中没有用到
    std::string creator ;                // ---- 这个是文件的 作者， 在设定索引的时候，我们将其设定为可重复的辅助索引项


    file_node( int id , std::string path_name , std::string creator ):    // 这个是 文件-节点 结构体的构造函数
        id(id) , path_name(path_name) , length(0), creator(creator)
    { }

    friend std::ostream & operator<<(std::ostream &os , const file_node & f ) ;  // 这里重载了 operator<< 并将其设定为友元方法，
    // 为了方便将 file-node 中的属性信息转换为流
} ;

std::ostream &operator<< ( std::ostream &os , const file_node &f )            // 在这里根据文件路径打开文件，并读取文件的内容
{                                                                        // 将文件中的内容进行格式化并转换为 流 对象，并返回
    char file_buffer[1024] ;
    std::string file_content ;
    bool isOpen = false ;

    FILE *fp = fopen(f.path_name.c_str() , "r") ;
    if ( fp != NULL )
        isOpen = true ;

    if ( isOpen ) {
        int ret = fread((void*)file_buffer , sizeof(char) , sizeof(file_buffer)-1 , fp );
        file_buffer[ret] = '\0' ;
        file_content+= file_buffer ;
        fclose (fp) ;
    }
    else {
        file_content = "failed open file "+ f.path_name ;
    }

    os<<"[file path]     " << f.path_name << std::endl 
        << "[file index]     " << f.id << std::endl 
        <<"[file creator] " << f.creator << std::endl 
        <<"[file contents] " << file_content << std::endl << std::endl ; ;
    return os ;
}

/**
  here we tags for accessing the corresponding indices of file_node_table
  要将 struct file-node 中的某个属性字段设定为 file-node 的索引项的话，需要定义与该
  属性字段同名的空的结构体
  */

struct path_name {} ;   
struct creator {} ;

/**
  here we define a multi_index_container of file_node, with 
  following indices:
  - a unique index sorted by file_node::path_name
  - a non-unique index sorted by file_node::creator
  */

typedef multi_index_container <
    file_node, // table element , 要将什么作为创建表的元素，传入该结构体名称
    indexed_by <
        // 唯一索引，主索引，与表项一一映射， tag<传入刚刚定义好的 struct path_name {} 对应的名称 >
        ordered_unique <                
        // BOOST_MULTI_INDEX_MEMBER( 创建表中的元素结构体名称, 索引的类型， 索引的名称 等于 tag<> 中设定的名称 )
            tag<path_name> , BOOST_MULTI_INDEX_MEMBER(file_node, std::string , path_name) > , // this is for file's path_name , like primary-key in database table
        ordered_non_unique < tag<creator> , BOOST_MULTI_INDEX_MEMBER(file_node, std::string , creator) > 
    > // end indexed by
    // second index file node's creator
> file_node_table ;

    typedef boost::shared_ptr<file_node_table> ftPtr ; // 在这里我们使用 typedef 定义一个用来封装刚刚定义好的多重索引类型的 智能指针 类型


int main ()
{
    // try create obj by shared_ptr
    ftPtr file_table_ptr( new file_node_table ) ;  // shared_ptr 智能指针固有的创建对象的方法

    std::string file_path , author ;
    file_path = "/tmp/rsa_key" ;
    author = "kokia" ;

    // add additional node with creator/author name as 'Aimer'
    file_table_ptr->insert( file_node (1200 , "/tmp/rsa_key.pub" , "Aimer")) ; // 插入 file-node 文件-节点 作为表项

    for ( int i = 0 ; i < 5 ; i++ ) {
        char no = i+'0' ;    
        file_table_ptr->insert(file_node(i , file_path+no+".txt" , author)) ;
    }

    std::cout << "+++++++++++++++++++++ print files by author ++++++++++++++++++++ "<< std::endl ;
    file_node_table::index<creator>::type & index_by_author = file_table_ptr->get<creator>() ; 
    /*
       恭喜你，获得指向以 creator 属性排序的 文件表视图 , 该视图，通过 index_by_author 变量关联
       创建一个视图的迭代器，有了它便可以顺序遍历视图中 以 creator 属性进行排序的各个表项了，
       并访问表项中，也就是每个 file-node 中的各个属性了
    */                                                                                                         

    file_node_table::index<creator>::type::iterator creator_it = index_by_author.begin() ;     
    while ( creator_it != index_by_author.end()) {
        std::cout << *creator_it << std::endl ;
        creator_it++ ;
    }

    std::cout << "+++++++++++ print files by path name +++++++++++++++++++++" << std::endl ;
    // 这里和上面的原理是一样的，首先是获得 file-table 的按照 path_name 属性进行排序的视图 (注意视图与表的区别，视图仅仅是表的一个映像
    // 在系统中并不为视图中的元素提供实际的存储空间,类似于对象实体和指向对象的指针) ，通过该视图创建遍历视图的迭代器，通过迭代器
    // 逐个访问以 path_name 属性排序的视图中的各个 file-node 元素，并可以访问各个 file-node 中的属性值

    file_node_table::index<path_name>::type & file_table_view_index_by_path_name =
        file_table_ptr->get<path_name>() ;

    // path_it points to the first element in the VIEW which sorted by path_name
    file_node_table::index<path_name>::type::iterator 
        path_it = file_table_view_index_by_path_name.begin () ;

    for ( ; path_it != file_table_view_index_by_path_name.end() ; path_it++ ) {
        // call operator<< method of each object which pointed by iterator of path name
        std::cout << *path_it << std::endl ;
    }

    std::cout << "----------------------- test find method --------------------------------------" << std::endl ;
    //  在下面的演示中，我们首先通过 file_table_ptr->get 获得一个以 creator 属性排序的 file-node-table 的视图
    // 然后创建该 creator-视图调用 find 方法来查找 file-node.creator = "Aimer" 的 file-node
    // 如果找到的话， 迭代器是不会指向 creator-视图 的结尾空元素项的，通过该点即可判断， 因为 itr 类型特殊，不可使用 itr == NULL 判断
    // 在成功找到之后，将找到的对象通过特定类型(creator) 的迭代器指向，然后通过该迭代器即可访问 file-node 中的元素值了

    file_node_table::index<creator>::type::iterator 
        itr(file_table_ptr->get<creator>().find("Aimer")) ;

    if ( itr != file_table_ptr->get<creator>().end() ) {
        std::cout<< " find file node " << std::endl ;
        std::cout << *itr << std::endl ;
    }
    else {
        std::cout << " file node not find" << std::endl ;
    }

    return 0 ;
}
