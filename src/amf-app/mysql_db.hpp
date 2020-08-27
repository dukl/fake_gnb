#ifndef _MYSQL_DB_HANDLERS_H_
#define _MYSQL_DB_HANDLERS_H_

#include <stdint.h>
#include <pthread.h>
#include <mysql/mysql.h>
#include <netinet/in.h>
#include <string>

#define KEY_LENGTH  (16)
#define SQN_LENGTH  (6)
#define RAND_LENGTH (16)
typedef struct{
  uint8_t key[KEY_LENGTH];
  uint8_t sqn[SQN_LENGTH];
  uint8_t opc[KEY_LENGTH];
  uint8_t rand[RAND_LENGTH];
}mysql_auth_info_t;

typedef struct {
  /* The mysql reference connector object */
  MYSQL *db_conn;
  std::string server;
  std::string user;
  std::string password;
  std::string database;

  pthread_mutex_t db_cs_mutex;
} database_t;

#endif
