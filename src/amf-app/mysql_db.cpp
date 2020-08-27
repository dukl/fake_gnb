#include "amf_n1.hpp"
#include "logger.hpp"
#include "amf_config.hpp"
using namespace amf_application;
using namespace config;

extern amf_config amf_cfg;

bool amf_n1::get_mysql_auth_info(std::string imsi, mysql_auth_info_t &resp){//openair-cn/tree/v0.5.0/src/oai_hss/db/db_connector.c
  MYSQL_RES *res;
  MYSQL_ROW  row;
  std::string query;

  if(!db_desc->db_conn){
    Logger::amf_n1().error("Cannot connect to mysql db");
    return false;
  }
  query = "SELECT `key`,`sqn`,`rand`,`OPc` FROM `users` WHERE `users`.`imsi`='"+imsi+"' ";
  pthread_mutex_lock (&db_desc->db_cs_mutex);
  if(mysql_query(db_desc->db_conn, query.c_str())){
    pthread_mutex_unlock (&db_desc->db_cs_mutex);
    Logger::amf_n1().error("Query execution failed: %s\n", mysql_error (db_desc->db_conn));
    return false;
  }
  res = mysql_store_result (db_desc->db_conn);
  pthread_mutex_unlock (&db_desc->db_cs_mutex);
  if(!res){
    Logger::amf_n1().error("data fetched from mysql is not present");
    return false;
  }
  if(row = mysql_fetch_row(res)){
    if(row[0] == NULL || row[1] == NULL || row[2] == NULL || row[3] == NULL){
      Logger::amf_n1().error("row data failed");
      return false;
    }
  memcpy (resp.key, row[0], KEY_LENGTH);
  uint64_t sqn = 0;
  sqn = atoll(row[1]);
  resp.sqn[0] = (sqn & (255UL << 40)) >> 40;
  resp.sqn[1] = (sqn & (255UL << 32)) >> 32;
  resp.sqn[2] = (sqn & (255UL << 24)) >> 24;
  resp.sqn[3] = (sqn & (255UL << 16)) >> 16;
  resp.sqn[4] = (sqn & (255UL << 8)) >> 8;
  resp.sqn[5] = (sqn & 0xff);
  memcpy (resp.rand, row[2], RAND_LENGTH);
  memcpy (resp.opc, row[3], KEY_LENGTH);
  } 
  mysql_free_result (res);
  return true;
}


bool amf_n1::connect_to_mysql(){
  const int mysql_reconnect_val = 1;
  db_desc = (database_t*)calloc(1, sizeof(database_t));
  if(!db_desc){
    Logger::amf_n1().error("An error occurs when calloc");
    return false;
  }
  pthread_mutex_init (&db_desc->db_cs_mutex, NULL);
  db_desc->server = amf_cfg.auth_para.mysql_server;
  db_desc->user = amf_cfg.auth_para.mysql_user;
  db_desc->password = amf_cfg.auth_para.mysql_pass;
  db_desc->database = amf_cfg.auth_para.mysql_db;
  db_desc->db_conn = mysql_init (NULL);
  mysql_options (db_desc->db_conn, MYSQL_OPT_RECONNECT, &mysql_reconnect_val);
  if (!mysql_real_connect (db_desc->db_conn, db_desc->server.c_str(), db_desc->user.c_str(), db_desc->password.c_str(), db_desc->database.c_str(), 0, NULL, 0)) {
    Logger::amf_n1().error("An error occured while connecting to db: %s", mysql_error (db_desc->db_conn));
    mysql_thread_end();
    return false;
  }
  mysql_set_server_option (db_desc->db_conn, MYSQL_OPTION_MULTI_STATEMENTS_ON);
  return true;
}
 void amf_n1::mysql_push_rand_sqn(std::string imsi, uint8_t *rand_p, uint8_t *sqn){
  int                                     status = 0;
  MYSQL_RES                              *res;
  char                                    query[1000];
  int                                     query_length = 0;
  uint64_t                                sqn_decimal = 0;
  if(!db_desc->db_conn){
    Logger::amf_n1().error("Cannot connect to mysql");
    return;
  }
  if(!sqn || !rand_p){
    Logger::amf_n1().error("need sqn and rand");
    return;
  }
  sqn_decimal = ((uint64_t) sqn[0] << 40) | ((uint64_t) sqn[1] << 32) | ((uint64_t) sqn[2] << 24) | (sqn[3] << 16) | (sqn[4] << 8) | sqn[5];
  query_length = sprintf (query, "UPDATE `users` SET `rand`=UNHEX('");
  for (int i = 0; i < RAND_LENGTH; i++) {
    query_length += sprintf (&query[query_length], "%02x", rand_p[i]);
  }

  query_length += sprintf (&query[query_length], "'),`sqn`=%" PRIu64, sqn_decimal);
  query_length += sprintf (&query[query_length], " WHERE `users`.`imsi`='%s'", imsi.c_str());
  pthread_mutex_lock (&db_desc->db_cs_mutex);
  if (mysql_query (db_desc->db_conn, query)) {
    pthread_mutex_unlock (&db_desc->db_cs_mutex);
    Logger::amf_n1().error("Query execution failed: %s", mysql_error (db_desc->db_conn));
    return;
  }
  do{
    res = mysql_store_result (db_desc->db_conn);
    if(res){
      mysql_free_result (res);
    }else{
      if(mysql_field_count (db_desc->db_conn) == 0) {
        Logger::amf_n1().error("%lld rows affected", mysql_affected_rows (db_desc->db_conn));
      }else{                  /* some error occurred */
        Logger::amf_n1().error("Could not retrieve result set");
        break;
      }
    }
    if ((status = mysql_next_result (db_desc->db_conn)) > 0)
      Logger::amf_n1().error("Could not execute statement");
  }while(status == 0);
  pthread_mutex_unlock (&db_desc->db_cs_mutex);
  return;
}

void amf_n1::mysql_increment_sqn(std::string imsi){
  int                                     status;
  MYSQL_RES                              *res;
  char                                    query[1000];
  if (db_desc->db_conn == NULL) {
    Logger::amf_n1().error("Cannot connect to mysql");
    return;
  }
  sprintf (query, "UPDATE `users` SET `sqn` = `sqn` + 32 WHERE `users`.`imsi`='%s'", imsi.c_str());

  pthread_mutex_lock(&db_desc->db_cs_mutex);

  if (mysql_query (db_desc->db_conn, query)) {
    pthread_mutex_unlock (&db_desc->db_cs_mutex);
    Logger::amf_n1().error("Query execution failed: %s", mysql_error (db_desc->db_conn));
    return;
  }
  do{
    res = mysql_store_result (db_desc->db_conn);
    if (res) {
      mysql_free_result (res);
    } else {              
      if (mysql_field_count (db_desc->db_conn) == 0) {
        Logger::amf_n1().error("%lld rows affected", mysql_affected_rows (db_desc->db_conn));
      } else {      
        Logger::amf_n1().error("Could not retrieve result set");
        break;
      }
    }
    if ((status = mysql_next_result (db_desc->db_conn)) > 0)
      Logger::amf_n1().error("Could not execute statement");
  }while(status == 0);
  pthread_mutex_unlock (&db_desc->db_cs_mutex);
  return;
}
