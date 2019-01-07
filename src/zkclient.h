
#include <string> 
#include <vector>
#include <mutex>
#include <condition_variable>
#include <zookeeper/zookeeper.h>


using std::string;
using std::mutex;
using std::unique_lock;

class ZkClient {
	public:
		ZkClient(string connString, string path, int32_t client_type): _connString(connString), _path(path),
        _client_type(client_type) {

		}

		~ZkClient() {
			close();
		}

		void start();

		void close();

		int getData();

	private:
		string _connString;
		string _path;
		int timeout = 5000;
		clientid_t *cid = 0;
		zhandle_t * zk;

		friend void watcher_func(zhandle_t *zzh, int type, int state, 
			const char *path, void *watcherCtx);

		friend void get_complet_func(int rc, const char *value, int value_len,
 			const struct Stat *stat, const void *data);

        int32_t _client_type;

}; // end of class


