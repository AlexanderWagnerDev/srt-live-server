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


#ifndef _SLSListener_INCLUDE_
#define _SLSListener_INCLUDE_

#include <map>
#include <string>

#include "SLSRole.hpp"
#include "SLSRoleList.hpp"
#include "SLSPublisher.hpp"
#include "conf.hpp"
#include "SLSRecycleArray.hpp"
#include "SLSMapPublisher.hpp"
#include "SLSMapRelay.hpp"

/**
 * server conf
 */
SLS_CONF_DYNAMIC_DECLARE_BEGIN(server)
int              listen_player;        // Port for players
int              listen_publisher;     // Port for publishers
int              backlog;
int              latency_min;          // Minimum allowed latency (ms)
int              latency_max;          // Maximum allowed latency (ms)
int              idle_streams_timeout;//unit s; -1: unlimited
char             on_event_url[URL_MAX_LEN];
char             stream_ids_file[URL_MAX_LEN]; // Path to stream IDs JSON file
// Publisher specific settings
int              publisher_exit_delay;
char             record_hls[SHORT_STR_MAX_LEN];
int              record_hls_segment_duration;
SLS_CONF_DYNAMIC_DECLARE_END

/**
 * sls_conf_server_t
 */
SLS_CONF_CMD_DYNAMIC_DECLARE_BEGIN(server)
SLS_SET_CONF(server, int,    listen_player,        "listen port for players", 1, 65535),
SLS_SET_CONF(server, int,    listen_publisher,     "listen port for publishers", 1, 65535),
SLS_SET_CONF(server, int,    backlog,              "how many sockets may be allowed to wait until they are accepted", 1,    1024),
SLS_SET_CONF(server, int,    latency_min,          "minimum allowed latency (ms)", 0, 5000),
SLS_SET_CONF(server, int,    latency_max,          "maximum allowed latency (ms)", 0, 10000),
SLS_SET_CONF(server, int,    idle_streams_timeout, "players idle timeout when no publisher" , -1, 86400),
SLS_SET_CONF(server, string, on_event_url,         "on connect/close http url", 1,    URL_MAX_LEN-1),
SLS_SET_CONF(server, string, stream_ids_file,      "path to stream IDs JSON file", 1, URL_MAX_LEN-1),
// Publisher specific settings
SLS_SET_CONF(server, int,    publisher_exit_delay, "delay exit time, unit second.", 1, 300),
SLS_SET_CONF(server, string, record_hls,           "record_hls switch", 1, SHORT_STR_MAX_LEN-1),
SLS_SET_CONF(server, int,    record_hls_segment_duration, "record_hls_segment_duration", 1, 3600),
SLS_CONF_CMD_DYNAMIC_DECLARE_END


/**
 * SLSListener
 */
class CSLSListener : public CSLSRole
{
public :
	CSLSListener();
    ~CSLSListener();

    int         init();
    int         uninit();


    virtual int start();
    virtual int stop();

    virtual int  handler();

    void set_role_list(CSLSRoleList *list_role);
    void set_map_publisher(CSLSMapPublisher * publisher);
    void set_map_puller(CSLSMapRelay *map_puller);
    void set_map_pusher(CSLSMapRelay *map_puller);
    void set_record_hls_path_prefix(char *path);
    void set_listener_type(bool is_publisher);

    virtual std::string get_stat_info();

private:
    CSLSRoleList      * m_list_role;
    CSLSMapPublisher  * m_map_publisher;
    CSLSMapRelay      * m_map_puller;
    CSLSMapRelay      * m_map_pusher;

    CSLSMutex           m_mutex;

    int                 m_idle_streams_timeout_role;
    std::string 		m_stat_info;
    char                m_http_url_role[URL_MAX_LEN];
    char                m_record_hls_path_prefix[URL_MAX_LEN];
    bool                m_is_publisher_listener;
    char                m_stream_id_json_path[URL_MAX_LEN];

    int  init_conf_app();
    bool validate_stream_id(const char* stream_id, char* mapped_id = nullptr);

};


#endif
