/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2019-2020 Edward.Wu
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef _SLSManager_INCLUDE_
#define _SLSManager_INCLUDE_

#include <list>

#include "SLSRole.hpp"
#include "SLSRoleList.hpp"
#include "SLSGroup.hpp"
#include "SLSListener.hpp"
#include "conf.hpp"
#include "SLSMapData.hpp"
#include "SLSMapRelay.hpp"

#include "json.hpp"
using json = nlohmann::json;

/**
 * srt conf declare
 */
SLS_CONF_DYNAMIC_DECLARE_BEGIN(srt)
char             log_file[URL_MAX_LEN];
char             log_level[URL_MAX_LEN];
int              worker_threads;
int              worker_connections;
char             stat_post_url[URL_MAX_LEN];
int              stat_post_interval;
char             record_hls_path_prefix[URL_MAX_LEN];
int              http_port;
char             cors_header[URL_MAX_LEN];
char             database_path[URL_MAX_LEN];
// Rate limiting configuration
int              rate_limit_api;
int              rate_limit_stats;
int              rate_limit_config;
SLS_CONF_DYNAMIC_DECLARE_END


/**
 * srt cmd declare
 */
SLS_CONF_CMD_DYNAMIC_DECLARE_BEGIN(srt)
SLS_SET_CONF(srt, string, log_file,                    "save log file name.", 1, URL_MAX_LEN-1),
SLS_SET_CONF(srt, string, log_level,                   "log level", 1, URL_MAX_LEN-1),
SLS_SET_CONF(srt, int,    worker_threads,              "count of worker thread, if 0, only main thread.", 0, 100),
SLS_SET_CONF(srt, int,    worker_connections,          "", 1, 1024),
SLS_SET_CONF(srt, string, stat_post_url,               "statistic info post url", 1, URL_MAX_LEN-1),
SLS_SET_CONF(srt, int,    stat_post_interval,          "interval of statistic info post.", 1, 60),
SLS_SET_CONF(srt, string, record_hls_path_prefix,      "hls path prefix", 1, URL_MAX_LEN-1),
SLS_SET_CONF(srt, int,    http_port,                   "rest api port", 1, 65535),
SLS_SET_CONF(srt, string, cors_header,                 "cors header", 1, URL_MAX_LEN-1),
SLS_SET_CONF(srt, string, database_path,               "sqlite database path", 1, URL_MAX_LEN-1),
// Rate limiting settings
SLS_SET_CONF(srt, int,    rate_limit_api,             "rate limit for api endpoints (requests/minute)", 1, 1000),
SLS_SET_CONF(srt, int,    rate_limit_stats,           "rate limit for stats endpoints (requests/minute)", 1, 1000),
SLS_SET_CONF(srt, int,    rate_limit_config,          "rate limit for config endpoints (requests/minute)", 1, 1000)
SLS_CONF_CMD_DYNAMIC_DECLARE_END


/**
 * CSLSManager , manage players, publishers and listener
 */
class CSLSManager
{
public :
	CSLSManager();
	virtual ~CSLSManager();

	virtual int start();
	virtual int stop();
	virtual int reload();

	virtual int  single_thread_handler();
	bool         is_single_thread();
	int          check_invalid();

	json generate_json_for_publisher(std::string publisherName, int clear, bool legacy = false);
	json create_legacy_json_stats_for_publisher(CSLSRole *role, int clear);
	json create_json_stats_for_publisher(CSLSRole *role, int clear);
	char* find_publisher_by_player_key(char *player_key);

	void get_stat_info(std::string &info);
	static int  stat_client_callback(void *p, HTTP_CALLBACK_TYPE type, void *v, void* context);

private:
	std::list<CSLSListener *>     m_servers;
	int                           m_server_count;
	CSLSMapData                  *m_map_data;
	CSLSMapPublisher             *m_map_publisher;
	CSLSMapRelay                 *m_map_puller;
	CSLSMapRelay                 *m_map_pusher;

	std::list<CSLSGroup    *>     m_workers;
	int                           m_worker_threads;

	CSLSRoleList * m_list_role;
	CSLSGroup    * m_single_group;
};


#endif
