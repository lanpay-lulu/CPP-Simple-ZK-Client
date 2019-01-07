/**
* It mainly shows how to watch a node for any change. 
*
*/


#include <stdio.h>
#include "stdlib.h"
#include "zkclient.h"

/**
* This is the main function zk client will run in its loop.
* In this function you need to handle any event you are interested in, and all connection events.
*/
void watcher_func(zhandle_t *zzh, int type, int state, 
			const char *path, void *watcherCtx) {

	printf("----- watcher_func called -----\n");
    ZkClient* zkClient = (ZkClient *) watcherCtx;
    if (type == ZOO_SESSION_EVENT) {
    	if(state == ZOO_CONNECTED_STATE) {
    	    // used at start process
			printf("ZkClient Connected to zookeeper service successfully!\n");
			zkClient->getData();
    	}
    	else if(state == ZOO_EXPIRED_SESSION_STATE) {
    		printf("ZkClient session expired!");
    		zkClient->close();
    		zkClient->start();
    	}
    	else {
    		printf("ZkClient session type=%d, state=%d!", type, state);
    	}
    }
    else if (type == ZOO_CHANGED_EVENT) {
    	printf("ZkClient node changed!\n");
		zkClient->getData();
    }
};

/**
* This is the function that handle changed data of the node you are watching.
*/
void get_complet_func(int rc, const char *value, int value_len,
 			const struct Stat *stat, const void *data) {

	if(rc == ZOK) {
        if (data == NULL) {
            printf("zk client get complet func param data is NULL.\n");
            return;
        }
        int32_t client_type = *(int32_t*)data;
		string s(value, value_len);
        // TODO do anything you like with s which is the new content of the watched node.
	}
	else {
		printf("get_complet_func failed! rc=%d\n", rc);
	}
}

void ZkClient::start() {
	this->zk = zookeeper_init(_connString.c_str(), watcher_func, timeout, cid, (void *) this, 0);
}

void ZkClient::close() {
	if(zk != NULL) {
		int ret = zookeeper_close(zk);
		if(ret != ZOK){
			printf("zookeeper_close error! ret=%d\n", ret);
		}
	}	
}

/**
* This function will register a node watcher.
*/
int ZkClient::getData() {
    // \param 1 watch if nonzero. A watch will be set at the server to notify the client if the node changes.
    // \param &_client_type will be passed to the get_complet_func function.
	int flag = zoo_aget(zk, _path.c_str(), 1, get_complet_func, &_client_type);
	if (flag == ZOK) {
		printf("ZkClient::getData() call success! waiting for callback!\n");
		return 0;
	}
	else {
		printf("ZkClient::getData() failed! flag=%d\n", flag);
		return -1;
	}
}

int main() {
	string conn("172.22.1.11:2181");
	string path("/wshtest");
    int32_t type = 22; // any value you like.
	ZkClient zkClient(conn, path, type);
	zkClient.start();
	//sleep(100);
	return 0;
}
