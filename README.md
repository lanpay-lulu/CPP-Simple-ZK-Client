# CPP-Simple-ZK-Client
This is a simple use case of zookeeper client in c++ and mostly for watch function.


## Introduction
Each zookeeper client instance will have two threads. One is IO-thread, which handles all io event. The other one is completion-thread, which handles all completion function.

Each watcher will only be invoked once, so if you want to continusly watch any event, you should set a watcher at the end of a watch completion function, or at the io loop funtion.

## Note
Use zookeeper_mt library.
