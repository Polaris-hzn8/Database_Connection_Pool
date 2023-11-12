# 数据库连接池

---

- 主要技术点：

    - mysql数据库编程、单例模式、queue队列容器、
    - C++11多线程编程、线程互斥、线程同步通信、unique_lock（并没有使用操作系统方面的api而是基于C++11语言级别实现，具有一定的跨平台特性）、
    - 基于CAS的原子整型、智能指针share_ptr、lambda表达式、生产者消费者线程模型

- 项目背景：

    - 为了提高mysql数据库的访问瓶颈，除了在服务器端增加缓存服务器、缓存常用数据之外（如redis、memory cache等），还可以增加数据库连接池，来提高mysqlserver的访问效率，
    - 在高并发的情况下，大量的tcp三次握手、mysqlserver连接认证、mysqlserver关闭连接回收资源、tcp四次挥手，这样的耗费的性能时间是非常惊人的，增加数据库连接池就是为了减少这一部分性能的损耗，
    - 在市场上比较流行的连接池包括阿里的druid、c3p0以及apache dbcp连接池，对于短时间内大量的数据库增删改查操作性能的提升是非常明显的，但是它们都是使用java实现，本项目采用C++实现数据库连接池，提高mysqlserver的访问效率，

- 主要功能点：

    连接池一般包含了数据库连接所用的ip地址、port端口号、用户名称、密码以及其他参数（例如初始连接量、最大连接量、最大空闲时间、连接超时时间等），主要实现连接池都支持的通用的基础功能，

    - **初始连接量initSize**：连接池会事先和mysql创建initSize个数的connection连接，当应用发起mysql访问时，不再创建和mysqlserver新的连接，而是直接从连接池中获取到一个可用的连接。使用完成后也并不会去释放connection，而是将当前connection归还到数据库连接池中，
    
    - **最大连接量maxSize**：当并发访问mysqlserver的请求增多时，初始连接量已经不够使用了，此时会根据新的请求数量自动创建更多的连接给应用去使用，但是新创建的连接数量的上限是maxize，
    
        不能无限制的创建连接，因为每一个连接都会占用一个socket资源，一般连接池和服务器程序是部署在一台主机上的，如果连接池占用过多的socket资源，那么服务器就不能接收太多的客户端请求了。
    
    - **最大空闲时间maxIdleTime**：当访问mysql的并发请求多了以后，连接池里面的连接数量会动态增加上限是maxSize，当这些连接用完再次归还到连接池当中，如果在指定的maxIdleTime里面，这些新增加的连接都没有被再次使用过，那么新增加的这些连接资源就要被回收掉，只需要保持初始连接量initSize个连接即可，
    
    - **连接超时时间connectionTimeout**：当mysql并发请求量过大，连接池中的连接数量已经达到maxSize了，而此时没有空闲的连接可以使用，那么此时应用从连接池获取连接无法成功。其通过阻塞的方式获取连接的时间如果超过了connectionTimeout时间，那么获取连接失败，无法访问数据库。










